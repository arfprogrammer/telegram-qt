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

#ifndef CTELEGRAMCONNECTION_HPP
#define CTELEGRAMCONNECTION_HPP

#include <QObject>
#include <QByteArray>
#include <QVector>
#include <QMap>
#include <QStringList>

#include "TelegramNamespace.hpp"
#include "TLTypes.hpp"
#include "TLNumbers.hpp"
#include "crypto-rsa.hpp"
#include "crypto-aes.hpp"

class CAppInformation;
class CTelegramStream;
class CTelegramTransport;
class RpcProcessingContext;

#ifdef NETWORK_LOGGING
QT_FORWARD_DECLARE_CLASS(QFile)
#endif

QT_FORWARD_DECLARE_CLASS(QTimer)

class CTelegramConnection : public QObject
{
    Q_OBJECT
public:
    enum ConnectionStatus {
        ConnectionStatusDisconnected,
        ConnectionStatusDisconnecting,
        ConnectionStatusConnecting,
        ConnectionStatusConnected,
        ConnectionStatusAuthenticated,
        ConnectionStatusSigned
    };

    enum ConnectionStatusReason {
        ConnectionStatusReasonNone,
        ConnectionStatusReasonLocal,
        ConnectionStatusReasonRemote,
        ConnectionStatusReasonTimeout
    };

    enum AuthState {
        AuthStateNone,
        AuthStatePqRequested,
        AuthStateDhRequested,
        AuthStateDhGenerationResultRequested,
        AuthStateHaveAKey,
        AuthStateSignedIn
    };

    enum DeltaTimeHeuristicState {
        DeltaTimeIsOk,
        DeltaTimeCorrectionForward,
        DeltaTimeCorrectionBackward,
    };

#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
    Q_ENUM(ConnectionStatus)
    Q_ENUM(ConnectionStatusReason)
    Q_ENUM(AuthState)
    Q_ENUM(DeltaTimeHeuristicState)
#endif

    explicit CTelegramConnection(const CAppInformation *appInfo, QObject *parent = nullptr);

    void setDcInfo(const TLDcOption &newDcInfo);
    void setServerRsaKey(const Telegram::RsaKey &key);

    TLDcOption dcInfo() const { return m_dcInfo; }

    void setTransport(CTelegramTransport *newTransport);

public slots:
    void connectToDc();
    void disconnectFromDc();

public:
    ConnectionStatus status() const { return m_status; }

    static quint64 formatClientTimeStamp(qint64 timeInMs);

    void initAuth();
    void setKeepAliveSettings(quint32 interval, quint32 serverDisconnectionExtraTime);

