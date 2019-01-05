/*
   Copyright (C) 2014-2017 Alexandr Akulich <akulichalexander@gmail.com>

   This file is a part of TelegramQt library.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

 */

#include "CTelegramConnection.hpp"

#include <QDebug>

#include <QDateTime>
#include <QStringList>
#include <QTimer>

#include <QtEndian>

#ifdef NETWORK_LOGGING
#include <QDir>
#include <QFile>
#include <QTextStream>

const int valFieldWidth = 32;
QString formatTLValue(const TLValue &val)
{
    const QString text = val.toString();
    return QString(valFieldWidth - text.length(), QLatin1Char(' ')) + text;
}
#endif

#include "CAppInformation.hpp"
#include "CTelegramStream.hpp"
#include "CTelegramTransport.hpp"
#include "Debug_p.hpp"
#include "Utils.hpp"
#include "TelegramUtils.hpp"
#include "RpcProcessingContext.hpp"

#ifdef DEVELOPER_BUILD
#include "TLRpcDebug.hpp"
#endif

using namespace TelegramUtils;
using namespace Telegram;

static const quint32 s_defaultAuthInterval = 15000; // 15 sec

CTelegramConnection::CTelegramConnection(const CAppInformation *appInfo, QObject *parent) :
    QObject(parent),
    m_status(ConnectionStatusDisconnected),
    m_appInfo(appInfo),
    m_transport(0),
    m_authTimer(0),
    m_pingTimer(0),
    m_ackTimer(new QTimer(this)),
    m_authState(AuthStateNone),
    m_authId(0),
    m_authKeyAuxHash(0),
    m_serverSalt(0),
    m_sessionId(0),
    m_lastSentPingId(0),
    m_lastReceivedPingTime(0),
    m_lastSentPingTime(0),
    m_sequenceNumber(0),
    m_contentRelatedMessages(0),
    m_pingInterval(0),
    m_serverDisconnectionExtraTime(0),
    m_deltaTime(0),
    m_deltaTimeHeuristicState(DeltaTimeIsOk)
  #ifdef NETWORK_LOGGING
  , m_logFile(0)
  #endif
{
    m_ackTimer->setInterval(90 * 1000);
    m_ackTimer->setSingleShot(true);
    connect(m_ackTimer, &QTimer::timeout, this, &CTelegramConnection::onTimeToAckMessages);
}

void CTelegramConnection::setDcInfo(const TLDcOption &newDcInfo)
{
    m_dcInfo = newDcInfo;
}

void CTelegramConnection::setServerRsaKey(const Telegram::RsaKey &key)
{
    m_rsaKey = key;
}

void CTelegramConnection::connectToDc()
{
    if (m_status != ConnectionStatusDisconnected) {
        return;
    }

#ifdef DEVELOPER_BUILD
    qDebug() << Q_FUNC_INFO << m_dcInfo.id << m_dcInfo.ipAddress << m_dcInfo.port;
#endif

    if (m_transport->state() != QAbstractSocket::UnconnectedState) {
        m_transport->disconnectFromHost(); // Ensure that there is no connection
    }

    setStatus(ConnectionStatusConnecting, ConnectionStatusReasonLocal);
    setAuthState(AuthStateNone);
    m_transport->connectToHost(m_dcInfo.ipAddress, m_dcInfo.port);
}

void CTelegramConnection::disconnectFromDc()
{
    if (m_transport->state() == QAbstractSocket::UnconnectedState) {
        setStatus(ConnectionStatusDisconnected, ConnectionStatusReasonLocal);
    } else {
        setStatus(ConnectionStatusDisconnecting, ConnectionStatusReasonLocal);
        m_transport->disconnectFromHost();
    }
}

quint64 CTelegramConnection::formatClientTimeStamp(qint64 timeInMs)
{
     return formatTimeStamp(timeInMs) & ~quint64(3);
}

void CTelegramConnection::setTransport(CTelegramTransport *newTransport)
{
    m_transport = newTransport;

    connect(m_transport, &CTelegramTransport::stateChanged, this, &CTelegramConnection::onTransportStateChanged);
    connect(m_transport, &CTelegramTransport::packageReceived, this, &CTelegramConnection::onTransportPackageReceived);
    connect(m_transport, &CTelegramTransport::timeout, this, &CTelegramConnection::onTransportTimeout);
}

void CTelegramConnection::setAuthKey(const QByteArray &newAuthKey)
{
    if (newAuthKey.isEmpty()) {
        m_authKey.clear();
        m_authId = 0;
        m_authKeyAuxHash = 0;
    } else {
        m_authKey = newAuthKey;
        m_authId = Utils::getFingerprints(m_authKey, Utils::Lower64Bits);
        m_authKeyAuxHash = Utils::getFingerprints(m_authKey, Utils::Lower64Bits);
    }
#ifdef TELEGRAMQT_DEBUG_REVEAL_SECRETS
    qDebug() << Q_FUNC_INFO << "key:" << newAuthKey.toHex() << "keyId:" << m_authId << "auxHash:" << m_authKeyAuxHash;
#endif
}

void CTelegramConnection::setDeltaTime(const qint32 newDt)
{
    m_deltaTime = newDt;
}

void CTelegramConnection::initAuth()
{
    if (m_authState == AuthStateNone) {
        if (!m_rsaKey.isValid()) {
            qWarning() << "CTelegramConnection::initAuth(): RSA key is not valid!";
        }
        m_authRetryId = 0;
        Utils::randomBytes(m_clientNonce.data, m_clientNonce.size());
        requestPqAuthorization();
    }
}

void CTelegramConnection::requestPqAuthorization()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ReqPq;
    outputStream << m_clientNonce;
    sendPlainPackage(output);
    setAuthState(AuthStatePqRequested);
}

void CTelegramConnection::setKeepAliveSettings(quint32 interval, quint32 serverDisconnectionExtraTime)
{
    qDebug() << Q_FUNC_INFO << interval << serverDisconnectionExtraTime;

    m_serverDisconnectionExtraTime = serverDisconnectionExtraTime;

    if (m_pingInterval == interval) {
        return;
    }

    m_pingInterval = interval;

    if (interval && m_authState >= AuthStateHaveAKey) {
        startPingTimer();
    } else {
        stopPingTimer();
    }
}

quint64 CTelegramConnection::requestPhoneCode(const QString &phoneNumber)
{
    if (!m_appInfo || !m_appInfo->isValid()) {
        qWarning() << Q_FUNC_INFO << "Unable to request phone code without a valid application data";
        return 0;
    }
    qDebug() << Q_FUNC_INFO << "requestPhoneCode" << Telegram::Utils::maskPhoneNumber(phoneNumber) << "dc" << m_dcInfo.id;
    return authSendCode(phoneNumber, 0, m_appInfo->appId(), m_appInfo->appHash(), m_appInfo->languageCode());
}

quint64 CTelegramConnection::signIn(const QString &phoneNumber, const QString &authCode)
{
    qDebug() << "SignIn with number " << Telegram::Utils::maskPhoneNumber(phoneNumber);

    return authSignIn(phoneNumber, m_authCodeHash, authCode);
}

quint64 CTelegramConnection::signUp(const QString &phoneNumber, const QString &authCode, const QString &firstName, const QString &lastName)
{
    qDebug() << "SignUp with number " << Telegram::Utils::maskPhoneNumber(phoneNumber);
    return authSignUp(phoneNumber, m_authCodeHash, authCode, firstName, lastName);
}

void CTelegramConnection::downloadFile(const TLInputFileLocation &inputLocation, quint32 offset, quint32 limit, quint32 requestId)
{
    if (m_requestedFilesIds.contains(requestId)) {
        // Prevent from (really possible) repeated request.
        return;
    }

    const quint64 messageId = uploadGetFile(inputLocation, offset, limit);
    qDebug() << Q_FUNC_INFO << messageId << "offset:" << offset << "limit:" << limit << "request:" << requestId;

    m_requestedFilesIds.insert(messageId, requestId);
}

void CTelegramConnection::uploadFile(quint64 fileId, quint32 filePart, const QByteArray &bytes, quint32 requestId)
{
    qDebug() << Q_FUNC_INFO << "id" << fileId << "part" << filePart << "size" << bytes.count() << "request" << requestId;
    const quint64 messageId = uploadSaveFilePart(fileId, filePart, bytes);

    m_requestedFilesIds.insert(messageId, requestId);
}

quint64 CTelegramConnection::sendMessage(const TLInputPeer &peer, const QString &message, quint64 randomMessageId)
{
    if (message.length() > 4095) { // 4096 - 1
        qDebug() << Q_FUNC_INFO << "Can not send such long message due to server limitation. Current maximum length is 4095 characters.";
        return 0;
    }

    const TLReplyMarkup replyMarkup;
    const TLVector<TLMessageEntity> entities;

    return messagesSendMessage(/* flags */0, peer, /* reply to message id*/ 0, message, randomMessageId, replyMarkup, entities);
}

quint64 CTelegramConnection::sendMedia(const TLInputPeer &peer, const TLInputMedia &media, quint64 randomMessageId)
{
    const TLReplyMarkup replyMarkup;
    return messagesSendMedia(/* flags */0, peer, /* reply to message id*/ 0, media, randomMessageId, replyMarkup);
}