    // Generated Telegram API methods declaration
    quint64 accountAcceptAuthorization(quint32 botId, const QString &scope, const QString &publicKey, const TLVector<TLSecureValueHash> &valueHashes, const TLSecureCredentialsEncrypted &credentials);
    quint64 accountCancelPasswordEmail();
    quint64 accountChangePhone(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode);
    quint64 accountCheckUsername(const QString &username);
    quint64 accountConfirmPasswordEmail(const QString &code);
    quint64 accountConfirmPhone(const QString &phoneCodeHash, const QString &phoneCode);
    quint64 accountDeleteAccount(const QString &reason);
    quint64 accountDeleteSecureValue(const TLVector<TLSecureValueType> &types);
    quint64 accountFinishTakeoutSession(quint32 flags);
    quint64 accountGetAccountTTL();
    quint64 accountGetAllSecureValues();
    quint64 accountGetAuthorizationForm(quint32 botId, const QString &scope, const QString &publicKey);
    quint64 accountGetAuthorizations();
    quint64 accountGetContactSignUpNotification();
    quint64 accountGetNotifyExceptions(quint32 flags, const TLInputNotifyPeer &peer);
    quint64 accountGetNotifySettings(const TLInputNotifyPeer &peer);
    quint64 accountGetPassword();
    quint64 accountGetPasswordSettings(const TLInputCheckPasswordSRP &password);
    quint64 accountGetPrivacy(const TLInputPrivacyKey &key);
    quint64 accountGetSecureValue(const TLVector<TLSecureValueType> &types);
    quint64 accountGetTmpPassword(const TLInputCheckPasswordSRP &password, quint32 period);
    quint64 accountGetWallPapers();
    quint64 accountGetWebAuthorizations();
    quint64 accountInitTakeoutSession(quint32 flags, quint32 fileMaxSize);
    quint64 accountRegisterDevice(quint32 tokenType, const QString &token, bool appSandbox, const QByteArray &secret, const TLVector<quint32> &otherUids);
    quint64 accountReportPeer(const TLInputPeer &peer, const TLReportReason &reason);
    quint64 accountResendPasswordEmail();
    quint64 accountResetAuthorization(quint64 hash);
    quint64 accountResetNotifySettings();
    quint64 accountResetWebAuthorization(quint64 hash);
    quint64 accountResetWebAuthorizations();
    quint64 accountSaveSecureValue(const TLInputSecureValue &value, quint64 secureSecretId);
    quint64 accountSendChangePhoneCode(quint32 flags, const QString &phoneNumber, bool currentNumber);
    quint64 accountSendConfirmPhoneCode(quint32 flags, const QString &hash, bool currentNumber);
    quint64 accountSendVerifyEmailCode(const QString &email);
    quint64 accountSendVerifyPhoneCode(quint32 flags, const QString &phoneNumber, bool currentNumber);
    quint64 accountSetAccountTTL(const TLAccountDaysTTL &ttl);
    quint64 accountSetContactSignUpNotification(bool silent);
    quint64 accountSetPrivacy(const TLInputPrivacyKey &key, const TLVector<TLInputPrivacyRule> &rules);
    quint64 accountUnregisterDevice(quint32 tokenType, const QString &token, const TLVector<quint32> &otherUids);
    quint64 accountUpdateDeviceLocked(quint32 period);
    quint64 accountUpdateNotifySettings(const TLInputNotifyPeer &peer, const TLInputPeerNotifySettings &settings);
    quint64 accountUpdatePasswordSettings(const TLInputCheckPasswordSRP &password, const TLAccountPasswordInputSettings &newSettings);
    quint64 accountUpdateProfile(quint32 flags, const QString &firstName, const QString &lastName, const QString &about);
    quint64 accountUpdateStatus(bool offline);
    quint64 accountUpdateUsername(const QString &username);
    quint64 accountVerifyEmail(const QString &email, const QString &code);
    quint64 accountVerifyPhone(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode);
    quint64 authBindTempAuthKey(quint64 permAuthKeyId, quint64 nonce, quint32 expiresAt, const QByteArray &encryptedMessage);
    quint64 authCancelCode(const QString &phoneNumber, const QString &phoneCodeHash);
    quint64 authCheckPassword(const TLInputCheckPasswordSRP &password);
    quint64 authDropTempAuthKeys(const TLVector<quint64> &exceptAuthKeys);
    quint64 authExportAuthorization(quint32 dcId);
    quint64 authImportAuthorization(quint32 id, const QByteArray &bytes);
    quint64 authImportBotAuthorization(quint32 flags, quint32 apiId, const QString &apiHash, const QString &botAuthToken);
    quint64 authLogOut();
    quint64 authRecoverPassword(const QString &code);
    quint64 authRequestPasswordRecovery();
    quint64 authResendCode(const QString &phoneNumber, const QString &phoneCodeHash);
    quint64 authResetAuthorizations();
    quint64 authSendCode(quint32 flags, const QString &phoneNumber, bool currentNumber, quint32 apiId, const QString &apiHash);
    quint64 authSignIn(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode);
    quint64 authSignUp(const QString &phoneNumber, const QString &phoneCodeHash, const QString &phoneCode, const QString &firstName, const QString &lastName);
    quint64 channelsCheckUsername(const TLInputChannel &channel, const QString &username);
    quint64 channelsCreateChannel(quint32 flags, const QString &title, const QString &about);
    quint64 channelsDeleteChannel(const TLInputChannel &channel);
    quint64 channelsDeleteHistory(const TLInputChannel &channel, quint32 maxId);
    quint64 channelsDeleteMessages(const TLInputChannel &channel, const TLVector<quint32> &id);
    quint64 channelsDeleteUserHistory(const TLInputChannel &channel, const TLInputUser &userId);
    quint64 channelsEditAbout(const TLInputChannel &channel, const QString &about);
    quint64 channelsEditAdmin(const TLInputChannel &channel, const TLInputUser &userId, const TLChannelAdminRights &adminRights);
    quint64 channelsEditBanned(const TLInputChannel &channel, const TLInputUser &userId, const TLChannelBannedRights &bannedRights);
    quint64 channelsEditPhoto(const TLInputChannel &channel, const TLInputChatPhoto &photo);
    quint64 channelsEditTitle(const TLInputChannel &channel, const QString &title);
    quint64 channelsExportInvite(const TLInputChannel &channel);
    quint64 channelsExportMessageLink(const TLInputChannel &channel, quint32 id, bool grouped);
    quint64 channelsGetAdminLog(quint32 flags, const TLInputChannel &channel, const QString &q, const TLChannelAdminLogEventsFilter &eventsFilter, const TLVector<TLInputUser> &admins, quint64 maxId, quint64 minId, quint32 limit);
    quint64 channelsGetAdminedPublicChannels();
    quint64 channelsGetChannels(const TLVector<TLInputChannel> &id);
    quint64 channelsGetFullChannel(const TLInputChannel &channel);
    quint64 channelsGetLeftChannels(quint32 offset);
    quint64 channelsGetMessages(const TLInputChannel &channel, const TLVector<TLInputMessage> &id);
    quint64 channelsGetParticipant(const TLInputChannel &channel, const TLInputUser &userId);
    quint64 channelsGetParticipants(const TLInputChannel &channel, const TLChannelParticipantsFilter &filter, quint32 offset, quint32 limit, quint32 hash);
    quint64 channelsInviteToChannel(const TLInputChannel &channel, const TLVector<TLInputUser> &users);
    quint64 channelsJoinChannel(const TLInputChannel &channel);
    quint64 channelsLeaveChannel(const TLInputChannel &channel);
    quint64 channelsReadHistory(const TLInputChannel &channel, quint32 maxId);
    quint64 channelsReadMessageContents(const TLInputChannel &channel, const TLVector<quint32> &id);
    quint64 channelsReportSpam(const TLInputChannel &channel, const TLInputUser &userId, const TLVector<quint32> &id);
    quint64 channelsSetStickers(const TLInputChannel &channel, const TLInputStickerSet &stickerset);
    quint64 channelsToggleInvites(const TLInputChannel &channel, bool enabled);
    quint64 channelsTogglePreHistoryHidden(const TLInputChannel &channel, bool enabled);
    quint64 channelsToggleSignatures(const TLInputChannel &channel, bool enabled);
    quint64 channelsUpdateUsername(const TLInputChannel &channel, const QString &username);
    quint64 contactsBlock(const TLInputUser &id);
    quint64 contactsDeleteByPhones(const TLVector<QString> &phones);
    quint64 contactsDeleteContact(const TLInputUser &id);
    quint64 contactsDeleteContacts(const TLVector<TLInputUser> &id);
    quint64 contactsGetBlocked(quint32 offset, quint32 limit);
    quint64 contactsGetContactIDs(quint32 hash);
    quint64 contactsGetContacts(quint32 hash);
    quint64 contactsGetSaved();
    quint64 contactsGetStatuses();
    quint64 contactsGetTopPeers(quint32 flags, quint32 offset, quint32 limit, quint32 hash);
    quint64 contactsImportContacts(const TLVector<TLInputContact> &contacts);
    quint64 contactsResetSaved();
    quint64 contactsResetTopPeerRating(const TLTopPeerCategory &category, const TLInputPeer &peer);
    quint64 contactsResolveUsername(const QString &username);
    quint64 contactsSearch(const QString &q, quint32 limit);
    quint64 contactsToggleTopPeers(bool enabled);
    quint64 contactsUnblock(const TLInputUser &id);
    quint64 helpAcceptTermsOfService(const TLDataJSON &id);
    quint64 helpEditUserInfo(const TLInputUser &userId, const QString &message, const TLVector<TLMessageEntity> &entities);
    quint64 helpGetAppChangelog(const QString &prevAppVersion);
    quint64 helpGetAppConfig();
    quint64 helpGetAppUpdate(const QString &source);
    quint64 helpGetCdnConfig();
    quint64 helpGetConfig();
    quint64 helpGetDeepLinkInfo(const QString &path);
    quint64 helpGetInviteText();
    quint64 helpGetNearestDc();
    quint64 helpGetPassportConfig(quint32 hash);
    quint64 helpGetProxyData();
    quint64 helpGetRecentMeUrls(const QString &referer);
    quint64 helpGetSupport();
    quint64 helpGetSupportName();
    quint64 helpGetTermsOfServiceUpdate();
    quint64 helpGetUserInfo(const TLInputUser &userId);
    quint64 helpSaveAppLog(const TLVector<TLInputAppEvent> &events);
    quint64 helpSetBotUpdatesStatus(quint32 pendingUpdatesCount, const QString &message);
    quint64 messagesAcceptEncryption(const TLInputEncryptedChat &peer, const QByteArray &gB, quint64 keyFingerprint);
    quint64 messagesAddChatUser(quint32 chatId, const TLInputUser &userId, quint32 fwdLimit);
    quint64 messagesCheckChatInvite(const QString &hash);
    quint64 messagesClearAllDrafts();
    quint64 messagesClearRecentStickers(quint32 flags);
    quint64 messagesCreateChat(const TLVector<TLInputUser> &users, const QString &title);
    quint64 messagesDeleteChatUser(quint32 chatId, const TLInputUser &userId);
    quint64 messagesDeleteHistory(quint32 flags, const TLInputPeer &peer, quint32 maxId);
    quint64 messagesDeleteMessages(quint32 flags, const TLVector<quint32> &id);
    quint64 messagesDiscardEncryption(quint32 chatId);
    quint64 messagesEditChatAdmin(quint32 chatId, const TLInputUser &userId, bool isAdmin);
    quint64 messagesEditChatPhoto(quint32 chatId, const TLInputChatPhoto &photo);
    quint64 messagesEditChatTitle(quint32 chatId, const QString &title);
    quint64 messagesEditInlineBotMessage(quint32 flags, const TLInputBotInlineMessageID &id, const QString &message, const TLInputMedia &media, const TLReplyMarkup &replyMarkup, const TLVector<TLMessageEntity> &entities);
    quint64 messagesEditMessage(quint32 flags, const TLInputPeer &peer, quint32 id, const QString &message, const TLInputMedia &media, const TLReplyMarkup &replyMarkup, const TLVector<TLMessageEntity> &entities);
    quint64 messagesExportChatInvite(quint32 chatId);
    quint64 messagesFaveSticker(const TLInputDocument &id, bool unfave);
    quint64 messagesForwardMessages(quint32 flags, const TLInputPeer &fromPeer, const TLVector<quint32> &id, const TLVector<quint64> &randomId, const TLInputPeer &toPeer);
    quint64 messagesGetAllChats(const TLVector<quint32> &exceptIds);
    quint64 messagesGetAllDrafts();
    quint64 messagesGetAllStickers(quint32 hash);
    quint64 messagesGetArchivedStickers(quint32 flags, quint64 offsetId, quint32 limit);
    quint64 messagesGetAttachedStickers(const TLInputStickeredMedia &media);
    quint64 messagesGetBotCallbackAnswer(quint32 flags, const TLInputPeer &peer, quint32 msgId, const QByteArray &data);
    quint64 messagesGetChats(const TLVector<quint32> &id);
    quint64 messagesGetCommonChats(const TLInputUser &userId, quint32 maxId, quint32 limit);
    quint64 messagesGetDhConfig(quint32 version, quint32 randomLength);
    quint64 messagesGetDialogUnreadMarks();
    quint64 messagesGetDialogs(quint32 flags, quint32 offsetDate, quint32 offsetId, const TLInputPeer &offsetPeer, quint32 limit, quint32 hash);
    quint64 messagesGetDocumentByHash(const QByteArray &sha256, quint32 size, const QString &mimeType);
    quint64 messagesGetFavedStickers(quint32 hash);
    quint64 messagesGetFeaturedStickers(quint32 hash);
    quint64 messagesGetFullChat(quint32 chatId);
    quint64 messagesGetGameHighScores(const TLInputPeer &peer, quint32 id, const TLInputUser &userId);
    quint64 messagesGetHistory(const TLInputPeer &peer, quint32 offsetId, quint32 offsetDate, quint32 addOffset, quint32 limit, quint32 maxId, quint32 minId, quint32 hash);
    quint64 messagesGetInlineBotResults(quint32 flags, const TLInputUser &bot, const TLInputPeer &peer, const TLInputGeoPoint &geoPoint, const QString &query, const QString &offset);
    quint64 messagesGetInlineGameHighScores(const TLInputBotInlineMessageID &id, const TLInputUser &userId);
    quint64 messagesGetMaskStickers(quint32 hash);
    quint64 messagesGetMessageEditData(const TLInputPeer &peer, quint32 id);
    quint64 messagesGetMessages(const TLVector<TLInputMessage> &id);
    quint64 messagesGetMessagesViews(const TLInputPeer &peer, const TLVector<quint32> &id, bool increment);
    quint64 messagesGetOnlines(const TLInputPeer &peer);
    quint64 messagesGetPeerDialogs(const TLVector<TLInputDialogPeer> &peers);
    quint64 messagesGetPeerSettings(const TLInputPeer &peer);
    quint64 messagesGetPinnedDialogs();
    quint64 messagesGetPollResults(const TLInputPeer &peer, quint32 msgId);
    quint64 messagesGetRecentLocations(const TLInputPeer &peer, quint32 limit, quint32 hash);
    quint64 messagesGetRecentStickers(quint32 flags, quint32 hash);
    quint64 messagesGetSavedGifs(quint32 hash);
    quint64 messagesGetSplitRanges();
    quint64 messagesGetStatsURL(const TLInputPeer &peer);
    quint64 messagesGetStickerSet(const TLInputStickerSet &stickerset);
    quint64 messagesGetStickers(const QString &emoticon, quint32 hash);
    quint64 messagesGetUnreadMentions(const TLInputPeer &peer, quint32 offsetId, quint32 addOffset, quint32 limit, quint32 maxId, quint32 minId);
    quint64 messagesGetWebPage(const QString &url, quint32 hash);
    quint64 messagesGetWebPagePreview(quint32 flags, const QString &message, const TLVector<TLMessageEntity> &entities);
    quint64 messagesHideReportSpam(const TLInputPeer &peer);
    quint64 messagesImportChatInvite(const QString &hash);
    quint64 messagesInstallStickerSet(const TLInputStickerSet &stickerset, bool archived);
    quint64 messagesMarkDialogUnread(quint32 flags, const TLInputDialogPeer &peer);
    quint64 messagesMigrateChat(quint32 chatId);
    quint64 messagesReadEncryptedHistory(const TLInputEncryptedChat &peer, quint32 maxDate);
    quint64 messagesReadFeaturedStickers(const TLVector<quint64> &id);
    quint64 messagesReadHistory(const TLInputPeer &peer, quint32 maxId);
    quint64 messagesReadMentions(const TLInputPeer &peer);
    quint64 messagesReadMessageContents(const TLVector<quint32> &id);
    quint64 messagesReceivedMessages(quint32 maxId);
    quint64 messagesReceivedQueue(quint32 maxQts);
    quint64 messagesReorderPinnedDialogs(quint32 flags, const TLVector<TLInputDialogPeer> &order);
    quint64 messagesReorderStickerSets(quint32 flags, const TLVector<quint64> &order);
    quint64 messagesReport(const TLInputPeer &peer, const TLVector<quint32> &id, const TLReportReason &reason);
    quint64 messagesReportEncryptedSpam(const TLInputEncryptedChat &peer);
    quint64 messagesReportSpam(const TLInputPeer &peer);
    quint64 messagesRequestEncryption(const TLInputUser &userId, quint32 randomId, const QByteArray &gA);
    quint64 messagesSaveDraft(quint32 flags, quint32 replyToMsgId, const TLInputPeer &peer, const QString &message, const TLVector<TLMessageEntity> &entities);
    quint64 messagesSaveGif(const TLInputDocument &id, bool unsave);
    quint64 messagesSaveRecentSticker(quint32 flags, const TLInputDocument &id, bool unsave);
    quint64 messagesSearch(quint32 flags, const TLInputPeer &peer, const QString &q, const TLInputUser &fromId, const TLMessagesFilter &filter, quint32 minDate, quint32 maxDate, quint32 offsetId, quint32 addOffset, quint32 limit, quint32 maxId, quint32 minId, quint32 hash);
    quint64 messagesSearchGifs(const QString &q, quint32 offset);
    quint64 messagesSearchGlobal(const QString &q, quint32 offsetDate, const TLInputPeer &offsetPeer, quint32 offsetId, quint32 limit);
    quint64 messagesSearchStickerSets(quint32 flags, const QString &q, quint32 hash);
    quint64 messagesSendEncrypted(const TLInputEncryptedChat &peer, quint64 randomId, const QByteArray &data);
    quint64 messagesSendEncryptedFile(const TLInputEncryptedChat &peer, quint64 randomId, const QByteArray &data, const TLInputEncryptedFile &file);
    quint64 messagesSendEncryptedService(const TLInputEncryptedChat &peer, quint64 randomId, const QByteArray &data);
    quint64 messagesSendInlineBotResult(quint32 flags, const TLInputPeer &peer, quint32 replyToMsgId, quint64 randomId, quint64 queryId, const QString &id);
    quint64 messagesSendMedia(quint32 flags, const TLInputPeer &peer, quint32 replyToMsgId, const TLInputMedia &media, const QString &message, quint64 randomId, const TLReplyMarkup &replyMarkup, const TLVector<TLMessageEntity> &entities);
    quint64 messagesSendMessage(quint32 flags, const TLInputPeer &peer, quint32 replyToMsgId, const QString &message, quint64 randomId, const TLReplyMarkup &replyMarkup, const TLVector<TLMessageEntity> &entities);
    quint64 messagesSendMultiMedia(quint32 flags, const TLInputPeer &peer, quint32 replyToMsgId, const TLVector<TLInputSingleMedia> &multiMedia);
    quint64 messagesSendScreenshotNotification(const TLInputPeer &peer, quint32 replyToMsgId, quint64 randomId);
    quint64 messagesSendVote(const TLInputPeer &peer, quint32 msgId, const TLVector<QByteArray> &options);
    quint64 messagesSetBotCallbackAnswer(quint32 flags, quint64 queryId, const QString &message, const QString &url, quint32 cacheTime);
    quint64 messagesSetBotPrecheckoutResults(quint32 flags, quint64 queryId, const QString &error);
    quint64 messagesSetBotShippingResults(quint32 flags, quint64 queryId, const QString &error, const TLVector<TLShippingOption> &shippingOptions);
    quint64 messagesSetEncryptedTyping(const TLInputEncryptedChat &peer, bool typing);
    quint64 messagesSetGameScore(quint32 flags, const TLInputPeer &peer, quint32 id, const TLInputUser &userId, quint32 score);
    quint64 messagesSetInlineBotResults(quint32 flags, quint64 queryId, const TLVector<TLInputBotInlineResult> &results, quint32 cacheTime, const QString &nextOffset, const TLInlineBotSwitchPM &switchPm);
    quint64 messagesSetInlineGameScore(quint32 flags, const TLInputBotInlineMessageID &id, const TLInputUser &userId, quint32 score);
    quint64 messagesSetTyping(const TLInputPeer &peer, const TLSendMessageAction &action);
    quint64 messagesStartBot(const TLInputUser &bot, const TLInputPeer &peer, quint64 randomId, const QString &startParam);
    quint64 messagesToggleChatAdmins(quint32 chatId, bool enabled);
    quint64 messagesToggleDialogPin(quint32 flags, const TLInputDialogPeer &peer);
    quint64 messagesUninstallStickerSet(const TLInputStickerSet &stickerset);
    quint64 messagesUpdatePinnedMessage(quint32 flags, const TLInputPeer &peer, quint32 id);
    quint64 messagesUploadEncryptedFile(const TLInputEncryptedChat &peer, const TLInputEncryptedFile &file);
    quint64 messagesUploadMedia(const TLInputPeer &peer, const TLInputMedia &media);
    quint64 updatesGetChannelDifference(quint32 flags, const TLInputChannel &channel, const TLChannelMessagesFilter &filter, quint32 pts, quint32 limit);
    quint64 updatesGetDifference(quint32 flags, quint32 pts, quint32 ptsTotalLimit, quint32 date, quint32 qts);
    quint64 updatesGetState();
    quint64 uploadGetCdnFile(const QByteArray &fileToken, quint32 offset, quint32 limit);
    quint64 uploadGetCdnFileHashes(const QByteArray &fileToken, quint32 offset);
    quint64 uploadGetFile(const TLInputFileLocation &location, quint32 offset, quint32 limit);
    quint64 uploadGetFileHashes(const TLInputFileLocation &location, quint32 offset);
    quint64 uploadGetWebFile(const TLInputWebFileLocation &location, quint32 offset, quint32 limit);
    quint64 uploadReuploadCdnFile(const QByteArray &fileToken, const QByteArray &requestToken);
    quint64 uploadSaveBigFilePart(quint64 fileId, quint32 filePart, quint32 fileTotalParts, const QByteArray &bytes);
    quint64 uploadSaveFilePart(quint64 fileId, quint32 filePart, const QByteArray &bytes);
    quint64 usersGetFullUser(const TLInputUser &id);
    quint64 usersGetUsers(const TLVector<TLInputUser> &id);
    quint64 usersSetSecureValueErrors(const TLInputUser &id, const TLVector<TLSecureValueError> &errors);
    // End of generated Telegram API methods declaration

    quint64 ping();
    quint64 pingDelayDisconnect(quint32 disconnectInSec);

    quint64 acknowledgeMessages(const TLVector<quint64> &idsVector);

    quint64 requestPhoneCode(const QString &phoneNumber);
    quint64 signIn(const QString &phoneNumber, const QString &authCode);
    quint64 signUp(const QString &phoneNumber, const QString &authCode, const QString &firstName, const QString &lastName);

    void downloadFile(const TLInputFileLocation &inputLocation, quint32 offset, quint32 limit, quint32 requestId);
    void uploadFile(quint64 fileId, quint32 filePart, const QByteArray &bytes, quint32 requestId);

    quint64 sendMessage(const TLInputPeer &peer, const QString &message, quint64 randomMessageId);
    quint64 sendMedia(const TLInputPeer &peer, const TLInputMedia &media, quint64 randomMessageId);

    AuthState authState() { return m_authState; }

    void requestPqAuthorization();
    bool acceptPqAuthorization(const QByteArray &payload);
    void requestDhParameters();
    bool acceptDhAnswer(const QByteArray &payload);
    bool processServerDHParamsOK(const QByteArray &encryptedAnswer);
    void generateDh();
    void requestDhGenerationResult();
    bool processServerDhAnswer(const QByteArray &payload);