// Generated Telegram API methods implementation
quint64 CTelegramConnection::accountAcceptAuthorization(quint32 botId, const QString &scope, const QString &publicKey, const TLVector<TLSecureValueHash> &valueHashes, const TLSecureCredentialsEncrypted &credentials)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountAcceptAuthorization;
    outputStream << botId;
    outputStream << scope;
    outputStream << publicKey;
    outputStream << valueHashes;
    outputStream << credentials;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountCancelPasswordEmail()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountCancelPasswordEmail;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountChangePhone(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountChangePhone;
    outputStream << phoneNumber;
    outputStream << phoneCodeHash;
    outputStream << phoneCode;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountCheckUsername(const QString &username)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountCheckUsername;
    outputStream << username;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountConfirmPasswordEmail(const QString &code)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountConfirmPasswordEmail;
    outputStream << code;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountConfirmPhone(const QString &phoneCodeHash, const QString &phoneCode)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountConfirmPhone;
    outputStream << phoneCodeHash;
    outputStream << phoneCode;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountDeleteAccount(const QString &reason)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountDeleteAccount;
    outputStream << reason;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountDeleteSecureValue(const TLVector<TLSecureValueType> &types)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountDeleteSecureValue;
    outputStream << types;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountFinishTakeoutSession(quint32 flags)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountFinishTakeoutSession;
    outputStream << flags;
    // (flags & 1 << 0) stands for success "true" value
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetAccountTTL()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetAccountTTL;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetAllSecureValues()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetAllSecureValues;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetAuthorizationForm(quint32 botId, const QString &scope, const QString &publicKey)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetAuthorizationForm;
    outputStream << botId;
    outputStream << scope;
    outputStream << publicKey;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetAuthorizations()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetAuthorizations;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetContactSignUpNotification()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetContactSignUpNotification;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetNotifyExceptions(quint32 flags, const TLInputNotifyPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetNotifyExceptions;
    outputStream << flags;
    // (flags & 1 << 1) stands for compareSound "true" value
    if (flags & 1 << 0) {
        outputStream << peer;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetNotifySettings(const TLInputNotifyPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetNotifySettings;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetPassword()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetPassword;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetPasswordSettings(const TLInputCheckPasswordSRP &password)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetPasswordSettings;
    outputStream << password;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetPrivacy(const TLInputPrivacyKey &key)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetPrivacy;
    outputStream << key;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetSecureValue(const TLVector<TLSecureValueType> &types)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetSecureValue;
    outputStream << types;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetTmpPassword(const TLInputCheckPasswordSRP &password, quint32 period)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetTmpPassword;
    outputStream << password;
    outputStream << period;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetWallPapers()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetWallPapers;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountGetWebAuthorizations()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountGetWebAuthorizations;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountInitTakeoutSession(quint32 flags, quint32 fileMaxSize)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountInitTakeoutSession;
    outputStream << flags;
    // (flags & 1 << 0) stands for contacts "true" value
    // (flags & 1 << 1) stands for messageUsers "true" value
    // (flags & 1 << 2) stands for messageChats "true" value
    // (flags & 1 << 3) stands for messageMegagroups "true" value
    // (flags & 1 << 4) stands for messageChannels "true" value
    // (flags & 1 << 5) stands for files "true" value
    if (flags & 1 << 5) {
        outputStream << fileMaxSize;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountRegisterDevice(quint32 tokenType, const QString &token, bool appSandbox, const QByteArray &secret, const TLVector<quint32> &otherUids)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountRegisterDevice;
    outputStream << tokenType;
    outputStream << token;
    outputStream << appSandbox;
    outputStream << secret;
    outputStream << otherUids;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountReportPeer(const TLInputPeer &peer, const TLReportReason &reason)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountReportPeer;
    outputStream << peer;
    outputStream << reason;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountResendPasswordEmail()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountResendPasswordEmail;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountResetAuthorization(quint64 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountResetAuthorization;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountResetNotifySettings()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountResetNotifySettings;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountResetWebAuthorization(quint64 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountResetWebAuthorization;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountResetWebAuthorizations()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountResetWebAuthorizations;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountSaveSecureValue(const TLInputSecureValue &value, quint64 secureSecretId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountSaveSecureValue;
    outputStream << value;
    outputStream << secureSecretId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountSendChangePhoneCode(quint32 flags, const QString &phoneNumber, bool currentNumber)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountSendChangePhoneCode;
    outputStream << flags;
    // (flags & 1 << 0) stands for allowFlashcall "true" value
    outputStream << phoneNumber;
    if (flags & 1 << 0) {
        outputStream << currentNumber;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountSendConfirmPhoneCode(quint32 flags, const QString &hash, bool currentNumber)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountSendConfirmPhoneCode;
    outputStream << flags;
    // (flags & 1 << 0) stands for allowFlashcall "true" value
    outputStream << hash;
    if (flags & 1 << 0) {
        outputStream << currentNumber;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountSendVerifyEmailCode(const QString &email)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountSendVerifyEmailCode;
    outputStream << email;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountSendVerifyPhoneCode(quint32 flags, const QString &phoneNumber, bool currentNumber)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountSendVerifyPhoneCode;
    outputStream << flags;
    // (flags & 1 << 0) stands for allowFlashcall "true" value
    outputStream << phoneNumber;
    if (flags & 1 << 0) {
        outputStream << currentNumber;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountSetAccountTTL(const TLAccountDaysTTL &ttl)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountSetAccountTTL;
    outputStream << ttl;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountSetContactSignUpNotification(bool silent)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountSetContactSignUpNotification;
    outputStream << silent;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountSetPrivacy(const TLInputPrivacyKey &key, const TLVector<TLInputPrivacyRule> &rules)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountSetPrivacy;
    outputStream << key;
    outputStream << rules;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountUnregisterDevice(quint32 tokenType, const QString &token, const TLVector<quint32> &otherUids)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountUnregisterDevice;
    outputStream << tokenType;
    outputStream << token;
    outputStream << otherUids;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountUpdateDeviceLocked(quint32 period)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountUpdateDeviceLocked;
    outputStream << period;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountUpdateNotifySettings(const TLInputNotifyPeer &peer, const TLInputPeerNotifySettings &settings)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountUpdateNotifySettings;
    outputStream << peer;
    outputStream << settings;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountUpdatePasswordSettings(const TLInputCheckPasswordSRP &password, const TLAccountPasswordInputSettings &newSettings)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountUpdatePasswordSettings;
    outputStream << password;
    outputStream << newSettings;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountUpdateProfile(quint32 flags, const QString &firstName, const QString &lastName, const QString &about)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountUpdateProfile;
    outputStream << flags;
    if (flags & 1 << 0) {
        outputStream << firstName;
    }
    if (flags & 1 << 1) {
        outputStream << lastName;
    }
    if (flags & 1 << 2) {
        outputStream << about;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountUpdateStatus(bool offline)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountUpdateStatus;
    outputStream << offline;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountUpdateUsername(const QString &username)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountUpdateUsername;
    outputStream << username;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountVerifyEmail(const QString &email, const QString &code)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountVerifyEmail;
    outputStream << email;
    outputStream << code;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::accountVerifyPhone(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AccountVerifyPhone;
    outputStream << phoneNumber;
    outputStream << phoneCodeHash;
    outputStream << phoneCode;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authBindTempAuthKey(quint64 permAuthKeyId, quint64 nonce, quint32 expiresAt, const QByteArray &encryptedMessage)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthBindTempAuthKey;
    outputStream << permAuthKeyId;
    outputStream << nonce;
    outputStream << expiresAt;
    outputStream << encryptedMessage;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authCancelCode(const QString &phoneNumber, const QString &phoneCodeHash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthCancelCode;
    outputStream << phoneNumber;
    outputStream << phoneCodeHash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authCheckPassword(const TLInputCheckPasswordSRP &password)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthCheckPassword;
    outputStream << password;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authDropTempAuthKeys(const TLVector<quint64> &exceptAuthKeys)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthDropTempAuthKeys;
    outputStream << exceptAuthKeys;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authExportAuthorization(quint32 dcId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthExportAuthorization;
    outputStream << dcId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authImportAuthorization(quint32 id, const QByteArray &bytes)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthImportAuthorization;
    outputStream << id;
    outputStream << bytes;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authImportBotAuthorization(quint32 flags, quint32 apiId, const QString &apiHash, const QString &botAuthToken)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthImportBotAuthorization;
    outputStream << flags;
    outputStream << apiId;
    outputStream << apiHash;
    outputStream << botAuthToken;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authLogOut()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthLogOut;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authRecoverPassword(const QString &code)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthRecoverPassword;
    outputStream << code;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authRequestPasswordRecovery()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthRequestPasswordRecovery;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authResendCode(const QString &phoneNumber, const QString &phoneCodeHash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthResendCode;
    outputStream << phoneNumber;
    outputStream << phoneCodeHash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authResetAuthorizations()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthResetAuthorizations;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authSendCode(quint32 flags, const QString &phoneNumber, bool currentNumber, quint32 apiId, const QString &apiHash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthSendCode;
    outputStream << flags;
    // (flags & 1 << 0) stands for allowFlashcall "true" value
    outputStream << phoneNumber;
    if (flags & 1 << 0) {
        outputStream << currentNumber;
    }
    outputStream << apiId;
    outputStream << apiHash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authSignIn(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthSignIn;
    outputStream << phoneNumber;
    outputStream << phoneCodeHash;
    outputStream << phoneCode;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::authSignUp(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode, const QString &firstName, const QString &lastName)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::AuthSignUp;
    outputStream << phoneNumber;
    outputStream << phoneCodeHash;
    outputStream << phoneCode;
    outputStream << firstName;
    outputStream << lastName;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsCheckUsername(const TLInputChannel &channel, const QString &username)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsCheckUsername;
    outputStream << channel;
    outputStream << username;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsCreateChannel(quint32 flags, const QString &title, const QString &about)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsCreateChannel;
    outputStream << flags;
    // (flags & 1 << 0) stands for broadcast "true" value
    // (flags & 1 << 1) stands for megagroup "true" value
    outputStream << title;
    outputStream << about;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsDeleteChannel(const TLInputChannel &channel)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsDeleteChannel;
    outputStream << channel;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsDeleteHistory(const TLInputChannel &channel, quint32 maxId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsDeleteHistory;
    outputStream << channel;
    outputStream << maxId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsDeleteMessages(const TLInputChannel &channel, const TLVector<quint32> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsDeleteMessages;
    outputStream << channel;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsDeleteUserHistory(const TLInputChannel &channel, const TLInputUser &userId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsDeleteUserHistory;
    outputStream << channel;
    outputStream << userId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsEditAbout(const TLInputChannel &channel, const QString &about)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsEditAbout;
    outputStream << channel;
    outputStream << about;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsEditAdmin(const TLInputChannel &channel, const TLInputUser &userId, const TLChannelAdminRights &adminRights)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsEditAdmin;
    outputStream << channel;
    outputStream << userId;
    outputStream << adminRights;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsEditBanned(const TLInputChannel &channel, const TLInputUser &userId, const TLChannelBannedRights &bannedRights)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsEditBanned;
    outputStream << channel;
    outputStream << userId;
    outputStream << bannedRights;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsEditPhoto(const TLInputChannel &channel, const TLInputChatPhoto &photo)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsEditPhoto;
    outputStream << channel;
    outputStream << photo;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsEditTitle(const TLInputChannel &channel, const QString &title)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsEditTitle;
    outputStream << channel;
    outputStream << title;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsExportInvite(const TLInputChannel &channel)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsExportInvite;
    outputStream << channel;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsExportMessageLink(const TLInputChannel &channel, quint32 id, bool grouped)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsExportMessageLink;
    outputStream << channel;
    outputStream << id;
    outputStream << grouped;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsGetAdminLog(quint32 flags, const TLInputChannel &channel, const QString &q, const TLChannelAdminLogEventsFilter &eventsFilter, const TLVector<TLInputUser> &admins, quint64 maxId, quint64 minId, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsGetAdminLog;
    outputStream << flags;
    outputStream << channel;
    outputStream << q;
    if (flags & 1 << 0) {
        outputStream << eventsFilter;
    }
    if (flags & 1 << 1) {
        outputStream << admins;
    }
    outputStream << maxId;
    outputStream << minId;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsGetAdminedPublicChannels()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsGetAdminedPublicChannels;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsGetChannels(const TLVector<TLInputChannel> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsGetChannels;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsGetFullChannel(const TLInputChannel &channel)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsGetFullChannel;
    outputStream << channel;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsGetLeftChannels(quint32 offset)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsGetLeftChannels;
    outputStream << offset;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsGetMessages(const TLInputChannel &channel, const TLVector<TLInputMessage> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsGetMessages;
    outputStream << channel;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsGetParticipant(const TLInputChannel &channel, const TLInputUser &userId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsGetParticipant;
    outputStream << channel;
    outputStream << userId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsGetParticipants(const TLInputChannel &channel, const TLChannelParticipantsFilter &filter, quint32 offset, quint32 limit, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsGetParticipants;
    outputStream << channel;
    outputStream << filter;
    outputStream << offset;
    outputStream << limit;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsInviteToChannel(const TLInputChannel &channel, const TLVector<TLInputUser> &users)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsInviteToChannel;
    outputStream << channel;
    outputStream << users;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsJoinChannel(const TLInputChannel &channel)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsJoinChannel;
    outputStream << channel;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsLeaveChannel(const TLInputChannel &channel)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsLeaveChannel;
    outputStream << channel;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsReadHistory(const TLInputChannel &channel, quint32 maxId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsReadHistory;
    outputStream << channel;
    outputStream << maxId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsReadMessageContents(const TLInputChannel &channel, const TLVector<quint32> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsReadMessageContents;
    outputStream << channel;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsReportSpam(const TLInputChannel &channel, const TLInputUser &userId, const TLVector<quint32> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsReportSpam;
    outputStream << channel;
    outputStream << userId;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsSetStickers(const TLInputChannel &channel, const TLInputStickerSet &stickerset)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsSetStickers;
    outputStream << channel;
    outputStream << stickerset;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsToggleInvites(const TLInputChannel &channel, bool enabled)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsToggleInvites;
    outputStream << channel;
    outputStream << enabled;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsTogglePreHistoryHidden(const TLInputChannel &channel, bool enabled)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsTogglePreHistoryHidden;
    outputStream << channel;
    outputStream << enabled;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsToggleSignatures(const TLInputChannel &channel, bool enabled)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsToggleSignatures;
    outputStream << channel;
    outputStream << enabled;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::channelsUpdateUsername(const TLInputChannel &channel, const QString &username)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ChannelsUpdateUsername;
    outputStream << channel;
    outputStream << username;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsBlock(const TLInputUser &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsBlock;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsDeleteByPhones(const TLVector<QString> &phones)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsDeleteByPhones;
    outputStream << phones;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsDeleteContact(const TLInputUser &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsDeleteContact;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsDeleteContacts(const TLVector<TLInputUser> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsDeleteContacts;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsGetBlocked(quint32 offset, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsGetBlocked;
    outputStream << offset;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsGetContactIDs(quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsGetContactIDs;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsGetContacts(quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsGetContacts;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsGetSaved()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsGetSaved;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsGetStatuses()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsGetStatuses;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsGetTopPeers(quint32 flags, quint32 offset, quint32 limit, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsGetTopPeers;
    outputStream << flags;
    // (flags & 1 << 0) stands for correspondents "true" value
    // (flags & 1 << 1) stands for botsPm "true" value
    // (flags & 1 << 2) stands for botsInline "true" value
    // (flags & 1 << 3) stands for phoneCalls "true" value
    // (flags & 1 << 10) stands for groups "true" value
    // (flags & 1 << 15) stands for channels "true" value
    outputStream << offset;
    outputStream << limit;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsImportContacts(const TLVector<TLInputContact> &contacts)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsImportContacts;
    outputStream << contacts;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsResetSaved()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsResetSaved;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsResetTopPeerRating(const TLTopPeerCategory &category, const TLInputPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsResetTopPeerRating;
    outputStream << category;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsResolveUsername(const QString &username)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsResolveUsername;
    outputStream << username;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsSearch(const QString &q, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsSearch;
    outputStream << q;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsToggleTopPeers(bool enabled)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsToggleTopPeers;
    outputStream << enabled;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::contactsUnblock(const TLInputUser &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::ContactsUnblock;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpAcceptTermsOfService(const TLDataJSON &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpAcceptTermsOfService;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpEditUserInfo(const TLInputUser &userId, const QString &message, const TLVector<TLMessageEntity> &entities)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpEditUserInfo;
    outputStream << userId;
    outputStream << message;
    outputStream << entities;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetAppChangelog(const QString &prevAppVersion)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetAppChangelog;
    outputStream << prevAppVersion;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetAppConfig()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetAppConfig;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetAppUpdate(const QString &source)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetAppUpdate;
    outputStream << source;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetCdnConfig()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetCdnConfig;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetConfig()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetConfig;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetDeepLinkInfo(const QString &path)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetDeepLinkInfo;
    outputStream << path;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetInviteText()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetInviteText;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetNearestDc()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetNearestDc;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetPassportConfig(quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetPassportConfig;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetProxyData()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetProxyData;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetRecentMeUrls(const QString &referer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetRecentMeUrls;
    outputStream << referer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetSupport()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetSupport;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetSupportName()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetSupportName;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetTermsOfServiceUpdate()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetTermsOfServiceUpdate;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpGetUserInfo(const TLInputUser &userId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpGetUserInfo;
    outputStream << userId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpSaveAppLog(const TLVector<TLInputAppEvent> &events)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpSaveAppLog;
    outputStream << events;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::helpSetBotUpdatesStatus(quint32 pendingUpdatesCount, const QString &message)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::HelpSetBotUpdatesStatus;
    outputStream << pendingUpdatesCount;
    outputStream << message;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesAcceptEncryption(const TLInputEncryptedChat &peer, const QByteArray &gB, quint64 keyFingerprint)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesAcceptEncryption;
    outputStream << peer;
    outputStream << gB;
    outputStream << keyFingerprint;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesAddChatUser(quint32 chatId, const TLInputUser &userId, quint32 fwdLimit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesAddChatUser;
    outputStream << chatId;
    outputStream << userId;
    outputStream << fwdLimit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesCheckChatInvite(const QString &hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesCheckChatInvite;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesClearAllDrafts()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesClearAllDrafts;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesClearRecentStickers(quint32 flags)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesClearRecentStickers;
    outputStream << flags;
    // (flags & 1 << 0) stands for attached "true" value
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesCreateChat(const TLVector<TLInputUser> &users, const QString &title)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesCreateChat;
    outputStream << users;
    outputStream << title;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesDeleteChatUser(quint32 chatId, const TLInputUser &userId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesDeleteChatUser;
    outputStream << chatId;
    outputStream << userId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesDeleteHistory(quint32 flags, const TLInputPeer &peer, quint32 maxId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesDeleteHistory;
    outputStream << flags;
    // (flags & 1 << 0) stands for justClear "true" value
    outputStream << peer;
    outputStream << maxId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesDeleteMessages(quint32 flags, const TLVector<quint32> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesDeleteMessages;
    outputStream << flags;
    // (flags & 1 << 0) stands for revoke "true" value
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesDiscardEncryption(quint32 chatId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesDiscardEncryption;
    outputStream << chatId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesEditChatAdmin(quint32 chatId, const TLInputUser &userId, bool isAdmin)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesEditChatAdmin;
    outputStream << chatId;
    outputStream << userId;
    outputStream << isAdmin;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesEditChatPhoto(quint32 chatId, const TLInputChatPhoto &photo)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesEditChatPhoto;
    outputStream << chatId;
    outputStream << photo;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesEditChatTitle(quint32 chatId, const QString &title)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesEditChatTitle;
    outputStream << chatId;
    outputStream << title;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesEditInlineBotMessage(quint32 flags, const TLInputBotInlineMessageID &id, const QString &message, const TLInputMedia &media, const TLReplyMarkup &replyMarkup, const TLVector<TLMessageEntity> &entities)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesEditInlineBotMessage;
    outputStream << flags;
    // (flags & 1 << 1) stands for noWebpage "true" value
    outputStream << id;
    if (flags & 1 << 11) {
        outputStream << message;
    }
    if (flags & 1 << 14) {
        outputStream << media;
    }
    if (flags & 1 << 2) {
        outputStream << replyMarkup;
    }
    if (flags & 1 << 3) {
        outputStream << entities;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesEditMessage(quint32 flags, const TLInputPeer &peer, quint32 id, const QString &message, const TLInputMedia &media, const TLReplyMarkup &replyMarkup, const TLVector<TLMessageEntity> &entities)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesEditMessage;
    outputStream << flags;
    // (flags & 1 << 1) stands for noWebpage "true" value
    outputStream << peer;
    outputStream << id;
    if (flags & 1 << 11) {
        outputStream << message;
    }
    if (flags & 1 << 14) {
        outputStream << media;
    }
    if (flags & 1 << 2) {
        outputStream << replyMarkup;
    }
    if (flags & 1 << 3) {
        outputStream << entities;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesExportChatInvite(quint32 chatId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesExportChatInvite;
    outputStream << chatId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesFaveSticker(const TLInputDocument &id, bool unfave)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesFaveSticker;
    outputStream << id;
    outputStream << unfave;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesForwardMessages(quint32 flags, const TLInputPeer &fromPeer, const TLVector<quint32> &id, const TLVector<quint64> &randomId, const TLInputPeer &toPeer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesForwardMessages;
    outputStream << flags;
    // (flags & 1 << 5) stands for silent "true" value
    // (flags & 1 << 6) stands for background "true" value
    // (flags & 1 << 8) stands for withMyScore "true" value
    // (flags & 1 << 9) stands for grouped "true" value
    outputStream << fromPeer;
    outputStream << id;
    outputStream << randomId;
    outputStream << toPeer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetAllChats(const TLVector<quint32> &exceptIds)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetAllChats;
    outputStream << exceptIds;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetAllDrafts()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetAllDrafts;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetAllStickers(quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetAllStickers;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetArchivedStickers(quint32 flags, quint64 offsetId, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetArchivedStickers;
    outputStream << flags;
    // (flags & 1 << 0) stands for masks "true" value
    outputStream << offsetId;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetAttachedStickers(const TLInputStickeredMedia &media)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetAttachedStickers;
    outputStream << media;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetBotCallbackAnswer(quint32 flags, const TLInputPeer &peer, quint32 msgId, const QByteArray &data)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetBotCallbackAnswer;
    outputStream << flags;
    // (flags & 1 << 1) stands for game "true" value
    outputStream << peer;
    outputStream << msgId;
    if (flags & 1 << 0) {
        outputStream << data;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetChats(const TLVector<quint32> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetChats;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetCommonChats(const TLInputUser &userId, quint32 maxId, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetCommonChats;
    outputStream << userId;
    outputStream << maxId;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetDhConfig(quint32 version, quint32 randomLength)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetDhConfig;
    outputStream << version;
    outputStream << randomLength;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetDialogUnreadMarks()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetDialogUnreadMarks;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetDialogs(quint32 flags, quint32 offsetDate, quint32 offsetId, const TLInputPeer &offsetPeer, quint32 limit, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetDialogs;
    outputStream << flags;
    // (flags & 1 << 0) stands for excludePinned "true" value
    outputStream << offsetDate;
    outputStream << offsetId;
    outputStream << offsetPeer;
    outputStream << limit;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetDocumentByHash(const QByteArray &sha256, quint32 size, const QString &mimeType)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetDocumentByHash;
    outputStream << sha256;
    outputStream << size;
    outputStream << mimeType;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetFavedStickers(quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetFavedStickers;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetFeaturedStickers(quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetFeaturedStickers;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetFullChat(quint32 chatId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetFullChat;
    outputStream << chatId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetGameHighScores(const TLInputPeer &peer, quint32 id, const TLInputUser &userId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetGameHighScores;
    outputStream << peer;
    outputStream << id;
    outputStream << userId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetHistory(const TLInputPeer &peer, quint32 offsetId, quint32 offsetDate, quint32 addOffset, quint32 limit, quint32 maxId, quint32 minId, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetHistory;
    outputStream << peer;
    outputStream << offsetId;
    outputStream << offsetDate;
    outputStream << addOffset;
    outputStream << limit;
    outputStream << maxId;
    outputStream << minId;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetInlineBotResults(quint32 flags, const TLInputUser &bot, const TLInputPeer &peer, const TLInputGeoPoint &geoPoint, const QString &query, const QString &offset)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetInlineBotResults;
    outputStream << flags;
    outputStream << bot;
    outputStream << peer;
    if (flags & 1 << 0) {
        outputStream << geoPoint;
    }
    outputStream << query;
    outputStream << offset;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetInlineGameHighScores(const TLInputBotInlineMessageID &id, const TLInputUser &userId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetInlineGameHighScores;
    outputStream << id;
    outputStream << userId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetMaskStickers(quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetMaskStickers;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetMessageEditData(const TLInputPeer &peer, quint32 id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetMessageEditData;
    outputStream << peer;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetMessages(const TLVector<TLInputMessage> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetMessages;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetMessagesViews(const TLInputPeer &peer, const TLVector<quint32> &id, bool increment)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetMessagesViews;
    outputStream << peer;
    outputStream << id;
    outputStream << increment;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetOnlines(const TLInputPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetOnlines;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetPeerDialogs(const TLVector<TLInputDialogPeer> &peers)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetPeerDialogs;
    outputStream << peers;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetPeerSettings(const TLInputPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetPeerSettings;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetPinnedDialogs()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetPinnedDialogs;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetPollResults(const TLInputPeer &peer, quint32 msgId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetPollResults;
    outputStream << peer;
    outputStream << msgId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetRecentLocations(const TLInputPeer &peer, quint32 limit, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetRecentLocations;
    outputStream << peer;
    outputStream << limit;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetRecentStickers(quint32 flags, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetRecentStickers;
    outputStream << flags;
    // (flags & 1 << 0) stands for attached "true" value
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetSavedGifs(quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetSavedGifs;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetSplitRanges()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetSplitRanges;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetStatsURL(const TLInputPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetStatsURL;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetStickerSet(const TLInputStickerSet &stickerset)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetStickerSet;
    outputStream << stickerset;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetStickers(const QString &emoticon, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetStickers;
    outputStream << emoticon;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetUnreadMentions(const TLInputPeer &peer, quint32 offsetId, quint32 addOffset, quint32 limit, quint32 maxId, quint32 minId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetUnreadMentions;
    outputStream << peer;
    outputStream << offsetId;
    outputStream << addOffset;
    outputStream << limit;
    outputStream << maxId;
    outputStream << minId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetWebPage(const QString &url, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetWebPage;
    outputStream << url;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesGetWebPagePreview(quint32 flags, const QString &message, const TLVector<TLMessageEntity> &entities)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesGetWebPagePreview;
    outputStream << flags;
    outputStream << message;
    if (flags & 1 << 3) {
        outputStream << entities;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesHideReportSpam(const TLInputPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesHideReportSpam;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesImportChatInvite(const QString &hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesImportChatInvite;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesInstallStickerSet(const TLInputStickerSet &stickerset, bool archived)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesInstallStickerSet;
    outputStream << stickerset;
    outputStream << archived;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesMarkDialogUnread(quint32 flags, const TLInputDialogPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesMarkDialogUnread;
    outputStream << flags;
    // (flags & 1 << 0) stands for unread "true" value
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesMigrateChat(quint32 chatId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesMigrateChat;
    outputStream << chatId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReadEncryptedHistory(const TLInputEncryptedChat &peer, quint32 maxDate)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReadEncryptedHistory;
    outputStream << peer;
    outputStream << maxDate;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReadFeaturedStickers(const TLVector<quint64> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReadFeaturedStickers;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReadHistory(const TLInputPeer &peer, quint32 maxId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReadHistory;
    outputStream << peer;
    outputStream << maxId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReadMentions(const TLInputPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReadMentions;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReadMessageContents(const TLVector<quint32> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReadMessageContents;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReceivedMessages(quint32 maxId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReceivedMessages;
    outputStream << maxId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReceivedQueue(quint32 maxQts)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReceivedQueue;
    outputStream << maxQts;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReorderPinnedDialogs(quint32 flags, const TLVector<TLInputDialogPeer> &order)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReorderPinnedDialogs;
    outputStream << flags;
    // (flags & 1 << 0) stands for force "true" value
    outputStream << order;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReorderStickerSets(quint32 flags, const TLVector<quint64> &order)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReorderStickerSets;
    outputStream << flags;
    // (flags & 1 << 0) stands for masks "true" value
    outputStream << order;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReport(const TLInputPeer &peer, const TLVector<quint32> &id, const TLReportReason &reason)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReport;
    outputStream << peer;
    outputStream << id;
    outputStream << reason;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReportEncryptedSpam(const TLInputEncryptedChat &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReportEncryptedSpam;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesReportSpam(const TLInputPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesReportSpam;
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesRequestEncryption(const TLInputUser &userId, quint32 randomId, const QByteArray &gA)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesRequestEncryption;
    outputStream << userId;
    outputStream << randomId;
    outputStream << gA;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSaveDraft(quint32 flags, quint32 replyToMsgId, const TLInputPeer &peer, const QString &message, const TLVector<TLMessageEntity> &entities)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSaveDraft;
    outputStream << flags;
    // (flags & 1 << 1) stands for noWebpage "true" value
    if (flags & 1 << 0) {
        outputStream << replyToMsgId;
    }
    outputStream << peer;
    outputStream << message;
    if (flags & 1 << 3) {
        outputStream << entities;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSaveGif(const TLInputDocument &id, bool unsave)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSaveGif;
    outputStream << id;
    outputStream << unsave;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSaveRecentSticker(quint32 flags, const TLInputDocument &id, bool unsave)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSaveRecentSticker;
    outputStream << flags;
    // (flags & 1 << 0) stands for attached "true" value
    outputStream << id;
    outputStream << unsave;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSearch(quint32 flags, const TLInputPeer &peer, const QString &q, const TLInputUser &fromId, const TLMessagesFilter &filter, quint32 minDate, quint32 maxDate, quint32 offsetId, quint32 addOffset, quint32 limit, quint32 maxId, quint32 minId, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSearch;
    outputStream << flags;
    outputStream << peer;
    outputStream << q;
    if (flags & 1 << 0) {
        outputStream << fromId;
    }
    outputStream << filter;
    outputStream << minDate;
    outputStream << maxDate;
    outputStream << offsetId;
    outputStream << addOffset;
    outputStream << limit;
    outputStream << maxId;
    outputStream << minId;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSearchGifs(const QString &q, quint32 offset)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSearchGifs;
    outputStream << q;
    outputStream << offset;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSearchGlobal(const QString &q, quint32 offsetDate, const TLInputPeer &offsetPeer, quint32 offsetId, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSearchGlobal;
    outputStream << q;
    outputStream << offsetDate;
    outputStream << offsetPeer;
    outputStream << offsetId;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSearchStickerSets(quint32 flags, const QString &q, quint32 hash)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSearchStickerSets;
    outputStream << flags;
    // (flags & 1 << 0) stands for excludeFeatured "true" value
    outputStream << q;
    outputStream << hash;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendEncrypted(const TLInputEncryptedChat &peer, quint64 randomId, const QByteArray &data)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendEncrypted;
    outputStream << peer;
    outputStream << randomId;
    outputStream << data;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendEncryptedFile(const TLInputEncryptedChat &peer, quint64 randomId, const QByteArray &data, const TLInputEncryptedFile &file)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendEncryptedFile;
    outputStream << peer;
    outputStream << randomId;
    outputStream << data;
    outputStream << file;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendEncryptedService(const TLInputEncryptedChat &peer, quint64 randomId, const QByteArray &data)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendEncryptedService;
    outputStream << peer;
    outputStream << randomId;
    outputStream << data;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendInlineBotResult(quint32 flags, const TLInputPeer &peer, quint32 replyToMsgId, quint64 randomId, quint64 queryId, const QString &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendInlineBotResult;
    outputStream << flags;
    // (flags & 1 << 5) stands for silent "true" value
    // (flags & 1 << 6) stands for background "true" value
    // (flags & 1 << 7) stands for clearDraft "true" value
    // (flags & 1 << 11) stands for hideVia "true" value
    outputStream << peer;
    if (flags & 1 << 0) {
        outputStream << replyToMsgId;
    }
    outputStream << randomId;
    outputStream << queryId;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendMedia(quint32 flags, const TLInputPeer &peer, quint32 replyToMsgId, const TLInputMedia &media, const QString &message, quint64 randomId, const TLReplyMarkup &replyMarkup, const TLVector<TLMessageEntity> &entities)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendMedia;
    outputStream << flags;
    // (flags & 1 << 5) stands for silent "true" value
    // (flags & 1 << 6) stands for background "true" value
    // (flags & 1 << 7) stands for clearDraft "true" value
    outputStream << peer;
    if (flags & 1 << 0) {
        outputStream << replyToMsgId;
    }
    outputStream << media;
    outputStream << message;
    outputStream << randomId;
    if (flags & 1 << 2) {
        outputStream << replyMarkup;
    }
    if (flags & 1 << 3) {
        outputStream << entities;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendMessage(quint32 flags, const TLInputPeer &peer, quint32 replyToMsgId, const QString &message, quint64 randomId, const TLReplyMarkup &replyMarkup, const TLVector<TLMessageEntity> &entities)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendMessage;
    outputStream << flags;
    // (flags & 1 << 1) stands for noWebpage "true" value
    // (flags & 1 << 5) stands for silent "true" value
    // (flags & 1 << 6) stands for background "true" value
    // (flags & 1 << 7) stands for clearDraft "true" value
    outputStream << peer;
    if (flags & 1 << 0) {
        outputStream << replyToMsgId;
    }
    outputStream << message;
    outputStream << randomId;
    if (flags & 1 << 2) {
        outputStream << replyMarkup;
    }
    if (flags & 1 << 3) {
        outputStream << entities;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendMultiMedia(quint32 flags, const TLInputPeer &peer, quint32 replyToMsgId, const TLVector<TLInputSingleMedia> &multiMedia)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendMultiMedia;
    outputStream << flags;
    // (flags & 1 << 5) stands for silent "true" value
    // (flags & 1 << 6) stands for background "true" value
    // (flags & 1 << 7) stands for clearDraft "true" value
    outputStream << peer;
    if (flags & 1 << 0) {
        outputStream << replyToMsgId;
    }
    outputStream << multiMedia;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendScreenshotNotification(const TLInputPeer &peer, quint32 replyToMsgId, quint64 randomId)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendScreenshotNotification;
    outputStream << peer;
    outputStream << replyToMsgId;
    outputStream << randomId;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSendVote(const TLInputPeer &peer, quint32 msgId, const TLVector<QByteArray> &options)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSendVote;
    outputStream << peer;
    outputStream << msgId;
    outputStream << options;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSetBotCallbackAnswer(quint32 flags, quint64 queryId, const QString &message, const QString &url, quint32 cacheTime)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSetBotCallbackAnswer;
    outputStream << flags;
    // (flags & 1 << 1) stands for alert "true" value
    outputStream << queryId;
    if (flags & 1 << 0) {
        outputStream << message;
    }
    if (flags & 1 << 2) {
        outputStream << url;
    }
    outputStream << cacheTime;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSetBotPrecheckoutResults(quint32 flags, quint64 queryId, const QString &error)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSetBotPrecheckoutResults;
    outputStream << flags;
    // (flags & 1 << 1) stands for success "true" value
    outputStream << queryId;
    if (flags & 1 << 0) {
        outputStream << error;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSetBotShippingResults(quint32 flags, quint64 queryId, const QString &error, const TLVector<TLShippingOption> &shippingOptions)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSetBotShippingResults;
    outputStream << flags;
    outputStream << queryId;
    if (flags & 1 << 0) {
        outputStream << error;
    }
    if (flags & 1 << 1) {
        outputStream << shippingOptions;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSetEncryptedTyping(const TLInputEncryptedChat &peer, bool typing)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSetEncryptedTyping;
    outputStream << peer;
    outputStream << typing;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSetGameScore(quint32 flags, const TLInputPeer &peer, quint32 id, const TLInputUser &userId, quint32 score)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSetGameScore;
    outputStream << flags;
    // (flags & 1 << 0) stands for editMessage "true" value
    // (flags & 1 << 1) stands for force "true" value
    outputStream << peer;
    outputStream << id;
    outputStream << userId;
    outputStream << score;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSetInlineBotResults(quint32 flags, quint64 queryId, const TLVector<TLInputBotInlineResult> &results, quint32 cacheTime, const QString &nextOffset, const TLInlineBotSwitchPM &switchPm)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSetInlineBotResults;
    outputStream << flags;
    // (flags & 1 << 0) stands for gallery "true" value
    // (flags & 1 << 1) stands for isPrivate "true" value
    outputStream << queryId;
    outputStream << results;
    outputStream << cacheTime;
    if (flags & 1 << 2) {
        outputStream << nextOffset;
    }
    if (flags & 1 << 3) {
        outputStream << switchPm;
    }
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSetInlineGameScore(quint32 flags, const TLInputBotInlineMessageID &id, const TLInputUser &userId, quint32 score)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSetInlineGameScore;
    outputStream << flags;
    // (flags & 1 << 0) stands for editMessage "true" value
    // (flags & 1 << 1) stands for force "true" value
    outputStream << id;
    outputStream << userId;
    outputStream << score;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesSetTyping(const TLInputPeer &peer, const TLSendMessageAction &action)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesSetTyping;
    outputStream << peer;
    outputStream << action;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesStartBot(const TLInputUser &bot, const TLInputPeer &peer, quint64 randomId, const QString &startParam)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesStartBot;
    outputStream << bot;
    outputStream << peer;
    outputStream << randomId;
    outputStream << startParam;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesToggleChatAdmins(quint32 chatId, bool enabled)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesToggleChatAdmins;
    outputStream << chatId;
    outputStream << enabled;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesToggleDialogPin(quint32 flags, const TLInputDialogPeer &peer)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesToggleDialogPin;
    outputStream << flags;
    // (flags & 1 << 0) stands for pinned "true" value
    outputStream << peer;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesUninstallStickerSet(const TLInputStickerSet &stickerset)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesUninstallStickerSet;
    outputStream << stickerset;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesUpdatePinnedMessage(quint32 flags, const TLInputPeer &peer, quint32 id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesUpdatePinnedMessage;
    outputStream << flags;
    // (flags & 1 << 0) stands for silent "true" value
    outputStream << peer;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesUploadEncryptedFile(const TLInputEncryptedChat &peer, const TLInputEncryptedFile &file)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesUploadEncryptedFile;
    outputStream << peer;
    outputStream << file;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::messagesUploadMedia(const TLInputPeer &peer, const TLInputMedia &media)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::MessagesUploadMedia;
    outputStream << peer;
    outputStream << media;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::updatesGetChannelDifference(quint32 flags, const TLInputChannel &channel, const TLChannelMessagesFilter &filter, quint32 pts, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UpdatesGetChannelDifference;
    outputStream << flags;
    // (flags & 1 << 0) stands for force "true" value
    outputStream << channel;
    outputStream << filter;
    outputStream << pts;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::updatesGetDifference(quint32 flags, quint32 pts, quint32 ptsTotalLimit, quint32 date, quint32 qts)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UpdatesGetDifference;
    outputStream << flags;
    outputStream << pts;
    if (flags & 1 << 0) {
        outputStream << ptsTotalLimit;
    }
    outputStream << date;
    outputStream << qts;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::updatesGetState()
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UpdatesGetState;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::uploadGetCdnFile(const QByteArray &fileToken, quint32 offset, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UploadGetCdnFile;
    outputStream << fileToken;
    outputStream << offset;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::uploadGetCdnFileHashes(const QByteArray &fileToken, quint32 offset)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UploadGetCdnFileHashes;
    outputStream << fileToken;
    outputStream << offset;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::uploadGetFile(const TLInputFileLocation &location, quint32 offset, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UploadGetFile;
    outputStream << location;
    outputStream << offset;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::uploadGetFileHashes(const TLInputFileLocation &location, quint32 offset)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UploadGetFileHashes;
    outputStream << location;
    outputStream << offset;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::uploadGetWebFile(const TLInputWebFileLocation &location, quint32 offset, quint32 limit)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UploadGetWebFile;
    outputStream << location;
    outputStream << offset;
    outputStream << limit;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::uploadReuploadCdnFile(const QByteArray &fileToken, const QByteArray &requestToken)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UploadReuploadCdnFile;
    outputStream << fileToken;
    outputStream << requestToken;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::uploadSaveBigFilePart(quint64 fileId, quint32 filePart, quint32 fileTotalParts, const QByteArray &bytes)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UploadSaveBigFilePart;
    outputStream << fileId;
    outputStream << filePart;
    outputStream << fileTotalParts;
    outputStream << bytes;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::uploadSaveFilePart(quint64 fileId, quint32 filePart, const QByteArray &bytes)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UploadSaveFilePart;
    outputStream << fileId;
    outputStream << filePart;
    outputStream << bytes;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::usersGetFullUser(const TLInputUser &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UsersGetFullUser;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::usersGetUsers(const TLVector<TLInputUser> &id)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UsersGetUsers;
    outputStream << id;
    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::usersSetSecureValueErrors(const TLInputUser &id, const TLVector<TLSecureValueError> &errors)
{
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);
    outputStream << TLValue::UsersSetSecureValueErrors;
    outputStream << id;
    outputStream << errors;
    return sendEncryptedPackage(output);
}

// End of generated Telegram API methods implementation

quint64 CTelegramConnection::ping()
{
//    qDebug() << Q_FUNC_INFO;
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);

    outputStream << TLValue::Ping;
    outputStream << ++m_lastSentPingId;

    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::pingDelayDisconnect(quint32 disconnectInSec)
{
//    qDebug() << Q_FUNC_INFO << disconnectInSec;
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);

    outputStream << TLValue::PingDelayDisconnect;
    outputStream << ++m_lastSentPingId;
    outputStream << disconnectInSec;

    return sendEncryptedPackage(output);
}

quint64 CTelegramConnection::acknowledgeMessages(const TLVector<quint64> &idsVector)
{
//    qDebug() << Q_FUNC_INFO << idsVector;

    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);

    outputStream << TLValue::MsgsAck;
    outputStream << idsVector;

    return sendEncryptedPackage(output, /* save package */ false);
}

bool CTelegramConnection::acceptPqAuthorization(const QByteArray &payload)
{
    qDebug() << Q_FUNC_INFO;
    CTelegramStream inputStream(payload);

    TLValue responsePqValue;
    inputStream >> responsePqValue;

    if (responsePqValue != TLValue::ResPQ) {
        qDebug() << "Error: Unexpected operation code";
        return false;
    }

    TLNumber128 clientNonce;
    inputStream >> clientNonce;

    if (clientNonce != m_clientNonce) {
        qDebug() << "Error: Client nonce in incoming package is different from our own.";
        return false;
    }

    TLNumber128 serverNonce;
    inputStream >> serverNonce;
    m_serverNonce = serverNonce;

    QByteArray pq;

    inputStream >> pq;

    if (pq.size() != 8) {
        qDebug() << "Error: PQ should be 8 bytes in length";
        return false;
    }

    m_pq = qFromBigEndian<quint64>(reinterpret_cast<const uchar*>(pq.constData()));

    quint64 div1 = Utils::findDivider(m_pq);

    if (div1 == 1) {
        qDebug() << "Error: Can not solve PQ.";
        return false;
    }
    quint64 div2 = m_pq / div1;

    if (div1 < div2) {
        m_p = div1;
        m_q = div2;
    } else {
        m_p = div2;
        m_q = div1;
    }

    TLVector<quint64> fingerprints;
    inputStream >> fingerprints;
    if (fingerprints.count() != 1) {
        qDebug() << "Error: Unexpected Server RSA Fingersprints vector size:" << fingerprints.size();
        return false;
    }
#ifdef TELEGRAMQT_DEBUG_REVEAL_SECRETS
    qDebug() << Q_FUNC_INFO << "Client nonce:" << clientNonce;
    qDebug() << Q_FUNC_INFO << "Server nonce:" << serverNonce;
    qDebug() << Q_FUNC_INFO << "PQ:" << m_pq;
    qDebug() << Q_FUNC_INFO << "P:" << m_p;
    qDebug() << Q_FUNC_INFO << "Q:" << m_q;
    qDebug() << Q_FUNC_INFO << "Fingerprints:" << fingerprints;
#endif
    for (quint64 serverFingerprint : fingerprints) {
        if (serverFingerprint == m_rsaKey.fingerprint) {
            return true;
        }
    }
    qDebug() << "Error: Server RSA fingersprints" << fingerprints << " do not match to the loaded key" << m_rsaKey.fingerprint;
    return false;
}

void CTelegramConnection::requestDhParameters()
{
    qDebug() << Q_FUNC_INFO;
    Utils::randomBytes(m_newNonce.data, m_newNonce.size());

#ifdef TELEGRAMQT_DEBUG_REVEAL_SECRETS
    qDebug() << Q_FUNC_INFO << "New nonce:" << m_newNonce;
#endif

    QByteArray bigEndianNumber;
    bigEndianNumber.fill(char(0), 8);

    QByteArray encryptedPackage;
    {
        static const int requestedEncryptedPackageLength = 255;
        QByteArray innerData;
        CTelegramStream encryptedStream(&innerData, /* write */ true);

        encryptedStream << TLValue::PQInnerData;

        qToBigEndian(m_pq, (uchar *) bigEndianNumber.data());
        encryptedStream << bigEndianNumber;

        bigEndianNumber.fill(char(0), 4);
        qToBigEndian(m_p, (uchar *) bigEndianNumber.data());
        encryptedStream << bigEndianNumber;

        qToBigEndian(m_q, (uchar *) bigEndianNumber.data());
        encryptedStream << bigEndianNumber;

        encryptedStream << m_clientNonce;
        encryptedStream << m_serverNonce;
        encryptedStream << m_newNonce;

        QByteArray sha = Utils::sha1(innerData);
        QByteArray randomPadding;
        randomPadding.resize(requestedEncryptedPackageLength - (sha.length() + innerData.length()));
        Utils::randomBytes(&randomPadding);

        encryptedPackage = Utils::rsa(sha + innerData + randomPadding, m_rsaKey);
    }

    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);

    outputStream << TLValue::ReqDHParams;
    outputStream << m_clientNonce;
    outputStream << m_serverNonce;

    bigEndianNumber.fill(char(0), 4);
    qToBigEndian(m_p, (uchar *) bigEndianNumber.data());
    outputStream << bigEndianNumber;

    qToBigEndian(m_q, (uchar *) bigEndianNumber.data());
    outputStream << bigEndianNumber;

    outputStream << m_rsaKey.fingerprint;

    outputStream << encryptedPackage;

    sendPlainPackage(output);
    setAuthState(AuthStateDhRequested);
}

bool CTelegramConnection::acceptDhAnswer(const QByteArray &payload)
{
    qDebug() << Q_FUNC_INFO;
    CTelegramStream inputStream(payload);

    TLValue responseTLValue;
    inputStream >> responseTLValue;

    if (!checkClientServerNonse(inputStream)) {
        return false;
    }

    switch (responseTLValue) {
    case TLValue::ServerDHParamsOk: {
        QByteArray encryptedAnswer;
        inputStream >> encryptedAnswer;
        return processServerDHParamsOK(encryptedAnswer);
    }
    default:
        qDebug() << "Error: Server did not accept our DH params.";
    }
    return false;
}

bool CTelegramConnection::processServerDHParamsOK(const QByteArray &encryptedAnswer)
{
    m_tmpAesKey = generateTmpAesKey();

    const QByteArray answerWithHash = Utils::aesDecrypt(encryptedAnswer, m_tmpAesKey);
    const QByteArray sha1OfAnswer = answerWithHash.mid(0, 20);
    const QByteArray answer = answerWithHash.mid(20, 564);

    if (Utils::sha1(answer) != sha1OfAnswer) {
        qDebug() << "Error: SHA1 of encrypted answer is different from announced.";
        return false;
    }

    CTelegramStream encryptedInputStream(answer);

    TLValue responseTLValue;
    encryptedInputStream >> responseTLValue;

    if (responseTLValue != TLValue::ServerDHInnerData) {
        qDebug() << "Error: Unexpected TL Value in encrypted answer.";
        return false;
    }

    if (!checkClientServerNonse(encryptedInputStream)) {
        return false;
    }

    encryptedInputStream >> m_g;
    encryptedInputStream >> m_dhPrime;
    encryptedInputStream >> m_gA;

    if ((m_g < 2) || (m_g > 7)) {
        qDebug() << "Error: Received 'g' number is out of acceptable range [2-7].";
        return false;
    }

    if (m_dhPrime.length() != 2048 / 8) {
        qDebug() << "Error: Received dhPrime number length is not correct." << m_dhPrime.length() << 2048 / 8;
        return false;
    }

    if (!(m_dhPrime.at(0) & 128)) {
        qDebug() << "Error: Received dhPrime is too small.";
        return false;
    }

    quint32 serverTime;
    encryptedInputStream >> serverTime;
    setDeltaTime(qint64(serverTime) - (QDateTime::currentMSecsSinceEpoch() / 1000));
    m_deltaTimeHeuristicState = DeltaTimeIsOk;
    return true;
}

void CTelegramConnection::generateDh()
{
    qDebug() << Q_FUNC_INFO;
    // #6 Client computes random 2048-bit number b (using a sufficient amount of entropy) and sends the server a message
    m_b.resize(256);
    Utils::randomBytes(&m_b);

    // IMPORTANT: Apart from the conditions on the Diffie-Hellman prime dh_prime and generator g,
    // both sides are to check that g, g_a and g_b are greater than 1 and less than dh_prime - 1.
    // We recommend checking that g_a and g_b are between 2^{2048-64} and dh_prime - 2^{2048-64} as well.

#ifdef TELEGRAMQT_DEBUG_REVEAL_SECRETS
    qDebug() << "m_b" << m_b;
#endif
}

void CTelegramConnection::requestDhGenerationResult()
{
    qDebug() << Q_FUNC_INFO;
    QByteArray output;
    CTelegramStream outputStream(&output, /* write */ true);

    outputStream << TLValue::SetClientDHParams;
    outputStream << m_clientNonce;
    outputStream << m_serverNonce;

    QByteArray encryptedPackage;
    {
        QByteArray innerData;
        CTelegramStream encryptedStream(&innerData, /* write */ true);

        encryptedStream << TLValue::ClientDHInnerData;

        encryptedStream << m_clientNonce;
        encryptedStream << m_serverNonce;
        encryptedStream << m_authRetryId;

        QByteArray binNumber;
        binNumber.resize(sizeof(m_g));
        qToBigEndian(m_g, (uchar *) binNumber.data());

        binNumber = Utils::binaryNumberModExp(binNumber, m_dhPrime, m_b);

        encryptedStream << binNumber;

        QByteArray sha = Utils::sha1(innerData);
        QByteArray randomPadding;

        int packageLength = sha.length() + innerData.length();
        if ((packageLength) % 16) {
            randomPadding.resize(16 - (packageLength % 16));
            Utils::randomBytes(&randomPadding);

            packageLength += randomPadding.size();
        }

        encryptedPackage = Utils::aesEncrypt(sha + innerData + randomPadding, m_tmpAesKey);
        encryptedPackage.truncate(packageLength);
    }

    outputStream << encryptedPackage;

    sendPlainPackage(output);
    setAuthState(AuthStateDhGenerationResultRequested);
}

bool CTelegramConnection::processServerDhAnswer(const QByteArray &payload)
{
    CTelegramStream inputStream(payload);
    TLValue responseTLValue;
    inputStream >> responseTLValue;
    qDebug() << Q_FUNC_INFO << responseTLValue;

    if (!checkClientServerNonse(inputStream)) {
        return false;
    }

    TLNumber128 newNonceHashLower128;
    inputStream >> newNonceHashLower128;
    const QByteArray readedHashPart(newNonceHashLower128.data, newNonceHashLower128.size());
    const QByteArray newAuthKey = Utils::binaryNumberModExp(m_gA, m_dhPrime, m_b);
    const QByteArray newAuthKeySha = Utils::sha1(newAuthKey);
    QByteArray expectedHashData(m_newNonce.data, m_newNonce.size());
    expectedHashData.append(newAuthKeySha.left(8));
    if (responseTLValue == TLValue::DhGenOk) {
        qDebug() << Q_FUNC_INFO << "Answer OK";
        expectedHashData.insert(32, char(1));
    } else if (responseTLValue == TLValue::DhGenRetry) {
        qDebug() << Q_FUNC_INFO << "Answer RETRY";
        expectedHashData.insert(32, char(2));
    } else if (responseTLValue == TLValue::DhGenFail) {
        qDebug() << Q_FUNC_INFO << "Answer FAIL";
        expectedHashData.insert(32, char(3));
    } else {
        qDebug() << "Error: Unexpected server response.";
        return false;
    }
    if (Utils::sha1(expectedHashData).mid(4) != readedHashPart) {
        qDebug() << "Error: Server (newNonce + auth key) hash is not correct.";
        return false;
    }
    if (responseTLValue == TLValue::DhGenOk) {
        qDebug() << "Server DH answer is accepted. Setup the auth key...";
        setAuthKey(newAuthKey);
        m_serverSalt = m_serverNonce.parts[0] ^ m_newNonce.parts[0];
        setAuthState(AuthStateHaveAKey);
    } else {
        qDebug() << "Server DH answer is not accepted. Retry...";
        if (newAuthKey.isEmpty()) {
            m_authRetryId = 0;
        } else {
            m_authRetryId = Utils::getFingerprints(newAuthKey, Utils::Higher64Bits);
        }
        generateDh();
        requestDhGenerationResult();
    }
    return true;
}

void CTelegramConnection::processRedirectedPackage(const QByteArray &data)
{
    sendEncryptedPackage(data);
}

TLValue CTelegramConnection::processRpcQuery(const QByteArray &data)
{
    CTelegramStream stream(data);

    bool isUpdate;
    TLValue value = processUpdate(stream, &isUpdate, /* requestId */ 0); // Doubtfully that this approach will work in next time.

    if (isUpdate) {
        return value;
    }

    switch (value) {
    case TLValue::NewSessionCreated:
        processSessionCreated(stream);
        break;
    case TLValue::MsgContainer:
        processContainer(stream);
        break;
    case TLValue::RpcResult:
        processRpcResult(stream);
        break;
    case TLValue::MsgsAck:
        processMessageAck(stream);
        break;
    case TLValue::BadMsgNotification:
    case TLValue::BadServerSalt:
        processIgnoredMessageNotification(stream);
        break;
    case TLValue::GzipPacked:
        processGzipPackedRpcQuery(stream);
        break;
    case TLValue::Pong:
        processPingPong(stream);
        break;
    default:
        qDebug() << Q_FUNC_INFO << "value:" << value;
        break;
    }

    if ((value != TLValue::BadMsgNotification) && (m_deltaTimeHeuristicState != DeltaTimeIsOk)) {
        // If we have no bad message notification, then time is synced.
        m_deltaTimeHeuristicState = DeltaTimeIsOk;
    }

    if (stream.error()) {
        qWarning() << Q_FUNC_INFO << "Read of RPC result caused error. RPC type:" << value << "(read from the package -> can be misleading)";
    }

    return value;
}

void CTelegramConnection::processSessionCreated(CTelegramStream &stream)
{
    // https://core.telegram.org/mtproto/service_messages#new-session-creation-notification
    quint64 n;

    stream >> n;
    stream >> n;
    stream >> n;
}

void CTelegramConnection::processContainer(CTelegramStream &stream)
{
    // https://core.telegram.org/mtproto/service_messages#simple-container
    quint32 itemsCount;

    stream >> itemsCount;

    for (quint32 i = 0; i < itemsCount; ++i) {
        quint64 id;
        stream >> id;
        //todo: ack

        quint32 seqNo;
        stream >> seqNo;

        quint32 size;

        stream >> size;

        processRpcQuery(stream.readBytes(size));
    }
}

void CTelegramConnection::processRpcResult(CTelegramStream &stream, quint64 idHint)
{
    quint64 id = idHint;

    if (!id) {
        stream >> id;
    }

    RpcProcessingContext context(stream, id, m_submittedPackages.value(id));
    if (context.hasRequestData()) {
        if (!context.requestType().isValid()) {
            qWarning() << Q_FUNC_INFO << "Invalid request type from the saved package. Package with id" << id << "ignored.";
            return;
        }

        switch (context.requestType()) {
        // Generated RPC processing switch cases
        case TLValue::AccountAcceptAuthorization:
            processAccountAcceptAuthorization(&context);
            break;
        case TLValue::AccountCancelPasswordEmail:
            processAccountCancelPasswordEmail(&context);
            break;
        case TLValue::AccountChangePhone:
            processAccountChangePhone(&context);
            break;
        case TLValue::AccountCheckUsername:
            processAccountCheckUsername(&context);
            break;
        case TLValue::AccountConfirmPasswordEmail:
            processAccountConfirmPasswordEmail(&context);
            break;
        case TLValue::AccountConfirmPhone:
            processAccountConfirmPhone(&context);
            break;
        case TLValue::AccountDeleteAccount:
            processAccountDeleteAccount(&context);
            break;
        case TLValue::AccountDeleteSecureValue:
            processAccountDeleteSecureValue(&context);
            break;
        case TLValue::AccountFinishTakeoutSession:
            processAccountFinishTakeoutSession(&context);
            break;
        case TLValue::AccountGetAccountTTL:
            processAccountGetAccountTTL(&context);
            break;
        case TLValue::AccountGetAllSecureValues:
            processAccountGetAllSecureValues(&context);
            break;
        case TLValue::AccountGetAuthorizationForm:
            processAccountGetAuthorizationForm(&context);
            break;
        case TLValue::AccountGetAuthorizations:
            processAccountGetAuthorizations(&context);
            break;
        case TLValue::AccountGetContactSignUpNotification:
            processAccountGetContactSignUpNotification(&context);
            break;
        case TLValue::AccountGetNotifySettings:
            processAccountGetNotifySettings(&context);
            break;
        case TLValue::AccountGetPassword:
            processAccountGetPassword(&context);
            break;
        case TLValue::AccountGetPasswordSettings:
            processAccountGetPasswordSettings(&context);
            break;
        case TLValue::AccountGetPrivacy:
            processAccountGetPrivacy(&context);
            break;
        case TLValue::AccountGetSecureValue:
            processAccountGetSecureValue(&context);
            break;
        case TLValue::AccountGetTmpPassword:
            processAccountGetTmpPassword(&context);
            break;
        case TLValue::AccountGetWallPapers:
            processAccountGetWallPapers(&context);
            break;
        case TLValue::AccountGetWebAuthorizations:
            processAccountGetWebAuthorizations(&context);
            break;
        case TLValue::AccountInitTakeoutSession:
            processAccountInitTakeoutSession(&context);
            break;
        case TLValue::AccountRegisterDevice:
            processAccountRegisterDevice(&context);
            break;
        case TLValue::AccountReportPeer:
            processAccountReportPeer(&context);
            break;
        case TLValue::AccountResendPasswordEmail:
            processAccountResendPasswordEmail(&context);
            break;
        case TLValue::AccountResetAuthorization:
            processAccountResetAuthorization(&context);
            break;
        case TLValue::AccountResetNotifySettings:
            processAccountResetNotifySettings(&context);
            break;
        case TLValue::AccountResetWebAuthorization:
            processAccountResetWebAuthorization(&context);
            break;
        case TLValue::AccountResetWebAuthorizations:
            processAccountResetWebAuthorizations(&context);
            break;
        case TLValue::AccountSaveSecureValue:
            processAccountSaveSecureValue(&context);
            break;
        case TLValue::AccountSendChangePhoneCode:
            processAccountSendChangePhoneCode(&context);
            break;
        case TLValue::AccountSendConfirmPhoneCode:
            processAccountSendConfirmPhoneCode(&context);
            break;
        case TLValue::AccountSendVerifyEmailCode:
            processAccountSendVerifyEmailCode(&context);
            break;
        case TLValue::AccountSendVerifyPhoneCode:
            processAccountSendVerifyPhoneCode(&context);
            break;
        case TLValue::AccountSetAccountTTL:
            processAccountSetAccountTTL(&context);
            break;
        case TLValue::AccountSetContactSignUpNotification:
            processAccountSetContactSignUpNotification(&context);
            break;
        case TLValue::AccountSetPrivacy:
            processAccountSetPrivacy(&context);
            break;
        case TLValue::AccountUnregisterDevice:
            processAccountUnregisterDevice(&context);
            break;
        case TLValue::AccountUpdateDeviceLocked:
            processAccountUpdateDeviceLocked(&context);
            break;
        case TLValue::AccountUpdateNotifySettings:
            processAccountUpdateNotifySettings(&context);
            break;
        case TLValue::AccountUpdatePasswordSettings:
            processAccountUpdatePasswordSettings(&context);
            break;
        case TLValue::AccountUpdateProfile:
            processAccountUpdateProfile(&context);
            break;
        case TLValue::AccountUpdateStatus:
            processAccountUpdateStatus(&context);
            break;
        case TLValue::AccountUpdateUsername:
            processAccountUpdateUsername(&context);
            break;
        case TLValue::AccountVerifyEmail:
            processAccountVerifyEmail(&context);
            break;
        case TLValue::AccountVerifyPhone:
            processAccountVerifyPhone(&context);
            break;
        case TLValue::AuthBindTempAuthKey:
            processAuthBindTempAuthKey(&context);
            break;
        case TLValue::AuthCancelCode:
            processAuthCancelCode(&context);
            break;
        case TLValue::AuthCheckPassword:
            processAuthCheckPassword(&context);
            break;
        case TLValue::AuthDropTempAuthKeys:
            processAuthDropTempAuthKeys(&context);
            break;
        case TLValue::AuthExportAuthorization:
            processAuthExportAuthorization(&context);
            break;
        case TLValue::AuthImportAuthorization:
            processAuthImportAuthorization(&context);
            break;
        case TLValue::AuthImportBotAuthorization:
            processAuthImportBotAuthorization(&context);
            break;
        case TLValue::AuthLogOut:
            processAuthLogOut(&context);
            break;
        case TLValue::AuthRecoverPassword:
            processAuthRecoverPassword(&context);
            break;
        case TLValue::AuthRequestPasswordRecovery:
            processAuthRequestPasswordRecovery(&context);
            break;
        case TLValue::AuthResendCode:
            processAuthResendCode(&context);
            break;
        case TLValue::AuthResetAuthorizations:
            processAuthResetAuthorizations(&context);
            break;
        case TLValue::AuthSendCode:
            processAuthSendCode(&context);
            break;
        case TLValue::AuthSignIn:
            processAuthSignIn(&context);
            break;
        case TLValue::AuthSignUp:
            processAuthSignUp(&context);
            break;
        case TLValue::ChannelsCheckUsername:
            processChannelsCheckUsername(&context);
            break;
        case TLValue::ChannelsDeleteHistory:
            processChannelsDeleteHistory(&context);
            break;
        case TLValue::ChannelsDeleteMessages:
            processChannelsDeleteMessages(&context);
            break;
        case TLValue::ChannelsDeleteUserHistory:
            processChannelsDeleteUserHistory(&context);
            break;
        case TLValue::ChannelsEditAbout:
            processChannelsEditAbout(&context);
            break;
        case TLValue::ChannelsExportInvite:
            processChannelsExportInvite(&context);
            break;
        case TLValue::ChannelsExportMessageLink:
            processChannelsExportMessageLink(&context);
            break;
        case TLValue::ChannelsGetAdminLog:
            processChannelsGetAdminLog(&context);
            break;
        case TLValue::ChannelsGetAdminedPublicChannels:
            processChannelsGetAdminedPublicChannels(&context);
            break;
        case TLValue::ChannelsGetChannels:
            processChannelsGetChannels(&context);
            break;
        case TLValue::ChannelsGetFullChannel:
            processChannelsGetFullChannel(&context);
            break;
        case TLValue::ChannelsGetLeftChannels:
            processChannelsGetLeftChannels(&context);
            break;
        case TLValue::ChannelsGetMessages:
            processChannelsGetMessages(&context);
            break;
        case TLValue::ChannelsGetParticipant:
            processChannelsGetParticipant(&context);
            break;
        case TLValue::ChannelsGetParticipants:
            processChannelsGetParticipants(&context);
            break;
        case TLValue::ChannelsReadHistory:
            processChannelsReadHistory(&context);
            break;
        case TLValue::ChannelsReadMessageContents:
            processChannelsReadMessageContents(&context);
            break;
        case TLValue::ChannelsReportSpam:
            processChannelsReportSpam(&context);
            break;
        case TLValue::ChannelsSetStickers:
            processChannelsSetStickers(&context);
            break;
        case TLValue::ChannelsUpdateUsername:
            processChannelsUpdateUsername(&context);
            break;
        case TLValue::ContactsBlock:
            processContactsBlock(&context);
            break;
        case TLValue::ContactsDeleteByPhones:
            processContactsDeleteByPhones(&context);
            break;
        case TLValue::ContactsDeleteContact:
            processContactsDeleteContact(&context);
            break;
        case TLValue::ContactsDeleteContacts:
            processContactsDeleteContacts(&context);
            break;
        case TLValue::ContactsGetBlocked:
            processContactsGetBlocked(&context);
            break;
        case TLValue::ContactsGetContactIDs:
            processContactsGetContactIDs(&context);
            break;
        case TLValue::ContactsGetContacts:
            processContactsGetContacts(&context);
            break;
        case TLValue::ContactsGetSaved:
            processContactsGetSaved(&context);
            break;
        case TLValue::ContactsGetStatuses:
            processContactsGetStatuses(&context);
            break;
        case TLValue::ContactsGetTopPeers:
            processContactsGetTopPeers(&context);
            break;
        case TLValue::ContactsImportContacts:
            processContactsImportContacts(&context);
            break;
        case TLValue::ContactsResetSaved:
            processContactsResetSaved(&context);
            break;
        case TLValue::ContactsResetTopPeerRating:
            processContactsResetTopPeerRating(&context);
            break;
        case TLValue::ContactsResolveUsername:
            processContactsResolveUsername(&context);
            break;
        case TLValue::ContactsSearch:
            processContactsSearch(&context);
            break;
        case TLValue::ContactsToggleTopPeers:
            processContactsToggleTopPeers(&context);
            break;
        case TLValue::ContactsUnblock:
            processContactsUnblock(&context);
            break;
        case TLValue::HelpAcceptTermsOfService:
            processHelpAcceptTermsOfService(&context);
            break;
        case TLValue::HelpEditUserInfo:
            processHelpEditUserInfo(&context);
            break;
        case TLValue::HelpGetAppConfig:
            processHelpGetAppConfig(&context);
            break;
        case TLValue::HelpGetAppUpdate:
            processHelpGetAppUpdate(&context);
            break;
        case TLValue::HelpGetCdnConfig:
            processHelpGetCdnConfig(&context);
            break;
        case TLValue::HelpGetConfig:
            processHelpGetConfig(&context);
            break;
        case TLValue::HelpGetDeepLinkInfo:
            processHelpGetDeepLinkInfo(&context);
            break;
        case TLValue::HelpGetInviteText:
            processHelpGetInviteText(&context);
            break;
        case TLValue::HelpGetNearestDc:
            processHelpGetNearestDc(&context);
            break;
        case TLValue::HelpGetPassportConfig:
            processHelpGetPassportConfig(&context);
            break;
        case TLValue::HelpGetProxyData:
            processHelpGetProxyData(&context);
            break;
        case TLValue::HelpGetRecentMeUrls:
            processHelpGetRecentMeUrls(&context);
            break;
        case TLValue::HelpGetSupport:
            processHelpGetSupport(&context);
            break;
        case TLValue::HelpGetSupportName:
            processHelpGetSupportName(&context);
            break;
        case TLValue::HelpGetTermsOfServiceUpdate:
            processHelpGetTermsOfServiceUpdate(&context);
            break;
        case TLValue::HelpGetUserInfo:
            processHelpGetUserInfo(&context);
            break;
        case TLValue::HelpSaveAppLog:
            processHelpSaveAppLog(&context);
            break;
        case TLValue::HelpSetBotUpdatesStatus:
            processHelpSetBotUpdatesStatus(&context);
            break;
        case TLValue::MessagesAcceptEncryption:
            processMessagesAcceptEncryption(&context);
            break;
        case TLValue::MessagesCheckChatInvite:
            processMessagesCheckChatInvite(&context);
            break;
        case TLValue::MessagesClearAllDrafts:
            processMessagesClearAllDrafts(&context);
            break;
        case TLValue::MessagesClearRecentStickers:
            processMessagesClearRecentStickers(&context);
            break;
        case TLValue::MessagesDeleteHistory:
            processMessagesDeleteHistory(&context);
            break;
        case TLValue::MessagesDeleteMessages:
            processMessagesDeleteMessages(&context);
            break;
        case TLValue::MessagesDiscardEncryption:
            processMessagesDiscardEncryption(&context);
            break;
        case TLValue::MessagesEditChatAdmin:
            processMessagesEditChatAdmin(&context);
            break;
        case TLValue::MessagesEditInlineBotMessage:
            processMessagesEditInlineBotMessage(&context);
            break;
        case TLValue::MessagesExportChatInvite:
            processMessagesExportChatInvite(&context);
            break;
        case TLValue::MessagesFaveSticker:
            processMessagesFaveSticker(&context);
            break;
        case TLValue::MessagesGetAllChats:
            processMessagesGetAllChats(&context);
            break;
        case TLValue::MessagesGetAllStickers:
            processMessagesGetAllStickers(&context);
            break;
        case TLValue::MessagesGetArchivedStickers:
            processMessagesGetArchivedStickers(&context);
            break;
        case TLValue::MessagesGetAttachedStickers:
            processMessagesGetAttachedStickers(&context);
            break;
        case TLValue::MessagesGetBotCallbackAnswer:
            processMessagesGetBotCallbackAnswer(&context);
            break;
        case TLValue::MessagesGetChats:
            processMessagesGetChats(&context);
            break;
        case TLValue::MessagesGetCommonChats:
            processMessagesGetCommonChats(&context);
            break;
        case TLValue::MessagesGetDhConfig:
            processMessagesGetDhConfig(&context);
            break;
        case TLValue::MessagesGetDialogUnreadMarks:
            processMessagesGetDialogUnreadMarks(&context);
            break;
        case TLValue::MessagesGetDialogs:
            processMessagesGetDialogs(&context);
            break;
        case TLValue::MessagesGetDocumentByHash:
            processMessagesGetDocumentByHash(&context);
            break;
        case TLValue::MessagesGetFavedStickers:
            processMessagesGetFavedStickers(&context);
            break;
        case TLValue::MessagesGetFeaturedStickers:
            processMessagesGetFeaturedStickers(&context);
            break;
        case TLValue::MessagesGetFullChat:
            processMessagesGetFullChat(&context);
            break;
        case TLValue::MessagesGetGameHighScores:
            processMessagesGetGameHighScores(&context);
            break;
        case TLValue::MessagesGetHistory:
            processMessagesGetHistory(&context);
            break;
        case TLValue::MessagesGetInlineBotResults:
            processMessagesGetInlineBotResults(&context);
            break;
        case TLValue::MessagesGetInlineGameHighScores:
            processMessagesGetInlineGameHighScores(&context);
            break;
        case TLValue::MessagesGetMaskStickers:
            processMessagesGetMaskStickers(&context);
            break;
        case TLValue::MessagesGetMessageEditData:
            processMessagesGetMessageEditData(&context);
            break;
        case TLValue::MessagesGetMessages:
            processMessagesGetMessages(&context);
            break;
        case TLValue::MessagesGetMessagesViews:
            processMessagesGetMessagesViews(&context);
            break;
        case TLValue::MessagesGetOnlines:
            processMessagesGetOnlines(&context);
            break;
        case TLValue::MessagesGetPeerDialogs:
            processMessagesGetPeerDialogs(&context);
            break;
        case TLValue::MessagesGetPeerSettings:
            processMessagesGetPeerSettings(&context);
            break;
        case TLValue::MessagesGetPinnedDialogs:
            processMessagesGetPinnedDialogs(&context);
            break;
        case TLValue::MessagesGetRecentLocations:
            processMessagesGetRecentLocations(&context);
            break;
        case TLValue::MessagesGetRecentStickers:
            processMessagesGetRecentStickers(&context);
            break;
        case TLValue::MessagesGetSavedGifs:
            processMessagesGetSavedGifs(&context);
            break;
        case TLValue::MessagesGetSplitRanges:
            processMessagesGetSplitRanges(&context);
            break;
        case TLValue::MessagesGetStatsURL:
            processMessagesGetStatsURL(&context);
            break;
        case TLValue::MessagesGetStickerSet:
            processMessagesGetStickerSet(&context);
            break;
        case TLValue::MessagesGetStickers:
            processMessagesGetStickers(&context);
            break;
        case TLValue::MessagesGetUnreadMentions:
            processMessagesGetUnreadMentions(&context);
            break;
        case TLValue::MessagesGetWebPage:
            processMessagesGetWebPage(&context);
            break;
        case TLValue::MessagesGetWebPagePreview:
            processMessagesGetWebPagePreview(&context);
            break;
        case TLValue::MessagesHideReportSpam:
            processMessagesHideReportSpam(&context);
            break;
        case TLValue::MessagesInstallStickerSet:
            processMessagesInstallStickerSet(&context);
            break;
        case TLValue::MessagesMarkDialogUnread:
            processMessagesMarkDialogUnread(&context);
            break;
        case TLValue::MessagesReadEncryptedHistory:
            processMessagesReadEncryptedHistory(&context);
            break;
        case TLValue::MessagesReadFeaturedStickers:
            processMessagesReadFeaturedStickers(&context);
            break;
        case TLValue::MessagesReadHistory:
            processMessagesReadHistory(&context);
            break;
        case TLValue::MessagesReadMentions:
            processMessagesReadMentions(&context);
            break;
        case TLValue::MessagesReadMessageContents:
            processMessagesReadMessageContents(&context);
            break;
        case TLValue::MessagesReceivedMessages:
            processMessagesReceivedMessages(&context);
            break;
        case TLValue::MessagesReceivedQueue:
            processMessagesReceivedQueue(&context);
            break;
        case TLValue::MessagesReorderPinnedDialogs:
            processMessagesReorderPinnedDialogs(&context);
            break;
        case TLValue::MessagesReorderStickerSets:
            processMessagesReorderStickerSets(&context);
            break;
        case TLValue::MessagesReport:
            processMessagesReport(&context);
            break;
        case TLValue::MessagesReportEncryptedSpam:
            processMessagesReportEncryptedSpam(&context);
            break;
        case TLValue::MessagesReportSpam:
            processMessagesReportSpam(&context);
            break;
        case TLValue::MessagesRequestEncryption:
            processMessagesRequestEncryption(&context);
            break;
        case TLValue::MessagesSaveDraft:
            processMessagesSaveDraft(&context);
            break;
        case TLValue::MessagesSaveGif:
            processMessagesSaveGif(&context);
            break;
        case TLValue::MessagesSaveRecentSticker:
            processMessagesSaveRecentSticker(&context);
            break;
        case TLValue::MessagesSearch:
            processMessagesSearch(&context);
            break;
        case TLValue::MessagesSearchGifs:
            processMessagesSearchGifs(&context);
            break;
        case TLValue::MessagesSearchGlobal:
            processMessagesSearchGlobal(&context);
            break;
        case TLValue::MessagesSearchStickerSets:
            processMessagesSearchStickerSets(&context);
            break;
        case TLValue::MessagesSendEncrypted:
            processMessagesSendEncrypted(&context);
            break;
        case TLValue::MessagesSendEncryptedFile:
            processMessagesSendEncryptedFile(&context);
            break;
        case TLValue::MessagesSendEncryptedService:
            processMessagesSendEncryptedService(&context);
            break;
        case TLValue::MessagesSetBotCallbackAnswer:
            processMessagesSetBotCallbackAnswer(&context);
            break;
        case TLValue::MessagesSetBotPrecheckoutResults:
            processMessagesSetBotPrecheckoutResults(&context);
            break;
        case TLValue::MessagesSetBotShippingResults:
            processMessagesSetBotShippingResults(&context);
            break;
        case TLValue::MessagesSetEncryptedTyping:
            processMessagesSetEncryptedTyping(&context);
            break;
        case TLValue::MessagesSetInlineBotResults:
            processMessagesSetInlineBotResults(&context);
            break;
        case TLValue::MessagesSetInlineGameScore:
            processMessagesSetInlineGameScore(&context);
            break;
        case TLValue::MessagesSetTyping:
            processMessagesSetTyping(&context);
            break;
        case TLValue::MessagesToggleDialogPin:
            processMessagesToggleDialogPin(&context);
            break;
        case TLValue::MessagesUninstallStickerSet:
            processMessagesUninstallStickerSet(&context);
            break;
        case TLValue::MessagesUploadEncryptedFile:
            processMessagesUploadEncryptedFile(&context);
            break;
        case TLValue::MessagesUploadMedia:
            processMessagesUploadMedia(&context);
            break;
        case TLValue::UpdatesGetChannelDifference:
            processUpdatesGetChannelDifference(&context);
            break;
        case TLValue::UpdatesGetDifference:
            processUpdatesGetDifference(&context);
            break;
        case TLValue::UpdatesGetState:
            processUpdatesGetState(&context);
            break;
        case TLValue::UploadGetCdnFile:
            processUploadGetCdnFile(&context);
            break;
        case TLValue::UploadGetCdnFileHashes:
            processUploadGetCdnFileHashes(&context);
            break;
        case TLValue::UploadGetFile:
            processUploadGetFile(&context);
            break;
        case TLValue::UploadGetFileHashes:
            processUploadGetFileHashes(&context);
            break;
        case TLValue::UploadGetWebFile:
            processUploadGetWebFile(&context);
            break;
        case TLValue::UploadReuploadCdnFile:
            processUploadReuploadCdnFile(&context);
            break;
        case TLValue::UploadSaveBigFilePart:
            processUploadSaveBigFilePart(&context);
            break;
        case TLValue::UploadSaveFilePart:
            processUploadSaveFilePart(&context);
            break;
        case TLValue::UsersGetFullUser:
            processUsersGetFullUser(&context);
            break;
        case TLValue::UsersGetUsers:
            processUsersGetUsers(&context);
            break;
        case TLValue::UsersSetSecureValueErrors:
            processUsersSetSecureValueErrors(&context);
            break;
        // End of generated RPC processing switch cases
        // Generated RPC processing switch updates cases
        case TLValue::AccountGetNotifyExceptions:
        case TLValue::ChannelsCreateChannel:
        case TLValue::ChannelsDeleteChannel:
        case TLValue::ChannelsEditAdmin:
        case TLValue::ChannelsEditBanned:
        case TLValue::ChannelsEditPhoto:
        case TLValue::ChannelsEditTitle:
        case TLValue::ChannelsInviteToChannel:
        case TLValue::ChannelsJoinChannel:
        case TLValue::ChannelsLeaveChannel:
        case TLValue::ChannelsToggleInvites:
        case TLValue::ChannelsTogglePreHistoryHidden:
        case TLValue::ChannelsToggleSignatures:
        case TLValue::HelpGetAppChangelog:
        case TLValue::MessagesAddChatUser:
        case TLValue::MessagesCreateChat:
        case TLValue::MessagesDeleteChatUser:
        case TLValue::MessagesEditChatPhoto:
        case TLValue::MessagesEditChatTitle:
        case TLValue::MessagesEditMessage:
        case TLValue::MessagesForwardMessages:
        case TLValue::MessagesGetAllDrafts:
        case TLValue::MessagesGetPollResults:
        case TLValue::MessagesImportChatInvite:
        case TLValue::MessagesMigrateChat:
        case TLValue::MessagesSendInlineBotResult:
        case TLValue::MessagesSendMedia:
        case TLValue::MessagesSendMessage:
        case TLValue::MessagesSendMultiMedia:
        case TLValue::MessagesSendScreenshotNotification:
        case TLValue::MessagesSendVote:
        case TLValue::MessagesSetGameScore:
        case TLValue::MessagesStartBot:
        case TLValue::MessagesToggleChatAdmins:
        case TLValue::MessagesUpdatePinnedMessage:
        // End of generated RPC processing switch updates cases
        {
            bool ok;
            context.setReadCode(processUpdate(context.inputStream(), &ok, context.requestId()));
        }
            break;
        default:
            qDebug() << "Unknown outgoing RPC type:" << context.requestType();
            break;
        case TLValue::Ping:
            break;
        }

        switch (context.readCode()) {
        case TLValue::RpcError:
            processRpcError(stream, id, context.requestType());
            break;
        case TLValue::GzipPacked:
            processGzipPackedRpcResult(stream, id);
            break;
        default:
            // Any other results considered as success
            m_submittedPackages.remove(id);
            addMessageToAck(id);
            break;
        }
        if (stream.error()) {
            qWarning() << Q_FUNC_INFO << "Read of RPC result caused an error. RPC type:" << context.requestType() << "Package id:" << id;
        }
    } else {
        TLValue request;
        stream >> request;
        qDebug() << "Unexpected RPC message:" << request << "id" << id;
    }
}

void CTelegramConnection::processGzipPackedRpcQuery(CTelegramStream &stream)
{
    QByteArray packedData;
    stream >> packedData;

    const QByteArray data = Utils::unpackGZip(packedData);

    if (!data.isEmpty()) {
        processRpcQuery(data);
    }
}

void CTelegramConnection::processGzipPackedRpcResult(CTelegramStream &stream, quint64 id)
{
    QByteArray packedData;
    stream >> packedData;

    const QByteArray data = Utils::unpackGZip(packedData);

    if (!data.isEmpty()) {
        CTelegramStream unpackedStream(data);
        processRpcResult(unpackedStream, id);
    }
}

bool CTelegramConnection::processRpcError(CTelegramStream &stream, quint64 id, TLValue request)
{
    quint32 errorCode;
    stream >> errorCode;

    QString errorMessage;
    stream >> errorMessage;

    qDebug() << Q_FUNC_INFO << QString(QLatin1String("RPC Error %1: %2 for message %3 %4 (dc %5|%6:%7)"))
                .arg(errorCode).arg(errorMessage).arg(id).arg(request.toString()).arg(m_dcInfo.id).arg(m_dcInfo.ipAddress).arg(m_dcInfo.port);
    bool processed = false;
    switch (errorCode) {
    case 303: // ERROR_SEE_OTHER
        if (processErrorSeeOther(errorMessage, id)) {
            processed = true;
        }
        break;
    case 400: // BAD_REQUEST
#ifdef DEVELOPER_BUILD
        if (m_submittedPackages.contains(id)) {
            const QByteArray data = m_submittedPackages.value(id);
            CTelegramStream outputStream(data);
            dumpRpc(outputStream);
        } else {
            qDebug() << Q_FUNC_INFO << "Submitted package not found";
        }
#endif
        switch (request) {
        case TLValue::AuthCheckPassword:
        case TLValue::AuthCheckPhone:
        case TLValue::AuthSendCode:
        case TLValue::AuthSendCall:
        case TLValue::AuthSignIn:
        case TLValue::AuthSignUp:
            if (errorMessage == QLatin1String("API_ID_INVALID")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorAppIdIsInvalid, errorMessage);
            } else if (errorMessage == QLatin1String("PHONE_NUMBER_INVALID")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorPhoneNumberIsInvalid, errorMessage);
            } else if (errorMessage == QLatin1String("PHONE_NUMBER_OCCUPIED")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorPhoneNumberIsOccupied, errorMessage);
            } else if (errorMessage == QLatin1String("PHONE_NUMBER_UNOCCUPIED")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorPhoneNumberIsUnoccupied, errorMessage);
            } else if (errorMessage == QLatin1String("PHONE_CODE_INVALID")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorPhoneCodeIsInvalid, errorMessage);
            } else if (errorMessage == QLatin1String("PHONE_CODE_EXPIRED")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorPhoneCodeIsExpired, errorMessage);
            } else if (errorMessage == QLatin1String("PASSWORD_HASH_INVALID")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorPasswordHashInvalid, errorMessage);
            } else if (errorMessage == QLatin1String("FIRSTNAME_INVALID")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorFirstNameIsInvalid, errorMessage);
            } else if (errorMessage == QLatin1String("LASTNAME_INVALID")) {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorLastNameIsInvalid, errorMessage);
            } else {
                emit authSignErrorReceived(TelegramNamespace::AuthSignErrorUnknown, errorMessage);
            }
            processed = true;
            break;
        case TLValue::AccountCheckUsername:
        case TLValue::AccountUpdateUsername: {
            const QString userName = userNameFromPackage(id);

            if (errorMessage == QLatin1String("USERNAME_INVALID")) {
                emit userNameStatusUpdated(userName, TelegramNamespace::UserNameStatusIsInvalid);
            } else if (errorMessage == QLatin1String("USERNAME_OCCUPIED")) {
                emit userNameStatusUpdated(userName, TelegramNamespace::UserNameStatusIsOccupied);
            } else if (errorMessage == QLatin1String("USERNAME_NOT_MODIFIED")) {
                emit userNameStatusUpdated(userName, TelegramNamespace::UserNameStatusIsNotModified);
            } else {
                emit userNameStatusUpdated(userName, TelegramNamespace::UserNameStatusUnknown);
            }
        }
            processed = true;
            break;
        case TLValue::MessagesGetChats:
        {
            const QByteArray data = m_submittedPackages.value(id);
            CTelegramStream stream(data);

            TLValue request;
            stream >> request;
            TLVector<quint32> ids;
            stream >> ids;

            if (ids.count() != 1) {
                qDebug() << "Trying to restore by requesting chat one by one";
                foreach (const quint32 id, ids) {
                    messagesGetChats(TLVector<quint32>() << id);
                }
            }
        }
            processed = true;
            break;
        default:
            break;
        }
        break;
    case 401: // UNAUTHORIZED
        if (errorMessage == QLatin1String("AUTH_KEY_UNREGISTERED")) {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedErrorKeyUnregistered, errorMessage);
        } else if (errorMessage == QLatin1String("AUTH_KEY_INVALID")) {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedErrorKeyInvalid, errorMessage);
        } else if (errorMessage == QLatin1String("USER_DEACTIVATED")) {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedErrorUserDeactivated, errorMessage);
        } else if (errorMessage == QLatin1String("SESSION_REVOKED")) {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedErrorUserSessionRevoked, errorMessage);
        } else if (errorMessage == QLatin1String("SESSION_EXPIRED")) {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedErrorUserSessionExpired, errorMessage);
        } else if (errorMessage == QLatin1String("ACTIVE_USER_REQUIRED")) {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedErrorActiveUserRequired, errorMessage);
        } else if (errorMessage == QLatin1String("AUTH_KEY_PERM_EMPTY")) {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedErrorNeedPermanentKey, errorMessage);
        } else if (errorMessage == QLatin1String("SESSION_PASSWORD_NEEDED")) {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedSessionPasswordNeeded, errorMessage);
        } else {
            emit authorizationErrorReceived(TelegramNamespace::UnauthorizedUnknownError, errorMessage);
        }
        processed = true;
        break;
    default:
        qDebug() << "RPC Error can not be handled.";
        break;
    }

    emit errorReceived(errorCode, errorMessage, processed);
    return processed;
}

void CTelegramConnection::processMessageAck(CTelegramStream &stream)
{
    TLVector<quint64> idsVector;

    stream >> idsVector;

    foreach (quint64 id, idsVector) {
        qDebug() << Q_FUNC_INFO << "Package" << id << "acked";
//        m_submittedPackages.remove(id);
    }
}

void CTelegramConnection::processIgnoredMessageNotification(CTelegramStream &stream)
{
    // https://core.telegram.org/mtproto/service_messages_about_messages#notice-of-ignored-error-message
    quint64 id;
    stream >> id;

    quint32 seqNo;
    stream >> seqNo;

    quint32 errorCode;
    stream >> errorCode;

    QString errorText;
    switch (errorCode) {
    case 16:
        errorText = QLatin1String("Id too low");
        break;
    case 17:
        errorText = QLatin1String("Id too high");
        break;
    case 18:
        errorText = QLatin1String("Incorrect two lower order id bits");
        break;
    case 19:
        errorText = QLatin1String("Container id is the same as id of a previously received message");
        break;
    case 20:
        errorText = QLatin1String("Message too old, and it cannot be verified whether the server has received a message with this id or not");
        break;
    case 32:
        errorText = QLatin1String("Sequence number too low");
        break;
    case 33:
        errorText = QLatin1String("Sequence number too high");
        break;
    case 34:
        errorText = QLatin1String("An even sequence number expected");
        break;
    case 35:
        errorText = QLatin1String("Odd sequence number expected");
        break;
    case 48:
        errorText = QLatin1String("Incorrect server salt");
        break;
    case 64:
        errorText = QLatin1String("Invalid container");
        break;
    default:
        errorText = QLatin1String("Unknown error code");
        break;
    }
    qDebug() << QString(QLatin1String("Bad message %1/%2: Code %3 (%4).")).arg(id).arg(seqNo).arg(errorCode).arg(errorText);

    if (errorCode == 16) {
        if (m_deltaTimeHeuristicState == DeltaTimeIsOk) {
            m_deltaTimeHeuristicState = DeltaTimeCorrectionForward;
        }

        if (m_deltaTimeHeuristicState == DeltaTimeCorrectionForward) {
            setDeltaTime(deltaTime() + 1000);
        } else {
            setDeltaTime(deltaTime() + 100);
        }

        sendEncryptedPackageAgain(id);
        qDebug() << "DeltaTime factor increased to" << deltaTime();
    } else if (errorCode == 17) {
        if (m_deltaTimeHeuristicState == DeltaTimeIsOk) {
            m_deltaTimeHeuristicState = DeltaTimeCorrectionBackward;
        }

        if (m_deltaTimeHeuristicState == DeltaTimeCorrectionBackward) {
            setDeltaTime(deltaTime() - 1000);
        } else {
            setDeltaTime(deltaTime() - 100);
        }

        sendEncryptedPackageAgain(id);
        qDebug() << "DeltaTime factor reduced to" << deltaTime();
    } else if (errorCode == 48) {
        m_serverSalt = m_receivedServerSalt;
        qDebug() << "Local serverSalt fixed to" << m_serverSalt;
        sendEncryptedPackageAgain(id);
    }
}

void CTelegramConnection::processPingPong(CTelegramStream &stream)
{
    quint64 pid;
    quint64 msgId;
    stream >> msgId;
    stream >> pid;

    m_lastReceivedPingId = pid;
    m_lastReceivedPingTime = QDateTime::currentMSecsSinceEpoch();

//    qDebug() << Q_FUNC_INFO << m_lastReceivedPingId << m_lastReceivedPingTime;
}

// Partially generated Telegram API RPC process implementation
void CTelegramConnection::processAccountAcceptAuthorization(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountCancelPasswordEmail(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountChangePhone(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLUser result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountCheckUsername(RpcProcessingContext *context)
{
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);

    const QString userName = context->userNameFromRequest();
    switch (result) {
    case TLValue::BoolTrue:
        emit userNameStatusUpdated(userName, TelegramNamespace::UserNameStatusCanBeUsed);
        context->setSucceed(true);
        break;
    case TLValue::BoolFalse:
        emit userNameStatusUpdated(userName, TelegramNamespace::UserNameStatusCanNotBeUsed);
        context->setSucceed(true);
        break;
    default:
        context->setSucceed(false);
        break;
    }
}

void CTelegramConnection::processAccountConfirmPasswordEmail(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountConfirmPhone(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountDeleteAccount(RpcProcessingContext *context)
{
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountDeleteSecureValue(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountFinishTakeoutSession(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountGetAccountTTL(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountDaysTTL result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetAllSecureValues(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLSecureValue> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetAuthorizationForm(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountAuthorizationForm result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetAuthorizations(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountAuthorizations result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetContactSignUpNotification(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountGetNotifySettings(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLPeerNotifySettings result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetPassword(RpcProcessingContext *context)
{
    TLAccountPassword result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit passwordReceived(result, context->requestId());
}

void CTelegramConnection::processAccountGetPasswordSettings(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountPasswordSettings result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetPrivacy(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountPrivacyRules result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetSecureValue(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLSecureValue> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetTmpPassword(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountTmpPassword result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetWallPapers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLWallPaper> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountGetWebAuthorizations(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountWebAuthorizations result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountInitTakeoutSession(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountTakeout result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountRegisterDevice(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountReportPeer(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountResendPasswordEmail(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountResetAuthorization(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountResetNotifySettings(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountResetWebAuthorization(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountResetWebAuthorizations(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountSaveSecureValue(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLSecureValue result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountSendChangePhoneCode(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountSentChangePhoneCode result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountSendConfirmPhoneCode(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAuthSentCode result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountSendVerifyEmailCode(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountSentEmailCode result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountSendVerifyPhoneCode(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAuthSentCode result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountSetAccountTTL(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountSetContactSignUpNotification(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountSetPrivacy(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAccountPrivacyRules result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountUnregisterDevice(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountUpdateDeviceLocked(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountUpdateNotifySettings(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountUpdatePasswordSettings(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountUpdateProfile(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLUser result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAccountUpdateStatus(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountUpdateUsername(RpcProcessingContext *context)
{
    TLUser result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }

    if (result.username == context->userNameFromRequest()) {
        emit userNameStatusUpdated(result.username, TelegramNamespace::UserNameStatusAccepted);
    }
    emit usersReceived(QVector<TLUser>() << result);
}

void CTelegramConnection::processAccountVerifyEmail(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAccountVerifyPhone(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAuthBindTempAuthKey(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAuthCancelCode(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAuthCheckPassword(RpcProcessingContext *context)
{
    processAuthSign(context);
}

void CTelegramConnection::processAuthDropTempAuthKeys(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAuthExportAuthorization(RpcProcessingContext *context)
{
    TLAuthExportedAuthorization result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }

    CTelegramStream stream(context->requestData());
    TLValue value;
    quint32 dc;
    stream >> value;
    stream >> dc;

    emit authExportedAuthorizationReceived(dc, result.id, result.bytes);
}

void CTelegramConnection::processAuthImportAuthorization(RpcProcessingContext *context)
{
    processAuthSign(context);
}

void CTelegramConnection::processAuthImportBotAuthorization(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAuthAuthorization result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAuthLogOut(RpcProcessingContext *context)
{
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);

    if (result == TLValue::BoolTrue) {
        emit loggedOut(true);
    } else if (result == TLValue::BoolFalse) {
        emit loggedOut(false);
    }
}

void CTelegramConnection::processAuthRecoverPassword(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAuthAuthorization result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAuthRequestPasswordRecovery(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAuthPasswordRecovery result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAuthResendCode(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLAuthSentCode result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processAuthResetAuthorizations(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processAuthSendCode(RpcProcessingContext *context)
{
    TLAuthSentCode result;
    context->readRpcResult(&result);

    qDebug() << Q_FUNC_INFO << result.tlType;
    if (result.tlType == TLValue::AuthSentCode) {
        m_authCodeHash = result.phoneCodeHash;

        emit phoneCodeRequired();
    } else if (result.tlType == TLValue::AuthSentAppCode) {
        qDebug() << Q_FUNC_INFO << "AuthSentAppCode";
        m_authCodeHash = result.phoneCodeHash;
        CTelegramStream stream(context->requestData());
        TLValue value;
        QString phoneNumber;
        stream >> value;
        stream >> phoneNumber;
        authSendSms(phoneNumber, m_authCodeHash);
    }
}

void CTelegramConnection::processAuthSignIn(RpcProcessingContext *context)
{
    processAuthSign(context);
}

void CTelegramConnection::processAuthSignUp(RpcProcessingContext *context)
{
    processAuthSign(context);
}

void CTelegramConnection::processChannelsCheckUsername(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processChannelsDeleteHistory(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processChannelsDeleteMessages(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesAffectedMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsDeleteUserHistory(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesAffectedHistory result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsEditAbout(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processChannelsExportInvite(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLExportedChatInvite result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsExportMessageLink(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLExportedMessageLink result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsGetAdminLog(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLChannelsAdminLogResults result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsGetAdminedPublicChannels(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesChats result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsGetChannels(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesChats result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsGetFullChannel(RpcProcessingContext *context)
{
    TLMessagesChatFull result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit messagesFullChatReceived(result.fullChat, result.chats, result.users);
}

void CTelegramConnection::processChannelsGetLeftChannels(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesChats result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsGetMessages(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processChannelsGetParticipant(RpcProcessingContext *context)
{
    TLChannelsChannelParticipant result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    CTelegramStream stream(context->requestData());
    TLValue value;
    TLInputChannel channel;
    stream >> value;
    stream >> channel;

    if (value != TLValue::ChannelsGetParticipant) {
        qWarning() << Q_FUNC_INFO << "Unexpected request data";
        return;
    }

    emit usersReceived(result.users);
    emit channelsParticipantsReceived(channel.channelId, { result.participant });
}

void CTelegramConnection::processChannelsGetParticipants(RpcProcessingContext *context)
{
    TLChannelsChannelParticipants result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    CTelegramStream stream(context->requestData());
    TLValue value;
    TLInputChannel channel;
    stream >> value;
    stream >> channel;

    if (value != TLValue::ChannelsGetParticipants) {
        qWarning() << Q_FUNC_INFO << "Unexpected request data";
        return;
    }

    emit usersReceived(result.users);
    channelsParticipantsReceived(channel.channelId, result.participants);
}

void CTelegramConnection::processChannelsReadHistory(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processChannelsReadMessageContents(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processChannelsReportSpam(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processChannelsSetStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processChannelsUpdateUsername(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processContactsBlock(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processContactsDeleteByPhones(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processContactsDeleteContact(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLContactsLink result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processContactsDeleteContacts(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processContactsGetBlocked(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLContactsBlocked result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processContactsGetContactIDs(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<quint32> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processContactsGetContacts(RpcProcessingContext *context)
{
    TLContactsContacts result;
    context->readRpcResult(&result);

    if (result.tlType == TLValue::ContactsContacts) {
        emit usersReceived(result.users);

        QVector<quint32> contactList;
        foreach (const TLUser &user, result.users) {
            contactList.append(user.id);
        }

        emit contactListReceived(contactList);
    }
}

void CTelegramConnection::processContactsGetSaved(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLSavedContact> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processContactsGetStatuses(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLContactStatus> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processContactsGetTopPeers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLContactsTopPeers result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processContactsImportContacts(RpcProcessingContext *context)
{
    TLContactsImportedContacts result;
    context->readRpcResult(&result);
    if (result.tlType == TLValue::ContactsImportedContacts) {
        emit usersReceived(result.users);

        QVector<quint32> addedList;
        foreach (const TLUser &user, result.users) {
            addedList.append(user.id);
        }

        emit contactListChanged(addedList, { });
    }
}

void CTelegramConnection::processContactsResetSaved(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processContactsResetTopPeerRating(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processContactsResolveUsername(RpcProcessingContext *context)
{
    TLContactsResolvedPeer result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit usersReceived(result.users);
    if (!result.chats.isEmpty()) {
        emit messagesChatsReceived(result.chats);
    }
    const QString userName = context->userNameFromRequest();
    emit userNameStatusUpdated(userName, TelegramNamespace::UserNameStatusResolved);
}

void CTelegramConnection::processContactsSearch(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLContactsFound result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processContactsToggleTopPeers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processContactsUnblock(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processHelpAcceptTermsOfService(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processHelpEditUserInfo(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpUserInfo result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetAppConfig(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLJSONValue result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetAppUpdate(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpAppUpdate result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetCdnConfig(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLCdnConfig result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetConfig(RpcProcessingContext *context)
{
    TLConfig result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }

    m_dcConfiguration = result.dcOptions;

    if (m_dcInfo.id != result.thisDc) {
        const quint32 oldId = m_dcInfo.id;
        m_dcInfo.id = result.thisDc;
        emit actualDcIdReceived(oldId, result.thisDc);
    }

    emit dcConfigurationReceived(m_dcInfo.id);
}

void CTelegramConnection::processHelpGetDeepLinkInfo(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpDeepLinkInfo result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetInviteText(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpInviteText result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetNearestDc(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLNearestDc result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetPassportConfig(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpPassportConfig result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetProxyData(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpProxyData result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetRecentMeUrls(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpRecentMeUrls result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetSupport(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpSupport result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetSupportName(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpSupportName result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetTermsOfServiceUpdate(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpTermsOfServiceUpdate result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpGetUserInfo(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLHelpUserInfo result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processHelpSaveAppLog(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processHelpSetBotUpdatesStatus(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesAcceptEncryption(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLEncryptedChat result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesCheckChatInvite(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLChatInvite result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesClearAllDrafts(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesClearRecentStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesDeleteHistory(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesAffectedHistory result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesDeleteMessages(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesAffectedMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesDiscardEncryption(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesEditChatAdmin(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesEditInlineBotMessage(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesExportChatInvite(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLExportedChatInvite result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesFaveSticker(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesGetAllChats(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesChats result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetAllStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesAllStickers result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetArchivedStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesArchivedStickers result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetAttachedStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLStickerSetCovered> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetBotCallbackAnswer(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesBotCallbackAnswer result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetChats(RpcProcessingContext *context)
{
    TLMessagesChats result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit messagesChatsReceived(result.chats);
}

void CTelegramConnection::processMessagesGetCommonChats(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesChats result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetDhConfig(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesDhConfig result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetDialogUnreadMarks(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLDialogPeer> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetDialogs(RpcProcessingContext *context)
{
    TLMessagesDialogs result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }

    CTelegramStream stream(context->requestData());
    TLValue method;
    quint32 offsetDate = 0;
    quint32 offsetId = 0;
    TLInputPeer offsetPeer;
    quint32 limit = 0;

    stream >> method; // TLValue::MessagesGetDialogs
    stream >> offsetDate;
    stream >> offsetId;
    stream >> offsetPeer;
    stream >> limit;
    if (result.tlType != TLValue::MessagesDialogs) {
        qWarning() << Q_FUNC_INFO << result.tlType << "processed as Dialogs";
    }
    emit messagesDialogsReceived(result, offsetDate, offsetId, offsetPeer, limit);
}

void CTelegramConnection::processMessagesGetDocumentByHash(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLDocument result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetFavedStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesFavedStickers result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetFeaturedStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesFeaturedStickers result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetFullChat(RpcProcessingContext *context)
{
    TLMessagesChatFull result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit messagesFullChatReceived(result.fullChat, result.chats, result.users);
}

void CTelegramConnection::processMessagesGetGameHighScores(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesHighScores result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetHistory(RpcProcessingContext *context)
{
    TLMessagesMessages result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }

    CTelegramStream stream(context->requestData());
    TLValue value;
    TLInputPeer peer;
    stream >> value;
    stream >> peer;

    emit messagesHistoryReceived(result, peer);
}

void CTelegramConnection::processMessagesGetInlineBotResults(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesBotResults result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetInlineGameHighScores(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesHighScores result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetMaskStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesAllStickers result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetMessageEditData(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesMessageEditData result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetMessages(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetMessagesViews(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<quint32> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetOnlines(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLChatOnlines result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetPeerDialogs(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesPeerDialogs result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetPeerSettings(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLPeerSettings result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetPinnedDialogs(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesPeerDialogs result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetRecentLocations(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetRecentStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesRecentStickers result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetSavedGifs(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesSavedGifs result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetSplitRanges(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLMessageRange> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetStatsURL(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLStatsURL result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetStickerSet(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesStickerSet result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesStickers result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetUnreadMentions(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetWebPage(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLWebPage result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesGetWebPagePreview(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessageMedia result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesHideReportSpam(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesInstallStickerSet(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesMarkDialogUnread(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesReadEncryptedHistory(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesReadFeaturedStickers(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesReadHistory(RpcProcessingContext *context)
{
    TLMessagesAffectedMessages result;
    context->readRpcResult(&result);
    emit messagesAffectedMessagesReceived(result);
}

void CTelegramConnection::processMessagesReadMentions(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesAffectedHistory result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesReadMessageContents(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesAffectedMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesReceivedMessages(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLReceivedNotifyMessage> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesReceivedQueue(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<quint64> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesReorderPinnedDialogs(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesReorderStickerSets(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesReport(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesReportEncryptedSpam(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesReportSpam(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesRequestEncryption(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLEncryptedChat result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesSaveDraft(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSaveGif(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSaveRecentSticker(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSearch(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesSearchGifs(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesFoundGifs result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesSearchGlobal(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesMessages result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesSearchStickerSets(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesFoundStickerSets result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesSendEncrypted(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesSentEncryptedMessage result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesSendEncryptedFile(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesSentEncryptedMessage result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesSendEncryptedService(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessagesSentEncryptedMessage result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesSetBotCallbackAnswer(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSetBotPrecheckoutResults(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSetBotShippingResults(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSetEncryptedTyping(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSetInlineBotResults(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSetInlineGameScore(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesSetTyping(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesToggleDialogPin(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesUninstallStickerSet(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processMessagesUploadEncryptedFile(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLEncryptedFile result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processMessagesUploadMedia(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLMessageMedia result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processUpdatesGetChannelDifference(RpcProcessingContext *context)
{
    TLUpdatesChannelDifference result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit updatesChannelDifferenceReceived(result);
}

void CTelegramConnection::processUpdatesGetDifference(RpcProcessingContext *context)
{
    TLUpdatesDifference result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit updatesDifferenceReceived(result);
}

void CTelegramConnection::processUpdatesGetState(RpcProcessingContext *context)
{
    TLUpdatesState result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit updatesStateReceived(result);
}

void CTelegramConnection::processUploadGetCdnFile(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLUploadCdnFile result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processUploadGetCdnFileHashes(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLFileHash> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processUploadGetFile(RpcProcessingContext *context)
{
    TLUploadFile result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    CTelegramStream stream(context->requestData());
    TLValue value;
    TLInputFileLocation location;
    quint32 offset;

    stream >> value;
    stream >> location;
    stream >> offset;

    emit fileDataReceived(result, m_requestedFilesIds.value(context->requestId()), offset);
}

void CTelegramConnection::processUploadGetFileHashes(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLFileHash> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processUploadGetWebFile(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLUploadWebFile result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processUploadReuploadCdnFile(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLVector<TLFileHash> result;
    context->readRpcResult(&result);
}

void CTelegramConnection::processUploadSaveBigFilePart(RpcProcessingContext *context)
{
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

void CTelegramConnection::processUploadSaveFilePart(RpcProcessingContext *context)
{
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
    if (result == TLValue::BoolTrue) {
        emit fileDataSent(m_requestedFilesIds.take(context->requestId()));
    } else {
        qWarning() << "Unhandled UploadSaveFilePart result"; // retry putFile() call?
    }
}

void CTelegramConnection::processUsersGetFullUser(RpcProcessingContext *context)
{
    TLUserFull result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit fullUserReceived(result);
}

void CTelegramConnection::processUsersGetUsers(RpcProcessingContext *context)
{
    TLVector<TLUser> result;
    context->readRpcResult(&result);
    if (!result.isValid()) {
        return;
    }
    emit usersReceived(result);
}

void CTelegramConnection::processUsersSetSecureValueErrors(RpcProcessingContext *context)
{
    qWarning() << Q_FUNC_INFO << "Is not implemented yet";
    TLValue result; // bool
    context->inputStream() >> result;
    context->setReadCode(result);
}

// End of partially generated Telegram API RPC process implementation

void CTelegramConnection::processAuthSign(RpcProcessingContext *context)
{
    TLAuthAuthorization result;
    context->readRpcResult(&result);

    if (result.isValid()) {
        qDebug() << Q_FUNC_INFO << "AuthAuthorization" << Telegram::Utils::maskPhoneNumber(result.user.phone);
        if (!result.user.self()) {
            qCritical() << Q_FUNC_INFO << "The received user is not a self user";
        }
        emit usersReceived({ result.user });
        setAuthState(AuthStateSignedIn);
    }
}

bool CTelegramConnection::processErrorSeeOther(const QString errorMessage, quint64 id)
{
    bool ok;
    const quint16 dc = errorMessage.section(QLatin1Char('_'), -1, -1).toUInt(&ok);
    if (!ok) {
        return false;
    }
    const QByteArray data = m_submittedPackages.take(id);
    if (data.isEmpty()) {
        qDebug() << Q_FUNC_INFO << "Can not restore message" << id;
        return false;
    }

    CTelegramStream stream(data);
    TLValue value;
    stream >> value;

    switch (value) {
    case TLValue::AuthCheckPhone:
    case TLValue::AuthSendCode:
    case TLValue::AuthSendCall:
    case TLValue::AuthSendSms: {
        QString phoneNumber;
        stream >> phoneNumber;
        emit wantedMainDcChanged(dc, phoneNumber);
        emit newRedirectedPackage(data, dc);
    }
        break;
    default:
        emit newRedirectedPackage(data, dc);
        break;
    }

    return true;
}

TLValue CTelegramConnection::processUpdate(CTelegramStream &stream, bool *ok, quint64 id)
{
    TLUpdates updates;
    stream >> updates;

    if (stream.error()) {
        qWarning() << Q_FUNC_INFO << "Read of an update caused an error.";
    }

    switch (updates.tlType) {
    case TLValue::UpdatesTooLong:
    case TLValue::UpdateShortMessage:
    case TLValue::UpdateShortChatMessage:
    case TLValue::UpdateShortSentMessage:
    case TLValue::UpdateShort:
    case TLValue::UpdatesCombined:
    case TLValue::Updates:
        emit updatesReceived(updates, id);
        *ok = true;
        break;
    default:
        *ok = false;
        break;
    }

    return updates.tlType;
}

void CTelegramConnection::onTransportStateChanged()
{
#ifdef NETWORK_LOGGING
    if (!m_logFile) {
        QDir dir;
        dir.mkdir(QLatin1String("network"));

        m_logFile = new QFile(QLatin1String("network/") + m_dcInfo.ipAddress + QLatin1String(".log"));
        m_logFile->open(QIODevice::WriteOnly);
    }

    qDebug() << Q_FUNC_INFO << m_dcInfo.id << m_dcInfo.ipAddress << m_transport->state();
#endif

    switch (m_transport->state()) {
    case QAbstractSocket::ConnectedState:
        startAuthTimer();

        if (m_authKey.isEmpty()) {
            initAuth();
        } else {
            setAuthState(AuthStateSignedIn);
        }

        setStatus(ConnectionStatusConnected, ConnectionStatusReasonRemote);
        break;
    case QAbstractSocket::UnconnectedState:
        setStatus(ConnectionStatusDisconnected, status() == ConnectionStatusDisconnecting ? ConnectionStatusReasonLocal : ConnectionStatusReasonRemote);
        break;
    default:
        break;
    }
}

void CTelegramConnection::onTransportPackageReceived(const QByteArray &input)
{
    qDebug() << "Read" << input.length() << "bytes";

    CRawStream inputStream(input);

    quint64 authId = 0;
    QByteArray payload;
    inputStream >> authId;

    if (!authId) {
        // Plain Message
        quint64 timeStamp = 0;
        inputStream >> timeStamp;
        quint32 length = 0;
        inputStream >> length;

        if (inputStream.bytesAvailable() != int(length)) {
            qDebug() << Q_FUNC_INFO << "Corrupted packet. Specified length does not equal to real length";
            return;
        }

        payload = inputStream.readBytes(length);
#ifdef DEVELOPER_BUILD
        qDebug() << Q_FUNC_INFO << "new plain package in auth state" << m_authState << "payload:" << TLValue::firstFromArray(payload);
#endif

        switch (m_authState) {
        case AuthStatePqRequested:
            if (acceptPqAuthorization(payload)) {
                requestDhParameters();
            }
            break;
        case AuthStateDhRequested:
            if (acceptDhAnswer(payload)) {
                generateDh();
                requestDhGenerationResult();
            }
            break;
        case AuthStateDhGenerationResultRequested:
            if (!processServerDhAnswer(payload)) {
                emit connectionFailed(this);
                disconnectFromDc();
            }
            break;
        default:
            qWarning() << "Unexpected auth state!" << m_authState;
            break;
        }
    } else {
        if (m_authState < AuthStateHaveAKey) {
            qWarning() << Q_FUNC_INFO << "Unexpected message with auth data";
        }

        if (authId != m_authId) {
            qDebug() << Q_FUNC_INFO << "Incorrect auth id.";

#ifdef NETWORK_LOGGING
            QTextStream str(m_logFile);

            str << QDateTime::currentDateTime().toString(QLatin1String("yyyyMMdd HH:mm:ss:zzz")) << QLatin1Char('|');
            str << QLatin1String("pln|");
            str << QString(QLatin1String("size: %1|")).arg(input.length(), 4, 10, QLatin1Char('0'));
            str << QLatin1Char('|');
            str << input.toHex();
            str << endl;
            str.flush();
#endif

            return;
        }
        // Encrypted Message
        const QByteArray messageKey = inputStream.readBytes(16);
        const QByteArray data = inputStream.readBytes(inputStream.bytesAvailable());

        const SAesKey key = generateServerToClientAesKey(messageKey);

        QByteArray decryptedData = Utils::aesDecrypt(data, key).left(data.length());
        CRawStream decryptedStream(decryptedData);

        quint64 sessionId = 0;
        quint64 messageId  = 0;
        quint32 sequence = 0;
        quint32 contentLength = 0;

        decryptedStream >> m_receivedServerSalt;
        decryptedStream >> sessionId;
        decryptedStream >> messageId;
        decryptedStream >> sequence;
        decryptedStream >> contentLength;

        if (m_serverSalt != m_receivedServerSalt) {
            qDebug() << Q_FUNC_INFO << "Received different server salt:" << m_receivedServerSalt << "(remote) vs" << m_serverSalt << "(local)";
//            return;
        }

        if (m_sessionId != sessionId) {
            qDebug() << Q_FUNC_INFO << "Session Id is wrong.";
            return;
        }

        if (int(contentLength) > decryptedData.length()) {
            qDebug() << Q_FUNC_INFO << "Expected data length is more, than actual.";
            return;
        }

        const int headerLength = sizeof(m_receivedServerSalt) + sizeof(sessionId) + sizeof(messageId) + sizeof(sequence) + sizeof(contentLength);
        QByteArray expectedMessageKey = Utils::sha1(decryptedData.left(headerLength + contentLength)).mid(4);

        if (messageKey != expectedMessageKey) {
            qDebug() << Q_FUNC_INFO << "Wrong message key";
            return;
        }

        payload = decryptedStream.readAll();

        processRpcQuery(payload);
    }

#ifdef DEVELOPER_BUILD
    static int packagesCount = 0;
    qDebug() << Q_FUNC_INFO << "Got package" << ++packagesCount << TLValue::firstFromArray(payload);
#endif
}

void CTelegramConnection::onTransportTimeout()
{
    setStatus(ConnectionStatusDisconnected, ConnectionStatusReasonTimeout);
}

void CTelegramConnection::onTimeToPing()
{
//    qDebug() << Q_FUNC_INFO << QDateTime::currentMSecsSinceEpoch();

    if (status() < ConnectionStatusConnected) {
        // Nothing to do.
        return;
    }

    if (m_lastSentPingTime && (m_lastSentPingTime > m_lastReceivedPingTime + m_pingInterval)) {
        qDebug() << Q_FUNC_INFO << "pong time is out";
        setStatus(ConnectionStatusDisconnected, ConnectionStatusReasonTimeout);
        return;
    }

    m_lastSentPingTime = QDateTime::currentMSecsSinceEpoch();

    pingDelayDisconnect(m_pingInterval + m_serverDisconnectionExtraTime); // Server will close the connection after m_serverDisconnectionExtraTime ms more, than our ping interval.
}

void CTelegramConnection::onTimeToAckMessages()
{
    if (m_messagesToAck.isEmpty()) {
        return;
    }

    acknowledgeMessages(m_messagesToAck);
    m_messagesToAck.clear();
}

bool CTelegramConnection::checkClientServerNonse(CTelegramStream &stream) const
{
    TLNumber128 nonce;
    stream >> nonce;
    if (nonce != m_clientNonce) {
        qDebug() << Q_FUNC_INFO << "Error: Client nonce in incoming package is different from our own.";
        return false;
    }

    stream >> nonce;
    if (nonce != m_serverNonce) {
        qDebug() << Q_FUNC_INFO << "Error: Client nonce in incoming package is different from our own.";
        return false;
    }
    return true;
}

SAesKey CTelegramConnection::generateTmpAesKey() const
{
    QByteArray newNonceAndServerNonce;
    newNonceAndServerNonce.append(m_newNonce.data, m_newNonce.size());
    newNonceAndServerNonce.append(m_serverNonce.data, m_serverNonce.size());
    QByteArray serverNonceAndNewNonce;
    serverNonceAndNewNonce.append(m_serverNonce.data, m_serverNonce.size());
    serverNonceAndNewNonce.append(m_newNonce.data, m_newNonce.size());
    QByteArray newNonceAndNewNonce;
    newNonceAndNewNonce.append(m_newNonce.data, m_newNonce.size());
    newNonceAndNewNonce.append(m_newNonce.data, m_newNonce.size());

    const QByteArray key = Utils::sha1(newNonceAndServerNonce) + Utils::sha1(serverNonceAndNewNonce).mid(0, 12);
    const QByteArray iv  = Utils::sha1(serverNonceAndNewNonce).mid(12, 8) + Utils::sha1(newNonceAndNewNonce) + QByteArray(m_newNonce.data, 4);

    return SAesKey(key, iv);
}

SAesKey CTelegramConnection::generateAesKey(const QByteArray &messageKey, int x) const
{
    QByteArray sha1_a = Utils::sha1(messageKey + m_authKey.mid(x, 32));
    QByteArray sha1_b = Utils::sha1(m_authKey.mid(32 + x, 16) + messageKey + m_authKey.mid(48 + x, 16));
    QByteArray sha1_c = Utils::sha1(m_authKey.mid(64 + x, 32) + messageKey);
    QByteArray sha1_d = Utils::sha1(messageKey + m_authKey.mid(96 + x, 32));

    const QByteArray key = sha1_a.mid(0, 8) + sha1_b.mid(8, 12) + sha1_c.mid(4, 12);
    const QByteArray iv  = sha1_a.mid(8, 12) + sha1_b.mid(0, 8) + sha1_c.mid(16, 4) + sha1_d.mid(0, 8);

    return SAesKey(key, iv);
}

void CTelegramConnection::insertInitConnection(QByteArray *data) const
{
#ifdef DEVELOPER_BUILD
    qDebug() << Q_FUNC_INFO << "layer" << TLValue::CurrentLayer;
#endif

    CTelegramStream outputStream(data, /* write */ true);

    outputStream << TLValue::InvokeWithLayer;
    outputStream << TLValue::CurrentLayer;
    outputStream << TLValue::InitConnection;

    outputStream << m_appInfo->appId();
    outputStream << m_appInfo->deviceInfo();
    outputStream << m_appInfo->osInfo();
    outputStream << m_appInfo->appVersion();
    outputStream << m_appInfo->languageCode();
}

quint64 CTelegramConnection::sendPlainPackage(const QByteArray &buffer)
{
    quint64 messageId = newMessageId();

    QByteArray output;
    CRawStream outputStream(&output, /* write */ true);

    outputStream << quint64(0);
    outputStream << messageId;
    outputStream << quint32(buffer.length());
    outputStream << buffer;

    qDebug() << output.size();
    m_transport->sendPackage(output);

#ifdef NETWORK_LOGGING
    CTelegramStream readBack(buffer);
    TLValue val1;
    readBack >> val1;

    QTextStream str(m_logFile);

    str << QDateTime::currentDateTime().toString(QLatin1String("yyyyMMdd HH:mm:ss:zzz")) << QLatin1Char('|');
    str << QLatin1String("pln|");
    str << QString(QLatin1String("size: %1|")).arg(buffer.length(), 4, 10, QLatin1Char('0'));
    str << formatTLValue(val1) << QLatin1Char('|');
    str << buffer.toHex();
    str << endl;
    str.flush();
#endif

    return messageId;
}

quint64 CTelegramConnection::sendEncryptedPackage(const QByteArray &buffer, bool savePackage)
{
    QByteArray encryptedPackage;
    QByteArray messageKey;
    quint64 messageId = newMessageId();
    {
        m_sequenceNumber = m_contentRelatedMessages * 2 + 1;
        ++m_contentRelatedMessages;

        if (savePackage) {
            // Story only content-related messages
            m_submittedPackages.insert(messageId, buffer);
        }

        QByteArray innerData;
        CRawStream stream(&innerData, /* write */ true);

        stream << m_serverSalt;
        stream << m_sessionId;
        stream << messageId;
        stream << m_sequenceNumber;

        QByteArray header;
        if (m_sequenceNumber == 1) {
            insertInitConnection(&header);
        }

        stream << quint32(header.length() + buffer.length());
        stream << header + buffer;

        messageKey = Utils::sha1(innerData).mid(4);
        const SAesKey key = generateClientToServerAesKey(messageKey);

        quint32 packageLength = innerData.length();

        if ((packageLength) % 16) {
            QByteArray randomPadding;
            randomPadding.resize(16 - (packageLength % 16));
            Utils::randomBytes(&randomPadding);

            packageLength += randomPadding.size();

            stream << randomPadding;
        }

        encryptedPackage = Utils::aesEncrypt(innerData, key).left(packageLength);
    }

    qDebug() << this << "sendEncryptedPackage()" << TLValue::firstFromArray(buffer).toString() << "message id:" << messageId << "dc: " << m_dcInfo.id;

    QByteArray output;
    CRawStream outputStream(&output, /* write */ true);

    outputStream << m_authId;
    outputStream << messageKey;
    outputStream << encryptedPackage;

    m_transport->sendPackage(output);

#ifdef NETWORK_LOGGING
    CTelegramStream readBack(buffer);
    TLValue val1;
    readBack >> val1;

    QTextStream str(m_logFile);

    str << QString(QLatin1String("%1|enc|mId%2|seq%3|"))
           .arg(QDateTime::currentDateTime().toString(QLatin1String("yyyyMMdd HH:mm:ss:zzz")))
           .arg(messageId, 10, 10, QLatin1Char('0'))
           .arg(m_sequenceNumber, 4, 10, QLatin1Char('0'));

    str << QString(QLatin1String("size: %1|")).arg(buffer.length(), 4, 10, QLatin1Char('0'));

    str << formatTLValue(val1) << QLatin1Char('|');
    str << buffer.toHex();
    str << endl;
    str.flush();
#endif

    return messageId;
}

quint64 CTelegramConnection::sendEncryptedPackageAgain(quint64 id)
{
    --m_contentRelatedMessages;
    const QByteArray data = m_submittedPackages.take(id);
#ifdef DEVELOPER_BUILD
    qDebug() << Q_FUNC_INFO << id << TLValue::firstFromArray(data);
#endif
    return sendEncryptedPackage(data);
}

void CTelegramConnection::setStatus(ConnectionStatus status, ConnectionStatusReason reason)
{
    if (m_status == status) {
        return;
    }

    m_status = status;
    emit statusChanged(status, reason, m_dcInfo.id);

    if (status < ConnectionStatusConnected) {
        stopPingTimer();
    }
}

void CTelegramConnection::setAuthState(CTelegramConnection::AuthState newState)
{
    if (m_authState == newState)
        return;

    m_authState = newState;

    if ((m_authState >= AuthStateHaveAKey) && !m_sessionId) {
        Utils::randomBytes(&m_sessionId);
    }

    emit authStateChanged(m_authState, m_dcInfo.id);

    if (m_authState >= AuthStateHaveAKey) {
        stopAuthTimer();

        if (m_pingInterval) {
            startPingTimer();
        }
    }
}

quint64 CTelegramConnection::newMessageId()
{
    quint64 newLastMessageId = formatClientTimeStamp(QDateTime::currentMSecsSinceEpoch() + deltaTime() * 1000);
    return m_transport->getNewMessageId(newLastMessageId);
}

QString CTelegramConnection::userNameFromPackage(quint64 id) const
{
    const QByteArray data = m_submittedPackages.value(id);

    if (data.isEmpty()) {
        return QString();
    }

    CTelegramStream outputStream(data);

    TLValue method;

    outputStream >> method;

    switch (method) {
    case TLValue::AccountCheckUsername:
    case TLValue::AccountUpdateUsername:
    case TLValue::ContactsResolveUsername:
        break;
    default:
        return QString();
    }

    QString name;
    outputStream >> name;

    return name;
}

void CTelegramConnection::startAuthTimer()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_authTimer) {
        m_authTimer = new QTimer(this);
        m_authTimer->setInterval(s_defaultAuthInterval);
        m_authTimer->setSingleShot(true);
        connect(m_authTimer, &QTimer::timeout, this, &CTelegramConnection::onTransportTimeout);
    }

    m_authTimer->start();
}

void CTelegramConnection::stopAuthTimer()
{
    if (!m_authTimer) {
        return;
    }
    qDebug() << Q_FUNC_INFO;
    m_authTimer->deleteLater();
    m_authTimer = 0;
}

void CTelegramConnection::startPingTimer()
{
    qDebug() << Q_FUNC_INFO;
    if (!m_pingTimer) {
        m_pingTimer = new QTimer(this);
        m_pingTimer->setSingleShot(false);
        connect(m_pingTimer, &QTimer::timeout, this, &CTelegramConnection::onTimeToPing);
    }

    if (m_pingTimer->interval() != static_cast<int>(m_pingInterval)) {
        m_pingTimer->setInterval(m_pingInterval);
    }

    if (m_pingTimer->isActive()) {
        return;
    }

    m_lastReceivedPingId = 0;
    m_lastSentPingId = 0;
    m_lastReceivedPingTime = 0;
    m_lastSentPingTime = 0;

    m_pingTimer->start();
}

void CTelegramConnection::stopPingTimer()
{
    if (m_pingTimer && m_pingTimer->isActive()) {
        qDebug() << Q_FUNC_INFO;
        m_pingTimer->stop();
    }
}

void CTelegramConnection::addMessageToAck(quint64 id)
{
//    qDebug() << Q_FUNC_INFO << id;
    if (!m_ackTimer->isActive()) {
        m_ackTimer->start();
    }

    m_messagesToAck.append(id);

    if (m_messagesToAck.count() > 6) {
        onTimeToAckMessages();
        m_ackTimer->stop();
    }
}