    TLNumber128 clientNonce() const { return m_clientNonce; }
    TLNumber128 serverNonce() const { return m_serverNonce; }

    quint64 pq() const { return m_pq; }
    quint64 p() const { return m_p; }
    quint64 q() const { return m_q; }

    QByteArray authKey() const { return m_authKey; }
    void setAuthKey(const QByteArray &newAuthKey);
    quint64 authId() const { return m_authId; }

    quint64 serverSalt() const { return m_serverSalt; }
    void setServerSalt(const quint64 salt) { m_serverSalt = salt; }
    quint64 sessionId() const { return m_sessionId; }

    QVector<TLDcOption> dcConfiguration() const { return m_dcConfiguration; }

    qint32 deltaTime() const { return m_deltaTime; }
    void setDeltaTime(const qint32 newDt);

    void processRedirectedPackage(const QByteArray &data);

signals:
    void connectionFailed(CTelegramConnection *connection);
    void wantedMainDcChanged(quint32 dc, QString dcForPhoneNumber);
    void newRedirectedPackage(const QByteArray &data, quint32 dc);

    void statusChanged(ConnectionStatus status, int reason, quint32 dc);
    void authStateChanged(AuthState status, quint32 dc);
    void actualDcIdReceived(quint32 dc, quint32 newDcId);
    void dcConfigurationReceived(quint32 dc);
    void phoneStatusReceived(const QString &phone, bool registered);
    void passwordReceived(const TLAccountPassword &password, quint64 requestId);
    void phoneCodeRequired();
    void loggedOut(bool result);
    void errorReceived(int code, const QString &errorMessage, bool processed);
    void authSignErrorReceived(TelegramNamespace::AuthSignError errorCode, const QString &errorMessage);
    void authorizationErrorReceived(TelegramNamespace::UnauthorizedError errorCode, const QString &errorMessage);
    void userNameStatusUpdated(const QString &userName, TelegramNamespace::UserNameStatus status);
    void usersReceived(const QVector<TLUser> &users);
    void contactListReceived(const QVector<quint32> &contactList);
    void contactListChanged(const QVector<quint32> &added, const QVector<quint32> &removed);
    void fileDataReceived(const TLUploadFile &file, quint32 requestId, quint32 offset);
    void fileDataSent(quint32 requestId);

    void messagesChatsReceived(const QVector<TLChat> &chats);
    void messagesFullChatReceived(const TLChatFull &chat, const QVector<TLChat> &chats, const QVector<TLUser> &users);
    void channelsParticipantsReceived(quint32 channelId, const TLVector<TLChannelParticipant> &participants);

    void fullUserReceived(const TLUserFull &userFull);
    void messagesHistoryReceived(const TLMessagesMessages &history, const TLInputPeer &peer);
    void messagesDialogsReceived(const TLMessagesDialogs &dialogs, quint32 offsetDate, quint32 offsetId, const TLInputPeer &offsetPeer, quint32 limit);
    void channelsDialogsReceived(const TLMessagesDialogs &dialogs, quint32 offsetId, quint32 limit);
    void messagesAffectedMessagesReceived(const TLMessagesAffectedMessages &affectedMessages);

    void updatesReceived(const TLUpdates &update, quint64 id);
    void updatesStateReceived(const TLUpdatesState &updatesState);
    void updatesChannelDifferenceReceived(const TLUpdatesChannelDifference &channelDifference);
    void updatesDifferenceReceived(const TLUpdatesDifference &updatesDifference);

    void authExportedAuthorizationReceived(quint32 dc, quint32 id, const QByteArray &data);

protected:
    TLValue processRpcQuery(const QByteArray &data);

    void processSessionCreated(CTelegramStream &stream);
    void processContainer(CTelegramStream &stream);
    void processRpcResult(CTelegramStream &stream, quint64 idHint = 0);
    void processGzipPackedRpcQuery(CTelegramStream &stream);
    void processGzipPackedRpcResult(CTelegramStream &stream, quint64 id);
    bool processRpcError(CTelegramStream &stream, quint64 id, TLValue request);

    void processMessageAck(CTelegramStream &stream);
    void processIgnoredMessageNotification(CTelegramStream &stream);
    void processPingPong(CTelegramStream &stream);

    // Generated Telegram API RPC process declarations
    void processAccountAcceptAuthorization(RpcProcessingContext *context);
    void processAccountCancelPasswordEmail(RpcProcessingContext *context);
    void processAccountChangePhone(RpcProcessingContext *context);
    void processAccountCheckUsername(RpcProcessingContext *context);
    void processAccountConfirmPasswordEmail(RpcProcessingContext *context);
    void processAccountConfirmPhone(RpcProcessingContext *context);
    void processAccountDeleteAccount(RpcProcessingContext *context);
    void processAccountDeleteSecureValue(RpcProcessingContext *context);
    void processAccountFinishTakeoutSession(RpcProcessingContext *context);
    void processAccountGetAccountTTL(RpcProcessingContext *context);
    void processAccountGetAllSecureValues(RpcProcessingContext *context);
    void processAccountGetAuthorizationForm(RpcProcessingContext *context);
    void processAccountGetAuthorizations(RpcProcessingContext *context);
    void processAccountGetContactSignUpNotification(RpcProcessingContext *context);
    void processAccountGetNotifySettings(RpcProcessingContext *context);
    void processAccountGetPassword(RpcProcessingContext *context);
    void processAccountGetPasswordSettings(RpcProcessingContext *context);
    void processAccountGetPrivacy(RpcProcessingContext *context);
    void processAccountGetSecureValue(RpcProcessingContext *context);
    void processAccountGetTmpPassword(RpcProcessingContext *context);
    void processAccountGetWallPapers(RpcProcessingContext *context);
    void processAccountGetWebAuthorizations(RpcProcessingContext *context);
    void processAccountInitTakeoutSession(RpcProcessingContext *context);
    void processAccountRegisterDevice(RpcProcessingContext *context);
    void processAccountReportPeer(RpcProcessingContext *context);
    void processAccountResendPasswordEmail(RpcProcessingContext *context);
    void processAccountResetAuthorization(RpcProcessingContext *context);
    void processAccountResetNotifySettings(RpcProcessingContext *context);
    void processAccountResetWebAuthorization(RpcProcessingContext *context);
    void processAccountResetWebAuthorizations(RpcProcessingContext *context);
    void processAccountSaveSecureValue(RpcProcessingContext *context);
    void processAccountSendChangePhoneCode(RpcProcessingContext *context);
    void processAccountSendConfirmPhoneCode(RpcProcessingContext *context);
    void processAccountSendVerifyEmailCode(RpcProcessingContext *context);
    void processAccountSendVerifyPhoneCode(RpcProcessingContext *context);
    void processAccountSetAccountTTL(RpcProcessingContext *context);
    void processAccountSetContactSignUpNotification(RpcProcessingContext *context);
    void processAccountSetPrivacy(RpcProcessingContext *context);
    void processAccountUnregisterDevice(RpcProcessingContext *context);
    void processAccountUpdateDeviceLocked(RpcProcessingContext *context);
    void processAccountUpdateNotifySettings(RpcProcessingContext *context);
    void processAccountUpdatePasswordSettings(RpcProcessingContext *context);
    void processAccountUpdateProfile(RpcProcessingContext *context);
    void processAccountUpdateStatus(RpcProcessingContext *context);
    void processAccountUpdateUsername(RpcProcessingContext *context);
    void processAccountVerifyEmail(RpcProcessingContext *context);
    void processAccountVerifyPhone(RpcProcessingContext *context);
    void processAuthBindTempAuthKey(RpcProcessingContext *context);
    void processAuthCancelCode(RpcProcessingContext *context);
    void processAuthCheckPassword(RpcProcessingContext *context);
    void processAuthDropTempAuthKeys(RpcProcessingContext *context);
    void processAuthExportAuthorization(RpcProcessingContext *context);
    void processAuthImportAuthorization(RpcProcessingContext *context);
    void processAuthImportBotAuthorization(RpcProcessingContext *context);
    void processAuthLogOut(RpcProcessingContext *context);
    void processAuthRecoverPassword(RpcProcessingContext *context);
    void processAuthRequestPasswordRecovery(RpcProcessingContext *context);
    void processAuthResendCode(RpcProcessingContext *context);
    void processAuthResetAuthorizations(RpcProcessingContext *context);
    void processAuthSendCode(RpcProcessingContext *context);
    void processAuthSignIn(RpcProcessingContext *context);
    void processAuthSignUp(RpcProcessingContext *context);
    void processChannelsCheckUsername(RpcProcessingContext *context);
    void processChannelsDeleteHistory(RpcProcessingContext *context);
    void processChannelsDeleteMessages(RpcProcessingContext *context);
    void processChannelsDeleteUserHistory(RpcProcessingContext *context);
    void processChannelsEditAbout(RpcProcessingContext *context);
    void processChannelsExportInvite(RpcProcessingContext *context);
    void processChannelsExportMessageLink(RpcProcessingContext *context);
    void processChannelsGetAdminLog(RpcProcessingContext *context);
    void processChannelsGetAdminedPublicChannels(RpcProcessingContext *context);
    void processChannelsGetChannels(RpcProcessingContext *context);
    void processChannelsGetFullChannel(RpcProcessingContext *context);
    void processChannelsGetLeftChannels(RpcProcessingContext *context);
    void processChannelsGetMessages(RpcProcessingContext *context);
    void processChannelsGetParticipant(RpcProcessingContext *context);
    void processChannelsGetParticipants(RpcProcessingContext *context);
    void processChannelsReadHistory(RpcProcessingContext *context);
    void processChannelsReadMessageContents(RpcProcessingContext *context);
    void processChannelsReportSpam(RpcProcessingContext *context);
    void processChannelsSetStickers(RpcProcessingContext *context);
    void processChannelsUpdateUsername(RpcProcessingContext *context);
    void processContactsBlock(RpcProcessingContext *context);
    void processContactsDeleteByPhones(RpcProcessingContext *context);
    void processContactsDeleteContact(RpcProcessingContext *context);
    void processContactsDeleteContacts(RpcProcessingContext *context);
    void processContactsGetBlocked(RpcProcessingContext *context);
    void processContactsGetContactIDs(RpcProcessingContext *context);
    void processContactsGetContacts(RpcProcessingContext *context);
    void processContactsGetSaved(RpcProcessingContext *context);
    void processContactsGetStatuses(RpcProcessingContext *context);
    void processContactsGetTopPeers(RpcProcessingContext *context);
    void processContactsImportContacts(RpcProcessingContext *context);
    void processContactsResetSaved(RpcProcessingContext *context);
    void processContactsResetTopPeerRating(RpcProcessingContext *context);
    void processContactsResolveUsername(RpcProcessingContext *context);
    void processContactsSearch(RpcProcessingContext *context);
    void processContactsToggleTopPeers(RpcProcessingContext *context);
    void processContactsUnblock(RpcProcessingContext *context);
    void processHelpAcceptTermsOfService(RpcProcessingContext *context);
    void processHelpEditUserInfo(RpcProcessingContext *context);
    void processHelpGetAppConfig(RpcProcessingContext *context);
    void processHelpGetAppUpdate(RpcProcessingContext *context);
    void processHelpGetCdnConfig(RpcProcessingContext *context);
    void processHelpGetConfig(RpcProcessingContext *context);
    void processHelpGetDeepLinkInfo(RpcProcessingContext *context);
    void processHelpGetInviteText(RpcProcessingContext *context);
    void processHelpGetNearestDc(RpcProcessingContext *context);
    void processHelpGetPassportConfig(RpcProcessingContext *context);
    void processHelpGetProxyData(RpcProcessingContext *context);
    void processHelpGetRecentMeUrls(RpcProcessingContext *context);
    void processHelpGetSupport(RpcProcessingContext *context);
    void processHelpGetSupportName(RpcProcessingContext *context);
    void processHelpGetTermsOfServiceUpdate(RpcProcessingContext *context);
    void processHelpGetUserInfo(RpcProcessingContext *context);
    void processHelpSaveAppLog(RpcProcessingContext *context);
    void processHelpSetBotUpdatesStatus(RpcProcessingContext *context);
    void processMessagesAcceptEncryption(RpcProcessingContext *context);
    void processMessagesCheckChatInvite(RpcProcessingContext *context);
    void processMessagesClearAllDrafts(RpcProcessingContext *context);
    void processMessagesClearRecentStickers(RpcProcessingContext *context);
    void processMessagesDeleteHistory(RpcProcessingContext *context);
    void processMessagesDeleteMessages(RpcProcessingContext *context);
    void processMessagesDiscardEncryption(RpcProcessingContext *context);
    void processMessagesEditChatAdmin(RpcProcessingContext *context);
    void processMessagesEditInlineBotMessage(RpcProcessingContext *context);
    void processMessagesExportChatInvite(RpcProcessingContext *context);
    void processMessagesFaveSticker(RpcProcessingContext *context);
    void processMessagesGetAllChats(RpcProcessingContext *context);
    void processMessagesGetAllStickers(RpcProcessingContext *context);
    void processMessagesGetArchivedStickers(RpcProcessingContext *context);
    void processMessagesGetAttachedStickers(RpcProcessingContext *context);
    void processMessagesGetBotCallbackAnswer(RpcProcessingContext *context);
    void processMessagesGetChats(RpcProcessingContext *context);
    void processMessagesGetCommonChats(RpcProcessingContext *context);
    void processMessagesGetDhConfig(RpcProcessingContext *context);
    void processMessagesGetDialogUnreadMarks(RpcProcessingContext *context);
    void processMessagesGetDialogs(RpcProcessingContext *context);
    void processMessagesGetDocumentByHash(RpcProcessingContext *context);
    void processMessagesGetFavedStickers(RpcProcessingContext *context);
    void processMessagesGetFeaturedStickers(RpcProcessingContext *context);
    void processMessagesGetFullChat(RpcProcessingContext *context);
    void processMessagesGetGameHighScores(RpcProcessingContext *context);
    void processMessagesGetHistory(RpcProcessingContext *context);
    void processMessagesGetInlineBotResults(RpcProcessingContext *context);
    void processMessagesGetInlineGameHighScores(RpcProcessingContext *context);
    void processMessagesGetMaskStickers(RpcProcessingContext *context);
    void processMessagesGetMessageEditData(RpcProcessingContext *context);
    void processMessagesGetMessages(RpcProcessingContext *context);
    void processMessagesGetMessagesViews(RpcProcessingContext *context);
    void processMessagesGetOnlines(RpcProcessingContext *context);
    void processMessagesGetPeerDialogs(RpcProcessingContext *context);
    void processMessagesGetPeerSettings(RpcProcessingContext *context);
    void processMessagesGetPinnedDialogs(RpcProcessingContext *context);
    void processMessagesGetRecentLocations(RpcProcessingContext *context);
    void processMessagesGetRecentStickers(RpcProcessingContext *context);
    void processMessagesGetSavedGifs(RpcProcessingContext *context);
    void processMessagesGetSplitRanges(RpcProcessingContext *context);
    void processMessagesGetStatsURL(RpcProcessingContext *context);
    void processMessagesGetStickerSet(RpcProcessingContext *context);
    void processMessagesGetStickers(RpcProcessingContext *context);
    void processMessagesGetUnreadMentions(RpcProcessingContext *context);
    void processMessagesGetWebPage(RpcProcessingContext *context);
    void processMessagesGetWebPagePreview(RpcProcessingContext *context);
    void processMessagesHideReportSpam(RpcProcessingContext *context);
    void processMessagesInstallStickerSet(RpcProcessingContext *context);
    void processMessagesMarkDialogUnread(RpcProcessingContext *context);
    void processMessagesReadEncryptedHistory(RpcProcessingContext *context);
    void processMessagesReadFeaturedStickers(RpcProcessingContext *context);
    void processMessagesReadHistory(RpcProcessingContext *context);
    void processMessagesReadMentions(RpcProcessingContext *context);
    void processMessagesReadMessageContents(RpcProcessingContext *context);
    void processMessagesReceivedMessages(RpcProcessingContext *context);
    void processMessagesReceivedQueue(RpcProcessingContext *context);
    void processMessagesReorderPinnedDialogs(RpcProcessingContext *context);
    void processMessagesReorderStickerSets(RpcProcessingContext *context);
    void processMessagesReport(RpcProcessingContext *context);
    void processMessagesReportEncryptedSpam(RpcProcessingContext *context);
    void processMessagesReportSpam(RpcProcessingContext *context);
    void processMessagesRequestEncryption(RpcProcessingContext *context);
    void processMessagesSaveDraft(RpcProcessingContext *context);
    void processMessagesSaveGif(RpcProcessingContext *context);
    void processMessagesSaveRecentSticker(RpcProcessingContext *context);
    void processMessagesSearch(RpcProcessingContext *context);
    void processMessagesSearchGifs(RpcProcessingContext *context);
    void processMessagesSearchGlobal(RpcProcessingContext *context);
    void processMessagesSearchStickerSets(RpcProcessingContext *context);
    void processMessagesSendEncrypted(RpcProcessingContext *context);
    void processMessagesSendEncryptedFile(RpcProcessingContext *context);
    void processMessagesSendEncryptedService(RpcProcessingContext *context);
    void processMessagesSetBotCallbackAnswer(RpcProcessingContext *context);
    void processMessagesSetBotPrecheckoutResults(RpcProcessingContext *context);
    void processMessagesSetBotShippingResults(RpcProcessingContext *context);
    void processMessagesSetEncryptedTyping(RpcProcessingContext *context);
    void processMessagesSetInlineBotResults(RpcProcessingContext *context);
    void processMessagesSetInlineGameScore(RpcProcessingContext *context);
    void processMessagesSetTyping(RpcProcessingContext *context);
    void processMessagesToggleDialogPin(RpcProcessingContext *context);
    void processMessagesUninstallStickerSet(RpcProcessingContext *context);
    void processMessagesUploadEncryptedFile(RpcProcessingContext *context);
    void processMessagesUploadMedia(RpcProcessingContext *context);
    void processUpdatesGetChannelDifference(RpcProcessingContext *context);
    void processUpdatesGetDifference(RpcProcessingContext *context);
    void processUpdatesGetState(RpcProcessingContext *context);
    void processUploadGetCdnFile(RpcProcessingContext *context);
    void processUploadGetCdnFileHashes(RpcProcessingContext *context);
    void processUploadGetFile(RpcProcessingContext *context);
    void processUploadGetFileHashes(RpcProcessingContext *context);
    void processUploadGetWebFile(RpcProcessingContext *context);
    void processUploadReuploadCdnFile(RpcProcessingContext *context);
    void processUploadSaveBigFilePart(RpcProcessingContext *context);
    void processUploadSaveFilePart(RpcProcessingContext *context);
    void processUsersGetFullUser(RpcProcessingContext *context);
    void processUsersGetUsers(RpcProcessingContext *context);
    void processUsersSetSecureValueErrors(RpcProcessingContext *context);
    // End of generated Telegram API RPC process declarations

    void processAuthSign(RpcProcessingContext *context);
    bool processErrorSeeOther(const QString errorMessage, quint64 id);

    TLValue processUpdate(CTelegramStream &stream, bool *ok, quint64 id);

    SAesKey generateTmpAesKey() const;
    SAesKey generateClientToServerAesKey(const QByteArray &messageKey) const;
    SAesKey generateServerToClientAesKey(const QByteArray &messageKey) const;

    SAesKey generateAesKey(const QByteArray &messageKey, int xValue) const;

    void insertInitConnection(QByteArray *data) const;

    quint64 sendPlainPackage(const QByteArray &buffer);
    quint64 sendEncryptedPackage(const QByteArray &buffer, bool savePackage = true);
    quint64 sendEncryptedPackageAgain(quint64 id);

    void setStatus(ConnectionStatus status, ConnectionStatusReason reason);
    void setAuthState(AuthState newState);

    quint64 newMessageId();

    QString userNameFromPackage(quint64 id) const;

    void startAuthTimer();
    void stopAuthTimer();

    void startPingTimer();
    void stopPingTimer();

    void addMessageToAck(quint64 id);

protected slots:
    void onTransportStateChanged();
    void onTransportPackageReceived(const QByteArray &package);
    void onTransportTimeout();
    void onTimeToPing();
    void onTimeToAckMessages();

protected:
    bool checkClientServerNonse(CTelegramStream &stream) const;

    ConnectionStatus m_status;
    const CAppInformation *m_appInfo;

    QMap<quint64, QByteArray> m_submittedPackages; // <message id, package data>
    QMap<quint64, quint32> m_requestedFilesIds; // <message id, file id>

    CTelegramTransport *m_transport;
    QTimer *m_authTimer;
    QTimer *m_pingTimer;
    QTimer *m_ackTimer;

    AuthState m_authState;

    QByteArray m_authKey;
    quint64 m_authId;
    quint64 m_authKeyAuxHash;
    quint64 m_serverSalt;
    quint64 m_receivedServerSalt;
    quint64 m_sessionId;
    quint64 m_lastMessageId;
    quint64 m_lastSentPingId;
    quint64 m_lastReceivedPingId;
    qint64 m_lastReceivedPingTime;
    qint64 m_lastSentPingTime;
    quint32 m_sequenceNumber;
    quint32 m_contentRelatedMessages;

    TLVector<quint64> m_messagesToAck;

    quint32 m_pingInterval;
    quint32 m_serverDisconnectionExtraTime;
    qint32 m_deltaTime;
    DeltaTimeHeuristicState m_deltaTimeHeuristicState;

    TLNumber128 m_clientNonce;
    TLNumber128 m_serverNonce;
    TLNumber256 m_newNonce;

    quint64 m_pq;
    quint32 m_p;
    quint32 m_q;

    Telegram::RsaKey m_rsaKey;
    SAesKey m_tmpAesKey;

    quint32 m_g;
    QByteArray m_dhPrime;
    QByteArray m_gA;
    QByteArray m_b;

    quint64 m_authRetryId;

    TLDcOption m_dcInfo;

    QVector<TLDcOption> m_dcConfiguration;

    QString m_authCodeHash;

#ifdef NETWORK_LOGGING
    QFile *m_logFile;
#endif

};

inline SAesKey CTelegramConnection::generateClientToServerAesKey(const QByteArray &messageKey) const
{
    return generateAesKey(messageKey, 0);
}

inline SAesKey CTelegramConnection::generateServerToClientAesKey(const QByteArray &messageKey) const
{
    return generateAesKey(messageKey, 8);
}

#endif // CTELEGRAMCONNECTION_HPP
