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

#include "CTelegramStream_p.hpp"

#include <QtEndian>

#include <QIODevice>
#include <QDebug>

template CTelegramStream &CTelegramStream::operator>>(TLVector<qint32> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<quint32> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<qint64> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<quint64> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<QString> &v);

template CTelegramStream &CTelegramStream::operator<<(const TLVector<qint32> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<quint32> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<qint64> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<quint64> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<QString> &v);

template CTelegramStream &CTelegramStream::operator>>(TLNumber128 &v);
template CTelegramStream &CTelegramStream::operator>>(TLNumber256 &v);

template CTelegramStream &CTelegramStream::operator<<(const TLNumber128 &v);
template CTelegramStream &CTelegramStream::operator<<(const TLNumber256 &v);

// Generated vector read templates instancing
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLAuthorization> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPrivacyRule> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLUser> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLWallPaper> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLContactBlocked> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLContact> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLContactStatus> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLImportedContact> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLChat> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPeer> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLDcOption> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLMessageEntity> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLUpdate> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLStickerSet> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLDialog> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLMessage> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLDocumentAttribute> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLBotInlineResult> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLDocument> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLStickerPack> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLReceivedNotifyMessage> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLFoundGif> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLEncryptedMessage> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLBotCommand> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLChatParticipant> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLKeyboardButton> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPhotoSize> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLMessageRange> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputUser> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLKeyboardButtonRow> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLChannelParticipant> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLBotInfo> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPhoto> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputPrivacyRule> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputChannel> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputContact> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputAppEvent> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputBotInlineResult> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputPhoto> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLSecureValue> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLSecureRequiredType> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLSecureValueError> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLWebAuthorization> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLSecureFile> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLChannelAdminLogEvent> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLSavedContact> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLTopPeerCategoryPeers> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPopularContact> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLJSONValue> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLJSONObjectValue> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLCdnPublicKey> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLRecentMeUrl> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLStickerSetCovered> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLDialogPeer> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLHighScore> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLFileHash> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLFutureSalt> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLRichText> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLLabeledPrice> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLTopPeer> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLIpPort> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLAccessPointRule> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputSecureFile> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLLangPackString> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPageTableCell> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLShippingOption> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPollAnswer> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPollAnswerVoters> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputDocument> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLSecureValueType> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPhoneConnection> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPageBlock> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPageListItem> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPageTableRow> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPageListOrderedItem> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLPageRelatedArticle> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLSecureValueHash> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputMessage> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputDialogPeer> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputSingleMedia> &v);
template CTelegramStream &CTelegramStream::operator>>(TLVector<TLInputStickerSetItem> &v);
// End of generated vector read templates instancing

// Generated vector write templates instancing
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputPrivacyRule> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputChannel> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputUser> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputContact> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputAppEvent> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLMessageEntity> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputBotInlineResult> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputPhoto> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLDocumentAttribute> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLKeyboardButtonRow> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLMessageRange> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLKeyboardButton> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLSecureValueHash> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLSecureValueType> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputMessage> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputDialogPeer> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputSingleMedia> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLShippingOption> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputStickerSetItem> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLSecureValueError> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputSecureFile> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLInputDocument> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLLabeledPrice> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLJSONValue> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLJSONObjectValue> &v);
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLPollAnswer> &v);
// End of generated vector write templates instancing
template CTelegramStream &CTelegramStream::operator<<(const TLVector<TLDcOption> &v);

template <int Size>
CTelegramStream &CTelegramStream::operator>>(TLNumber<Size> &n)
{
    read(n.data, Size / 8);
    return *this;
}

template <int Size>
CTelegramStream &CTelegramStream::operator<<(const TLNumber<Size> &n)
{
    write(n.data, Size / 8);
    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLDcOption &dcOption)
{
    *this << dcOption.tlType;
    *this << dcOption.flags;
    *this << dcOption.id;
    *this << dcOption.ipAddress;
    *this << dcOption.port;

    return *this;
}

// Generated read operators implementation
CTelegramStream &CTelegramStream::operator>>(TLAccountDaysTTL &accountDaysTTLValue)
{
    TLAccountDaysTTL result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountDaysTTL:
        *this >> result.days;
        break;
    default:
        break;
    }

    accountDaysTTLValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountSentEmailCode &accountSentEmailCodeValue)
{
    TLAccountSentEmailCode result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountSentEmailCode:
        *this >> result.emailPattern;
        *this >> result.length;
        break;
    default:
        break;
    }

    accountSentEmailCodeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountTakeout &accountTakeoutValue)
{
    TLAccountTakeout result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountTakeout:
        *this >> result.id;
        break;
    default:
        break;
    }

    accountTakeoutValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountTmpPassword &accountTmpPasswordValue)
{
    TLAccountTmpPassword result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountTmpPassword:
        *this >> result.tmpPassword;
        *this >> result.validUntil;
        break;
    default:
        break;
    }

    accountTmpPasswordValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAuthCheckedPhone &authCheckedPhoneValue)
{
    TLAuthCheckedPhone result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AuthCheckedPhone:
        *this >> result.phoneRegistered;
        break;
    default:
        break;
    }

    authCheckedPhoneValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAuthCodeType &authCodeTypeValue)
{
    TLAuthCodeType result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AuthCodeTypeSms:
    case TLValue::AuthCodeTypeCall:
    case TLValue::AuthCodeTypeFlashCall:
        break;
    default:
        break;
    }

    authCodeTypeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAuthExportedAuthorization &authExportedAuthorizationValue)
{
    TLAuthExportedAuthorization result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AuthExportedAuthorization:
        *this >> result.id;
        *this >> result.bytes;
        break;
    default:
        break;
    }

    authExportedAuthorizationValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAuthPasswordRecovery &authPasswordRecoveryValue)
{
    TLAuthPasswordRecovery result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AuthPasswordRecovery:
        *this >> result.emailPattern;
        break;
    default:
        break;
    }

    authPasswordRecoveryValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAuthSentCodeType &authSentCodeTypeValue)
{
    TLAuthSentCodeType result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AuthSentCodeTypeApp:
    case TLValue::AuthSentCodeTypeSms:
    case TLValue::AuthSentCodeTypeCall:
        *this >> result.length;
        break;
    case TLValue::AuthSentCodeTypeFlashCall:
        *this >> result.pattern;
        break;
    default:
        break;
    }

    authSentCodeTypeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLBadMsgNotification &badMsgNotificationValue)
{
    TLBadMsgNotification result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::BadMsgNotification:
        *this >> result.badMsgId;
        *this >> result.badMsgSeqno;
        *this >> result.errorCode;
        break;
    case TLValue::BadServerSalt:
        *this >> result.badMsgId;
        *this >> result.badMsgSeqno;
        *this >> result.errorCode;
        *this >> result.newServerSalt;
        break;
    default:
        break;
    }

    badMsgNotificationValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLBotCommand &botCommandValue)
{
    TLBotCommand result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::BotCommand:
        *this >> result.command;
        *this >> result.description;
        break;
    default:
        break;
    }

    botCommandValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLBotInfo &botInfoValue)
{
    TLBotInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::BotInfo:
        *this >> result.userId;
        *this >> result.description;
        *this >> result.commands;
        break;
    default:
        break;
    }

    botInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLCdnPublicKey &cdnPublicKeyValue)
{
    TLCdnPublicKey result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::CdnPublicKey:
        *this >> result.dcId;
        *this >> result.publicKey;
        break;
    default:
        break;
    }

    cdnPublicKeyValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChannelParticipantsFilter &channelParticipantsFilterValue)
{
    TLChannelParticipantsFilter result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChannelParticipantsRecent:
    case TLValue::ChannelParticipantsAdmins:
    case TLValue::ChannelParticipantsBots:
        break;
    case TLValue::ChannelParticipantsKicked:
    case TLValue::ChannelParticipantsBanned:
    case TLValue::ChannelParticipantsSearch:
        *this >> result.q;
        break;
    default:
        break;
    }

    channelParticipantsFilterValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChatOnlines &chatOnlinesValue)
{
    TLChatOnlines result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChatOnlines:
        *this >> result.onlines;
        break;
    default:
        break;
    }

    chatOnlinesValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChatParticipant &chatParticipantValue)
{
    TLChatParticipant result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChatParticipant:
    case TLValue::ChatParticipantAdmin:
        *this >> result.userId;
        *this >> result.inviterId;
        *this >> result.date;
        break;
    case TLValue::ChatParticipantCreator:
        *this >> result.userId;
        break;
    default:
        break;
    }

    chatParticipantValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChatParticipants &chatParticipantsValue)
{
    TLChatParticipants result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChatParticipantsForbidden:
        *this >> result.flags;
        *this >> result.chatId;
        if (result.flags & 1 << 0) {
            *this >> result.selfParticipant;
        }
        break;
    case TLValue::ChatParticipants:
        *this >> result.chatId;
        *this >> result.participants;
        *this >> result.version;
        break;
    default:
        break;
    }

    chatParticipantsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLClientDHInnerData &clientDHInnerDataValue)
{
    TLClientDHInnerData result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ClientDHInnerData:
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.retryId;
        *this >> result.gB;
        break;
    default:
        break;
    }

    clientDHInnerDataValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContact &contactValue)
{
    TLContact result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Contact:
        *this >> result.userId;
        *this >> result.mutual;
        break;
    default:
        break;
    }

    contactValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactBlocked &contactBlockedValue)
{
    TLContactBlocked result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactBlocked:
        *this >> result.userId;
        *this >> result.date;
        break;
    default:
        break;
    }

    contactBlockedValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactLink &contactLinkValue)
{
    TLContactLink result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactLinkUnknown:
    case TLValue::ContactLinkNone:
    case TLValue::ContactLinkHasPhone:
    case TLValue::ContactLinkContact:
        break;
    default:
        break;
    }

    contactLinkValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDataJSON &dataJSONValue)
{
    TLDataJSON result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DataJSON:
        *this >> result.data;
        break;
    default:
        break;
    }

    dataJSONValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDestroyAuthKeyRes &destroyAuthKeyResValue)
{
    TLDestroyAuthKeyRes result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DestroyAuthKeyOk:
    case TLValue::DestroyAuthKeyNone:
    case TLValue::DestroyAuthKeyFail:
        break;
    default:
        break;
    }

    destroyAuthKeyResValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDestroySessionRes &destroySessionResValue)
{
    TLDestroySessionRes result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DestroySessionOk:
    case TLValue::DestroySessionNone:
        *this >> result.sessionId;
        break;
    default:
        break;
    }

    destroySessionResValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLEncryptedChat &encryptedChatValue)
{
    TLEncryptedChat result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::EncryptedChatEmpty:
    case TLValue::EncryptedChatDiscarded:
        *this >> result.id;
        break;
    case TLValue::EncryptedChatWaiting:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.date;
        *this >> result.adminId;
        *this >> result.participantId;
        break;
    case TLValue::EncryptedChatRequested:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.date;
        *this >> result.adminId;
        *this >> result.participantId;
        *this >> result.gA;
        break;
    case TLValue::EncryptedChat:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.date;
        *this >> result.adminId;
        *this >> result.participantId;
        *this >> result.gAOrB;
        *this >> result.keyFingerprint;
        break;
    default:
        break;
    }

    encryptedChatValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLEncryptedFile &encryptedFileValue)
{
    TLEncryptedFile result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::EncryptedFileEmpty:
        break;
    case TLValue::EncryptedFile:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.size;
        *this >> result.dcId;
        *this >> result.keyFingerprint;
        break;
    default:
        break;
    }

    encryptedFileValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLEncryptedMessage &encryptedMessageValue)
{
    TLEncryptedMessage result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::EncryptedMessage:
        *this >> result.randomId;
        *this >> result.chatId;
        *this >> result.date;
        *this >> result.bytes;
        *this >> result.file;
        break;
    case TLValue::EncryptedMessageService:
        *this >> result.randomId;
        *this >> result.chatId;
        *this >> result.date;
        *this >> result.bytes;
        break;
    default:
        break;
    }

    encryptedMessageValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLError &errorValue)
{
    TLError result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Error:
        *this >> result.code;
        *this >> result.text;
        break;
    default:
        break;
    }

    errorValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLExportedChatInvite &exportedChatInviteValue)
{
    TLExportedChatInvite result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChatInviteEmpty:
        break;
    case TLValue::ChatInviteExported:
        *this >> result.link;
        break;
    default:
        break;
    }

    exportedChatInviteValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLExportedMessageLink &exportedMessageLinkValue)
{
    TLExportedMessageLink result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ExportedMessageLink:
        *this >> result.link;
        *this >> result.html;
        break;
    default:
        break;
    }

    exportedMessageLinkValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLFileHash &fileHashValue)
{
    TLFileHash result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::FileHash:
        *this >> result.offset;
        *this >> result.limit;
        *this >> result.hash;
        break;
    default:
        break;
    }

    fileHashValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLFileLocation &fileLocationValue)
{
    TLFileLocation result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::FileLocationUnavailable:
        *this >> result.volumeId;
        *this >> result.localId;
        *this >> result.secret;
        break;
    case TLValue::FileLocation:
        *this >> result.dcId;
        *this >> result.volumeId;
        *this >> result.localId;
        *this >> result.secret;
        *this >> result.fileReference;
        break;
    default:
        break;
    }

    fileLocationValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLFutureSalt &futureSaltValue)
{
    TLFutureSalt result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::FutureSalt:
        *this >> result.validSince;
        *this >> result.validUntil;
        *this >> result.salt;
        break;
    default:
        break;
    }

    futureSaltValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLFutureSalts &futureSaltsValue)
{
    TLFutureSalts result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::FutureSalts:
        *this >> result.reqMsgId;
        *this >> result.now;
        *this >> result.salts;
        break;
    default:
        break;
    }

    futureSaltsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLGeoPoint &geoPointValue)
{
    TLGeoPoint result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::GeoPointEmpty:
        break;
    case TLValue::GeoPoint:
        *this >> result.longitude;
        *this >> result.latitude;
        *this >> result.accessHash;
        break;
    default:
        break;
    }

    geoPointValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpInviteText &helpInviteTextValue)
{
    TLHelpInviteText result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpInviteText:
        *this >> result.message;
        break;
    default:
        break;
    }

    helpInviteTextValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpPassportConfig &helpPassportConfigValue)
{
    TLHelpPassportConfig result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpPassportConfigNotModified:
        break;
    case TLValue::HelpPassportConfig:
        *this >> result.hash;
        *this >> result.countriesLangs;
        break;
    default:
        break;
    }

    helpPassportConfigValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpSupportName &helpSupportNameValue)
{
    TLHelpSupportName result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpSupportName:
        *this >> result.name;
        break;
    default:
        break;
    }

    helpSupportNameValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHighScore &highScoreValue)
{
    TLHighScore result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HighScore:
        *this >> result.pos;
        *this >> result.userId;
        *this >> result.score;
        break;
    default:
        break;
    }

    highScoreValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHttpWait &httpWaitValue)
{
    TLHttpWait result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HttpWait:
        *this >> result.maxDelay;
        *this >> result.waitAfter;
        *this >> result.maxWait;
        break;
    default:
        break;
    }

    httpWaitValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLImportedContact &importedContactValue)
{
    TLImportedContact result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ImportedContact:
        *this >> result.userId;
        *this >> result.clientId;
        break;
    default:
        break;
    }

    importedContactValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInlineBotSwitchPM &inlineBotSwitchPMValue)
{
    TLInlineBotSwitchPM result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InlineBotSwitchPM:
        *this >> result.text;
        *this >> result.startParam;
        break;
    default:
        break;
    }

    inlineBotSwitchPMValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputBotInlineMessageID &inputBotInlineMessageIDValue)
{
    TLInputBotInlineMessageID result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputBotInlineMessageID:
        *this >> result.dcId;
        *this >> result.id;
        *this >> result.accessHash;
        break;
    default:
        break;
    }

    inputBotInlineMessageIDValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputChannel &inputChannelValue)
{
    TLInputChannel result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputChannelEmpty:
        break;
    case TLValue::InputChannel:
        *this >> result.channelId;
        *this >> result.accessHash;
        break;
    default:
        break;
    }

    inputChannelValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputCheckPasswordSRP &inputCheckPasswordSRPValue)
{
    TLInputCheckPasswordSRP result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputCheckPasswordEmpty:
        break;
    case TLValue::InputCheckPasswordSRP:
        *this >> result.srpId;
        *this >> result.A;
        *this >> result.M1;
        break;
    default:
        break;
    }

    inputCheckPasswordSRPValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputClientProxy &inputClientProxyValue)
{
    TLInputClientProxy result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputClientProxy:
        *this >> result.address;
        *this >> result.port;
        break;
    default:
        break;
    }

    inputClientProxyValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputContact &inputContactValue)
{
    TLInputContact result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputPhoneContact:
        *this >> result.clientId;
        *this >> result.phone;
        *this >> result.firstName;
        *this >> result.lastName;
        break;
    default:
        break;
    }

    inputContactValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputDocument &inputDocumentValue)
{
    TLInputDocument result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputDocumentEmpty:
        break;
    case TLValue::InputDocument:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.fileReference;
        break;
    default:
        break;
    }

    inputDocumentValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputEncryptedChat &inputEncryptedChatValue)
{
    TLInputEncryptedChat result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputEncryptedChat:
        *this >> result.chatId;
        *this >> result.accessHash;
        break;
    default:
        break;
    }

    inputEncryptedChatValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputEncryptedFile &inputEncryptedFileValue)
{
    TLInputEncryptedFile result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputEncryptedFileEmpty:
        break;
    case TLValue::InputEncryptedFileUploaded:
        *this >> result.id;
        *this >> result.parts;
        *this >> result.md5Checksum;
        *this >> result.keyFingerprint;
        break;
    case TLValue::InputEncryptedFile:
        *this >> result.id;
        *this >> result.accessHash;
        break;
    case TLValue::InputEncryptedFileBigUploaded:
        *this >> result.id;
        *this >> result.parts;
        *this >> result.keyFingerprint;
        break;
    default:
        break;
    }

    inputEncryptedFileValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputFile &inputFileValue)
{
    TLInputFile result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputFile:
        *this >> result.id;
        *this >> result.parts;
        *this >> result.name;
        *this >> result.md5Checksum;
        break;
    case TLValue::InputFileBig:
        *this >> result.id;
        *this >> result.parts;
        *this >> result.name;
        break;
    default:
        break;
    }

    inputFileValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputFileLocation &inputFileLocationValue)
{
    TLInputFileLocation result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputFileLocation:
        *this >> result.volumeId;
        *this >> result.localId;
        *this >> result.secret;
        *this >> result.fileReference;
        break;
    case TLValue::InputEncryptedFileLocation:
    case TLValue::InputSecureFileLocation:
        *this >> result.id;
        *this >> result.accessHash;
        break;
    case TLValue::InputDocumentFileLocation:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.fileReference;
        break;
    case TLValue::InputTakeoutFileLocation:
        break;
    default:
        break;
    }

    inputFileLocationValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputGeoPoint &inputGeoPointValue)
{
    TLInputGeoPoint result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputGeoPointEmpty:
        break;
    case TLValue::InputGeoPoint:
        *this >> result.latitude;
        *this >> result.longitude;
        break;
    default:
        break;
    }

    inputGeoPointValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputMessage &inputMessageValue)
{
    TLInputMessage result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputMessageID:
    case TLValue::InputMessageReplyTo:
        *this >> result.id;
        break;
    case TLValue::InputMessagePinned:
        break;
    default:
        break;
    }

    inputMessageValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputPeer &inputPeerValue)
{
    TLInputPeer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputPeerEmpty:
    case TLValue::InputPeerSelf:
        break;
    case TLValue::InputPeerChat:
        *this >> result.chatId;
        break;
    case TLValue::InputPeerUser:
        *this >> result.userId;
        *this >> result.accessHash;
        break;
    case TLValue::InputPeerChannel:
        *this >> result.channelId;
        *this >> result.accessHash;
        break;
    default:
        break;
    }

    inputPeerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputPeerNotifySettings &inputPeerNotifySettingsValue)
{
    TLInputPeerNotifySettings result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputPeerNotifySettings:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.showPreviews;
        }
        if (result.flags & 1 << 1) {
            *this >> result.silent;
        }
        if (result.flags & 1 << 2) {
            *this >> result.muteUntil;
        }
        if (result.flags & 1 << 3) {
            *this >> result.sound;
        }
        break;
    default:
        break;
    }

    inputPeerNotifySettingsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputPhoneCall &inputPhoneCallValue)
{
    TLInputPhoneCall result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputPhoneCall:
        *this >> result.id;
        *this >> result.accessHash;
        break;
    default:
        break;
    }

    inputPhoneCallValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputPhoto &inputPhotoValue)
{
    TLInputPhoto result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputPhotoEmpty:
        break;
    case TLValue::InputPhoto:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.fileReference;
        break;
    default:
        break;
    }

    inputPhotoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputPrivacyKey &inputPrivacyKeyValue)
{
    TLInputPrivacyKey result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputPrivacyKeyStatusTimestamp:
    case TLValue::InputPrivacyKeyChatInvite:
    case TLValue::InputPrivacyKeyPhoneCall:
    case TLValue::InputPrivacyKeyPhoneP2P:
        break;
    default:
        break;
    }

    inputPrivacyKeyValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputSecureFile &inputSecureFileValue)
{
    TLInputSecureFile result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputSecureFileUploaded:
        *this >> result.id;
        *this >> result.parts;
        *this >> result.md5Checksum;
        *this >> result.fileHash;
        *this >> result.secret;
        break;
    case TLValue::InputSecureFile:
        *this >> result.id;
        *this >> result.accessHash;
        break;
    default:
        break;
    }

    inputSecureFileValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputStickerSet &inputStickerSetValue)
{
    TLInputStickerSet result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputStickerSetEmpty:
        break;
    case TLValue::InputStickerSetID:
        *this >> result.id;
        *this >> result.accessHash;
        break;
    case TLValue::InputStickerSetShortName:
        *this >> result.shortName;
        break;
    default:
        break;
    }

    inputStickerSetValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputStickeredMedia &inputStickeredMediaValue)
{
    TLInputStickeredMedia result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputStickeredMediaPhoto:
        *this >> result.inputPhotoId;
        break;
    case TLValue::InputStickeredMediaDocument:
        *this >> result.inputDocumentId;
        break;
    default:
        break;
    }

    inputStickeredMediaValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputUser &inputUserValue)
{
    TLInputUser result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputUserEmpty:
    case TLValue::InputUserSelf:
        break;
    case TLValue::InputUser:
        *this >> result.userId;
        *this >> result.accessHash;
        break;
    default:
        break;
    }

    inputUserValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputWebFileLocation &inputWebFileLocationValue)
{
    TLInputWebFileLocation result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputWebFileLocation:
        *this >> result.url;
        *this >> result.accessHash;
        break;
    case TLValue::InputWebFileGeoPointLocation:
        *this >> result.geoPoint;
        *this >> result.accessHash;
        *this >> result.w;
        *this >> result.h;
        *this >> result.zoom;
        *this >> result.scale;
        break;
    default:
        break;
    }

    inputWebFileLocationValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLIpPort &ipPortValue)
{
    TLIpPort result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::IpPort:
        *this >> result.ipv4;
        *this >> result.port;
        break;
    case TLValue::IpPortSecret:
        *this >> result.ipv4;
        *this >> result.port;
        *this >> result.secret;
        break;
    default:
        break;
    }

    ipPortValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLLabeledPrice &labeledPriceValue)
{
    TLLabeledPrice result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::LabeledPrice:
        *this >> result.label;
        *this >> result.amount;
        break;
    default:
        break;
    }

    labeledPriceValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLLangPackString &langPackStringValue)
{
    TLLangPackString result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::LangPackString:
        *this >> result.key;
        *this >> result.value;
        break;
    case TLValue::LangPackStringPluralized:
        *this >> result.flags;
        *this >> result.key;
        if (result.flags & 1 << 0) {
            *this >> result.zeroValue;
        }
        if (result.flags & 1 << 1) {
            *this >> result.oneValue;
        }
        if (result.flags & 1 << 2) {
            *this >> result.twoValue;
        }
        if (result.flags & 1 << 3) {
            *this >> result.fewValue;
        }
        if (result.flags & 1 << 4) {
            *this >> result.manyValue;
        }
        *this >> result.otherValue;
        break;
    case TLValue::LangPackStringDeleted:
        *this >> result.key;
        break;
    default:
        break;
    }

    langPackStringValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMaskCoords &maskCoordsValue)
{
    TLMaskCoords result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MaskCoords:
        *this >> result.n;
        *this >> result.x;
        *this >> result.y;
        *this >> result.zoom;
        break;
    default:
        break;
    }

    maskCoordsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessageEntity &messageEntityValue)
{
    TLMessageEntity result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessageEntityUnknown:
    case TLValue::MessageEntityMention:
    case TLValue::MessageEntityHashtag:
    case TLValue::MessageEntityBotCommand:
    case TLValue::MessageEntityUrl:
    case TLValue::MessageEntityEmail:
    case TLValue::MessageEntityBold:
    case TLValue::MessageEntityItalic:
    case TLValue::MessageEntityCode:
    case TLValue::MessageEntityPhone:
    case TLValue::MessageEntityCashtag:
        *this >> result.offset;
        *this >> result.length;
        break;
    case TLValue::MessageEntityPre:
        *this >> result.offset;
        *this >> result.length;
        *this >> result.language;
        break;
    case TLValue::MessageEntityTextUrl:
        *this >> result.offset;
        *this >> result.length;
        *this >> result.url;
        break;
    case TLValue::MessageEntityMentionName:
        *this >> result.offset;
        *this >> result.length;
        *this >> result.quint32UserId;
        break;
    case TLValue::InputMessageEntityMentionName:
        *this >> result.offset;
        *this >> result.length;
        *this >> result.inputUserUserId;
        break;
    default:
        break;
    }

    messageEntityValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessageRange &messageRangeValue)
{
    TLMessageRange result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessageRange:
        *this >> result.minId;
        *this >> result.maxId;
        break;
    default:
        break;
    }

    messageRangeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesAffectedHistory &messagesAffectedHistoryValue)
{
    TLMessagesAffectedHistory result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesAffectedHistory:
        *this >> result.pts;
        *this >> result.ptsCount;
        *this >> result.offset;
        break;
    default:
        break;
    }

    messagesAffectedHistoryValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesAffectedMessages &messagesAffectedMessagesValue)
{
    TLMessagesAffectedMessages result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesAffectedMessages:
        *this >> result.pts;
        *this >> result.ptsCount;
        break;
    default:
        break;
    }

    messagesAffectedMessagesValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesDhConfig &messagesDhConfigValue)
{
    TLMessagesDhConfig result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesDhConfigNotModified:
        *this >> result.random;
        break;
    case TLValue::MessagesDhConfig:
        *this >> result.g;
        *this >> result.p;
        *this >> result.version;
        *this >> result.random;
        break;
    default:
        break;
    }

    messagesDhConfigValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesSentEncryptedMessage &messagesSentEncryptedMessageValue)
{
    TLMessagesSentEncryptedMessage result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesSentEncryptedMessage:
        *this >> result.date;
        break;
    case TLValue::MessagesSentEncryptedFile:
        *this >> result.date;
        *this >> result.file;
        break;
    default:
        break;
    }

    messagesSentEncryptedMessageValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMsgDetailedInfo &msgDetailedInfoValue)
{
    TLMsgDetailedInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MsgDetailedInfo:
        *this >> result.msgId;
        *this >> result.answerMsgId;
        *this >> result.bytes;
        *this >> result.status;
        break;
    case TLValue::MsgNewDetailedInfo:
        *this >> result.answerMsgId;
        *this >> result.bytes;
        *this >> result.status;
        break;
    default:
        break;
    }

    msgDetailedInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMsgResendReq &msgResendReqValue)
{
    TLMsgResendReq result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MsgResendReq:
        *this >> result.msgIds;
        break;
    default:
        break;
    }

    msgResendReqValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMsgsAck &msgsAckValue)
{
    TLMsgsAck result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MsgsAck:
        *this >> result.msgIds;
        break;
    default:
        break;
    }

    msgsAckValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMsgsAllInfo &msgsAllInfoValue)
{
    TLMsgsAllInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MsgsAllInfo:
        *this >> result.msgIds;
        *this >> result.info;
        break;
    default:
        break;
    }

    msgsAllInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMsgsStateInfo &msgsStateInfoValue)
{
    TLMsgsStateInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MsgsStateInfo:
        *this >> result.reqMsgId;
        *this >> result.info;
        break;
    default:
        break;
    }

    msgsStateInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMsgsStateReq &msgsStateReqValue)
{
    TLMsgsStateReq result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MsgsStateReq:
        *this >> result.msgIds;
        break;
    default:
        break;
    }

    msgsStateReqValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLNearestDc &nearestDcValue)
{
    TLNearestDc result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::NearestDc:
        *this >> result.country;
        *this >> result.thisDc;
        *this >> result.nearestDc;
        break;
    default:
        break;
    }

    nearestDcValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLNewSession &newSessionValue)
{
    TLNewSession result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::NewSessionCreated:
        *this >> result.firstMsgId;
        *this >> result.uniqueId;
        *this >> result.serverSalt;
        break;
    default:
        break;
    }

    newSessionValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPQInnerData &pQInnerDataValue)
{
    TLPQInnerData result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PQInnerData:
        *this >> result.pq;
        *this >> result.p;
        *this >> result.q;
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.newNonce;
        break;
    case TLValue::PQInnerDataDc:
        *this >> result.pq;
        *this >> result.p;
        *this >> result.q;
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.newNonce;
        *this >> result.dc;
        break;
    case TLValue::PQInnerDataTemp:
        *this >> result.pq;
        *this >> result.p;
        *this >> result.q;
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.newNonce;
        *this >> result.expiresIn;
        break;
    case TLValue::PQInnerDataTempDc:
        *this >> result.pq;
        *this >> result.p;
        *this >> result.q;
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.newNonce;
        *this >> result.dc;
        *this >> result.expiresIn;
        break;
    default:
        break;
    }

    pQInnerDataValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPageRelatedArticle &pageRelatedArticleValue)
{
    TLPageRelatedArticle result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PageRelatedArticle:
        *this >> result.flags;
        *this >> result.url;
        *this >> result.webpageId;
        if (result.flags & 1 << 0) {
            *this >> result.title;
        }
        if (result.flags & 1 << 1) {
            *this >> result.description;
        }
        if (result.flags & 1 << 2) {
            *this >> result.photoId;
        }
        if (result.flags & 1 << 3) {
            *this >> result.author;
        }
        if (result.flags & 1 << 4) {
            *this >> result.publishedDate;
        }
        break;
    default:
        break;
    }

    pageRelatedArticleValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPasswordKdfAlgo &passwordKdfAlgoValue)
{
    TLPasswordKdfAlgo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PasswordKdfAlgoUnknown:
        break;
    case TLValue::PasswordKdfAlgoSHA256SHA256PBKDF2HMACSHA512iter100000SHA256ModPow:
        *this >> result.salt1;
        *this >> result.salt2;
        *this >> result.g;
        *this >> result.p;
        break;
    default:
        break;
    }

    passwordKdfAlgoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPaymentCharge &paymentChargeValue)
{
    TLPaymentCharge result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PaymentCharge:
        *this >> result.id;
        *this >> result.providerChargeId;
        break;
    default:
        break;
    }

    paymentChargeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPaymentSavedCredentials &paymentSavedCredentialsValue)
{
    TLPaymentSavedCredentials result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PaymentSavedCredentialsCard:
        *this >> result.id;
        *this >> result.title;
        break;
    default:
        break;
    }

    paymentSavedCredentialsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPeer &peerValue)
{
    TLPeer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PeerUser:
        *this >> result.userId;
        break;
    case TLValue::PeerChat:
        *this >> result.chatId;
        break;
    case TLValue::PeerChannel:
        *this >> result.channelId;
        break;
    default:
        break;
    }

    peerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPeerNotifySettings &peerNotifySettingsValue)
{
    TLPeerNotifySettings result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PeerNotifySettings:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.showPreviews;
        }
        if (result.flags & 1 << 1) {
            *this >> result.silent;
        }
        if (result.flags & 1 << 2) {
            *this >> result.muteUntil;
        }
        if (result.flags & 1 << 3) {
            *this >> result.sound;
        }
        break;
    default:
        break;
    }

    peerNotifySettingsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhoneCallDiscardReason &phoneCallDiscardReasonValue)
{
    TLPhoneCallDiscardReason result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhoneCallDiscardReasonMissed:
    case TLValue::PhoneCallDiscardReasonDisconnect:
    case TLValue::PhoneCallDiscardReasonHangup:
    case TLValue::PhoneCallDiscardReasonBusy:
        break;
    default:
        break;
    }

    phoneCallDiscardReasonValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhoneConnection &phoneConnectionValue)
{
    TLPhoneConnection result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhoneConnection:
        *this >> result.id;
        *this >> result.ip;
        *this >> result.ipv6;
        *this >> result.port;
        *this >> result.peerTag;
        break;
    default:
        break;
    }

    phoneConnectionValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhotoSize &photoSizeValue)
{
    TLPhotoSize result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhotoSizeEmpty:
        *this >> result.type;
        break;
    case TLValue::PhotoSize:
        *this >> result.type;
        *this >> result.location;
        *this >> result.w;
        *this >> result.h;
        *this >> result.size;
        break;
    case TLValue::PhotoCachedSize:
        *this >> result.type;
        *this >> result.location;
        *this >> result.w;
        *this >> result.h;
        *this >> result.bytes;
        break;
    default:
        break;
    }

    photoSizeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPollAnswer &pollAnswerValue)
{
    TLPollAnswer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PollAnswer:
        *this >> result.text;
        *this >> result.option;
        break;
    default:
        break;
    }

    pollAnswerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPong &pongValue)
{
    TLPong result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Pong:
        *this >> result.msgId;
        *this >> result.pingId;
        break;
    default:
        break;
    }

    pongValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPopularContact &popularContactValue)
{
    TLPopularContact result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PopularContact:
        *this >> result.clientId;
        *this >> result.importers;
        break;
    default:
        break;
    }

    popularContactValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPostAddress &postAddressValue)
{
    TLPostAddress result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PostAddress:
        *this >> result.streetLine1;
        *this >> result.streetLine2;
        *this >> result.city;
        *this >> result.state;
        *this >> result.countryIso2;
        *this >> result.postCode;
        break;
    default:
        break;
    }

    postAddressValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPrivacyKey &privacyKeyValue)
{
    TLPrivacyKey result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PrivacyKeyStatusTimestamp:
    case TLValue::PrivacyKeyChatInvite:
    case TLValue::PrivacyKeyPhoneCall:
    case TLValue::PrivacyKeyPhoneP2P:
        break;
    default:
        break;
    }

    privacyKeyValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPrivacyRule &privacyRuleValue)
{
    TLPrivacyRule result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PrivacyValueAllowContacts:
    case TLValue::PrivacyValueAllowAll:
    case TLValue::PrivacyValueDisallowContacts:
    case TLValue::PrivacyValueDisallowAll:
        break;
    case TLValue::PrivacyValueAllowUsers:
    case TLValue::PrivacyValueDisallowUsers:
        *this >> result.users;
        break;
    default:
        break;
    }

    privacyRuleValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLReceivedNotifyMessage &receivedNotifyMessageValue)
{
    TLReceivedNotifyMessage result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ReceivedNotifyMessage:
        *this >> result.id;
        *this >> result.flags;
        break;
    default:
        break;
    }

    receivedNotifyMessageValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLReportReason &reportReasonValue)
{
    TLReportReason result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputReportReasonSpam:
    case TLValue::InputReportReasonViolence:
    case TLValue::InputReportReasonPornography:
    case TLValue::InputReportReasonChildAbuse:
    case TLValue::InputReportReasonCopyright:
        break;
    case TLValue::InputReportReasonOther:
        *this >> result.text;
        break;
    default:
        break;
    }

    reportReasonValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLResPQ &resPQValue)
{
    TLResPQ result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ResPQ:
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.pq;
        *this >> result.serverPublicKeyFingerprints;
        break;
    default:
        break;
    }

    resPQValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLRichText &richTextValue)
{
    TLRichText result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::TextEmpty:
        break;
    case TLValue::TextPlain:
        *this >> result.stringText;
        break;
    case TLValue::TextBold:
    case TLValue::TextItalic:
    case TLValue::TextUnderline:
    case TLValue::TextStrike:
    case TLValue::TextFixed:
    case TLValue::TextSubscript:
    case TLValue::TextSuperscript:
    case TLValue::TextMarked:
        *this >> *result.richText;
        break;
    case TLValue::TextUrl:
        *this >> *result.richText;
        *this >> result.url;
        *this >> result.webpageId;
        break;
    case TLValue::TextEmail:
        *this >> *result.richText;
        *this >> result.email;
        break;
    case TLValue::TextConcat:
        *this >> result.texts;
        break;
    case TLValue::TextPhone:
        *this >> *result.richText;
        *this >> result.phone;
        break;
    case TLValue::TextImage:
        *this >> result.documentId;
        *this >> result.w;
        *this >> result.h;
        break;
    case TLValue::TextAnchor:
        *this >> *result.richText;
        *this >> result.name;
        break;
    default:
        break;
    }

    richTextValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLRpcDropAnswer &rpcDropAnswerValue)
{
    TLRpcDropAnswer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::RpcAnswerUnknown:
    case TLValue::RpcAnswerDroppedRunning:
        break;
    case TLValue::RpcAnswerDropped:
        *this >> result.msgId;
        *this >> result.seqNo;
        *this >> result.bytes;
        break;
    default:
        break;
    }

    rpcDropAnswerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLRpcError &rpcErrorValue)
{
    TLRpcError result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::RpcError:
        *this >> result.errorCode;
        *this >> result.errorMessage;
        break;
    default:
        break;
    }

    rpcErrorValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSavedContact &savedContactValue)
{
    TLSavedContact result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SavedPhoneContact:
        *this >> result.phone;
        *this >> result.firstName;
        *this >> result.lastName;
        *this >> result.date;
        break;
    default:
        break;
    }

    savedContactValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureCredentialsEncrypted &secureCredentialsEncryptedValue)
{
    TLSecureCredentialsEncrypted result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureCredentialsEncrypted:
        *this >> result.data;
        *this >> result.hash;
        *this >> result.secret;
        break;
    default:
        break;
    }

    secureCredentialsEncryptedValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureData &secureDataValue)
{
    TLSecureData result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureData:
        *this >> result.data;
        *this >> result.dataHash;
        *this >> result.secret;
        break;
    default:
        break;
    }

    secureDataValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureFile &secureFileValue)
{
    TLSecureFile result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureFileEmpty:
        break;
    case TLValue::SecureFile:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.size;
        *this >> result.dcId;
        *this >> result.date;
        *this >> result.fileHash;
        *this >> result.secret;
        break;
    default:
        break;
    }

    secureFileValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecurePasswordKdfAlgo &securePasswordKdfAlgoValue)
{
    TLSecurePasswordKdfAlgo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecurePasswordKdfAlgoUnknown:
        break;
    case TLValue::SecurePasswordKdfAlgoPBKDF2HMACSHA512iter100000:
    case TLValue::SecurePasswordKdfAlgoSHA512:
        *this >> result.salt;
        break;
    default:
        break;
    }

    securePasswordKdfAlgoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecurePlainData &securePlainDataValue)
{
    TLSecurePlainData result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecurePlainPhone:
        *this >> result.phone;
        break;
    case TLValue::SecurePlainEmail:
        *this >> result.email;
        break;
    default:
        break;
    }

    securePlainDataValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureSecretSettings &secureSecretSettingsValue)
{
    TLSecureSecretSettings result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureSecretSettings:
        *this >> result.secureAlgo;
        *this >> result.secureSecret;
        *this >> result.secureSecretId;
        break;
    default:
        break;
    }

    secureSecretSettingsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureValueType &secureValueTypeValue)
{
    TLSecureValueType result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureValueTypePersonalDetails:
    case TLValue::SecureValueTypePassport:
    case TLValue::SecureValueTypeDriverLicense:
    case TLValue::SecureValueTypeIdentityCard:
    case TLValue::SecureValueTypeInternalPassport:
    case TLValue::SecureValueTypeAddress:
    case TLValue::SecureValueTypeUtilityBill:
    case TLValue::SecureValueTypeBankStatement:
    case TLValue::SecureValueTypeRentalAgreement:
    case TLValue::SecureValueTypePassportRegistration:
    case TLValue::SecureValueTypeTemporaryRegistration:
    case TLValue::SecureValueTypePhone:
    case TLValue::SecureValueTypeEmail:
        break;
    default:
        break;
    }

    secureValueTypeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSendMessageAction &sendMessageActionValue)
{
    TLSendMessageAction result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SendMessageTypingAction:
    case TLValue::SendMessageCancelAction:
    case TLValue::SendMessageRecordVideoAction:
    case TLValue::SendMessageRecordAudioAction:
    case TLValue::SendMessageGeoLocationAction:
    case TLValue::SendMessageChooseContactAction:
    case TLValue::SendMessageGamePlayAction:
    case TLValue::SendMessageRecordRoundAction:
        break;
    case TLValue::SendMessageUploadVideoAction:
    case TLValue::SendMessageUploadAudioAction:
    case TLValue::SendMessageUploadPhotoAction:
    case TLValue::SendMessageUploadDocumentAction:
    case TLValue::SendMessageUploadRoundAction:
        *this >> result.progress;
        break;
    default:
        break;
    }

    sendMessageActionValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLServerDHInnerData &serverDHInnerDataValue)
{
    TLServerDHInnerData result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ServerDHInnerData:
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.g;
        *this >> result.dhPrime;
        *this >> result.gA;
        *this >> result.serverTime;
        break;
    default:
        break;
    }

    serverDHInnerDataValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLServerDHParams &serverDHParamsValue)
{
    TLServerDHParams result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ServerDHParamsFail:
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.newNonceHash;
        break;
    case TLValue::ServerDHParamsOk:
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.encryptedAnswer;
        break;
    default:
        break;
    }

    serverDHParamsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSetClientDHParamsAnswer &setClientDHParamsAnswerValue)
{
    TLSetClientDHParamsAnswer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DhGenOk:
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.newNonceHash1;
        break;
    case TLValue::DhGenRetry:
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.newNonceHash2;
        break;
    case TLValue::DhGenFail:
        *this >> result.nonce;
        *this >> result.serverNonce;
        *this >> result.newNonceHash3;
        break;
    default:
        break;
    }

    setClientDHParamsAnswerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLShippingOption &shippingOptionValue)
{
    TLShippingOption result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ShippingOption:
        *this >> result.id;
        *this >> result.title;
        *this >> result.prices;
        break;
    default:
        break;
    }

    shippingOptionValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLStatsURL &statsURLValue)
{
    TLStatsURL result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::StatsURL:
        *this >> result.url;
        break;
    default:
        break;
    }

    statsURLValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLStickerPack &stickerPackValue)
{
    TLStickerPack result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::StickerPack:
        *this >> result.emoticon;
        *this >> result.documents;
        break;
    default:
        break;
    }

    stickerPackValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLStorageFileType &storageFileTypeValue)
{
    TLStorageFileType result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::StorageFileUnknown:
    case TLValue::StorageFilePartial:
    case TLValue::StorageFileJpeg:
    case TLValue::StorageFileGif:
    case TLValue::StorageFilePng:
    case TLValue::StorageFilePdf:
    case TLValue::StorageFileMp3:
    case TLValue::StorageFileMov:
    case TLValue::StorageFileMp4:
    case TLValue::StorageFileWebp:
        break;
    default:
        break;
    }

    storageFileTypeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLTopPeer &topPeerValue)
{
    TLTopPeer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::TopPeer:
        *this >> result.peer;
        *this >> result.rating;
        break;
    default:
        break;
    }

    topPeerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLTopPeerCategory &topPeerCategoryValue)
{
    TLTopPeerCategory result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::TopPeerCategoryBotsPM:
    case TLValue::TopPeerCategoryBotsInline:
    case TLValue::TopPeerCategoryCorrespondents:
    case TLValue::TopPeerCategoryGroups:
    case TLValue::TopPeerCategoryChannels:
    case TLValue::TopPeerCategoryPhoneCalls:
        break;
    default:
        break;
    }

    topPeerCategoryValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLTopPeerCategoryPeers &topPeerCategoryPeersValue)
{
    TLTopPeerCategoryPeers result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::TopPeerCategoryPeers:
        *this >> result.category;
        *this >> result.count;
        *this >> result.peers;
        break;
    default:
        break;
    }

    topPeerCategoryPeersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLUpdatesState &updatesStateValue)
{
    TLUpdatesState result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::UpdatesState:
        *this >> result.pts;
        *this >> result.qts;
        *this >> result.date;
        *this >> result.seq;
        *this >> result.unreadCount;
        break;
    default:
        break;
    }

    updatesStateValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLUploadCdnFile &uploadCdnFileValue)
{
    TLUploadCdnFile result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::UploadCdnFileReuploadNeeded:
        *this >> result.requestToken;
        break;
    case TLValue::UploadCdnFile:
        *this >> result.bytes;
        break;
    default:
        break;
    }

    uploadCdnFileValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLUploadFile &uploadFileValue)
{
    TLUploadFile result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::UploadFile:
        *this >> result.type;
        *this >> result.mtime;
        *this >> result.bytes;
        break;
    case TLValue::UploadFileCdnRedirect:
        *this >> result.dcId;
        *this >> result.fileToken;
        *this >> result.encryptionKey;
        *this >> result.encryptionIv;
        *this >> result.fileHashes;
        break;
    default:
        break;
    }

    uploadFileValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLUploadWebFile &uploadWebFileValue)
{
    TLUploadWebFile result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::UploadWebFile:
        *this >> result.size;
        *this >> result.mimeType;
        *this >> result.fileType;
        *this >> result.mtime;
        *this >> result.bytes;
        break;
    default:
        break;
    }

    uploadWebFileValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLUserProfilePhoto &userProfilePhotoValue)
{
    TLUserProfilePhoto result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::UserProfilePhotoEmpty:
        break;
    case TLValue::UserProfilePhoto:
        *this >> result.photoId;
        *this >> result.photoSmall;
        *this >> result.photoBig;
        break;
    default:
        break;
    }

    userProfilePhotoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLUserStatus &userStatusValue)
{
    TLUserStatus result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::UserStatusEmpty:
    case TLValue::UserStatusRecently:
    case TLValue::UserStatusLastWeek:
    case TLValue::UserStatusLastMonth:
        break;
    case TLValue::UserStatusOnline:
        *this >> result.expires;
        break;
    case TLValue::UserStatusOffline:
        *this >> result.wasOnline;
        break;
    default:
        break;
    }

    userStatusValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLWallPaper &wallPaperValue)
{
    TLWallPaper result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::WallPaper:
        *this >> result.id;
        *this >> result.title;
        *this >> result.sizes;
        *this >> result.color;
        break;
    case TLValue::WallPaperSolid:
        *this >> result.id;
        *this >> result.title;
        *this >> result.bgColor;
        *this >> result.color;
        break;
    default:
        break;
    }

    wallPaperValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLWebAuthorization &webAuthorizationValue)
{
    TLWebAuthorization result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::WebAuthorization:
        *this >> result.hash;
        *this >> result.botId;
        *this >> result.domain;
        *this >> result.browser;
        *this >> result.platform;
        *this >> result.dateCreated;
        *this >> result.dateActive;
        *this >> result.ip;
        *this >> result.region;
        break;
    default:
        break;
    }

    webAuthorizationValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccessPointRule &accessPointRuleValue)
{
    TLAccessPointRule result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccessPointRule:
        *this >> result.phonePrefixRules;
        *this >> result.dcId;
        *this >> result.ips;
        break;
    default:
        break;
    }

    accessPointRuleValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountPassword &accountPasswordValue)
{
    TLAccountPassword result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountPassword:
        *this >> result.flags;
        if (result.flags & 1 << 2) {
            *this >> result.currentAlgo;
        }
        if (result.flags & 1 << 2) {
            *this >> result.srpB;
        }
        if (result.flags & 1 << 2) {
            *this >> result.srpId;
        }
        if (result.flags & 1 << 3) {
            *this >> result.hint;
        }
        if (result.flags & 1 << 4) {
            *this >> result.emailUnconfirmedPattern;
        }
        *this >> result.newAlgo;
        *this >> result.newSecureAlgo;
        *this >> result.secureRandom;
        break;
    default:
        break;
    }

    accountPasswordValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountPasswordInputSettings &accountPasswordInputSettingsValue)
{
    TLAccountPasswordInputSettings result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountPasswordInputSettings:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.newAlgo;
        }
        if (result.flags & 1 << 0) {
            *this >> result.newPasswordHash;
        }
        if (result.flags & 1 << 0) {
            *this >> result.hint;
        }
        if (result.flags & 1 << 1) {
            *this >> result.email;
        }
        if (result.flags & 1 << 2) {
            *this >> result.newSecureSettings;
        }
        break;
    default:
        break;
    }

    accountPasswordInputSettingsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountPasswordSettings &accountPasswordSettingsValue)
{
    TLAccountPasswordSettings result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountPasswordSettings:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.email;
        }
        if (result.flags & 1 << 1) {
            *this >> result.secureSettings;
        }
        break;
    default:
        break;
    }

    accountPasswordSettingsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAuthorization &authorizationValue)
{
    TLAuthorization result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Authorization:
        *this >> result.flags;
        *this >> result.hash;
        *this >> result.deviceModel;
        *this >> result.platform;
        *this >> result.systemVersion;
        *this >> result.apiId;
        *this >> result.appName;
        *this >> result.appVersion;
        *this >> result.dateCreated;
        *this >> result.dateActive;
        *this >> result.ip;
        *this >> result.country;
        *this >> result.region;
        break;
    default:
        break;
    }

    authorizationValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLCdnConfig &cdnConfigValue)
{
    TLCdnConfig result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::CdnConfig:
        *this >> result.publicKeys;
        break;
    default:
        break;
    }

    cdnConfigValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChannelAdminLogEventsFilter &channelAdminLogEventsFilterValue)
{
    TLChannelAdminLogEventsFilter result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChannelAdminLogEventsFilter:
        *this >> result.flags;
        break;
    default:
        break;
    }

    channelAdminLogEventsFilterValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChannelAdminRights &channelAdminRightsValue)
{
    TLChannelAdminRights result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChannelAdminRights:
        *this >> result.flags;
        break;
    default:
        break;
    }

    channelAdminRightsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChannelBannedRights &channelBannedRightsValue)
{
    TLChannelBannedRights result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChannelBannedRights:
        *this >> result.flags;
        *this >> result.untilDate;
        break;
    default:
        break;
    }

    channelBannedRightsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChannelMessagesFilter &channelMessagesFilterValue)
{
    TLChannelMessagesFilter result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChannelMessagesFilterEmpty:
        break;
    case TLValue::ChannelMessagesFilter:
        *this >> result.flags;
        *this >> result.ranges;
        break;
    default:
        break;
    }

    channelMessagesFilterValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChannelParticipant &channelParticipantValue)
{
    TLChannelParticipant result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChannelParticipant:
        *this >> result.userId;
        *this >> result.date;
        break;
    case TLValue::ChannelParticipantSelf:
        *this >> result.userId;
        *this >> result.inviterId;
        *this >> result.date;
        break;
    case TLValue::ChannelParticipantCreator:
        *this >> result.userId;
        break;
    case TLValue::ChannelParticipantAdmin:
        *this >> result.flags;
        *this >> result.userId;
        *this >> result.inviterId;
        *this >> result.promotedBy;
        *this >> result.date;
        *this >> result.adminRights;
        break;
    case TLValue::ChannelParticipantBanned:
        *this >> result.flags;
        *this >> result.userId;
        *this >> result.kickedBy;
        *this >> result.date;
        *this >> result.bannedRights;
        break;
    default:
        break;
    }

    channelParticipantValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChatPhoto &chatPhotoValue)
{
    TLChatPhoto result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChatPhotoEmpty:
        break;
    case TLValue::ChatPhoto:
        *this >> result.photoSmall;
        *this >> result.photoBig;
        break;
    default:
        break;
    }

    chatPhotoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactStatus &contactStatusValue)
{
    TLContactStatus result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactStatus:
        *this >> result.userId;
        *this >> result.status;
        break;
    default:
        break;
    }

    contactStatusValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDcOption &dcOptionValue)
{
    TLDcOption result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DcOption:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.ipAddress;
        *this >> result.port;
        if (result.flags & 1 << 10) {
            *this >> result.secret;
        }
        break;
    default:
        break;
    }

    dcOptionValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDialogPeer &dialogPeerValue)
{
    TLDialogPeer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DialogPeer:
        *this >> result.peer;
        break;
    default:
        break;
    }

    dialogPeerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDocumentAttribute &documentAttributeValue)
{
    TLDocumentAttribute result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DocumentAttributeImageSize:
        *this >> result.w;
        *this >> result.h;
        break;
    case TLValue::DocumentAttributeAnimated:
    case TLValue::DocumentAttributeHasStickers:
        break;
    case TLValue::DocumentAttributeSticker:
        *this >> result.flags;
        *this >> result.alt;
        *this >> result.stickerset;
        if (result.flags & 1 << 0) {
            *this >> result.maskCoords;
        }
        break;
    case TLValue::DocumentAttributeVideo:
        *this >> result.flags;
        *this >> result.duration;
        *this >> result.w;
        *this >> result.h;
        break;
    case TLValue::DocumentAttributeAudio:
        *this >> result.flags;
        *this >> result.duration;
        if (result.flags & 1 << 0) {
            *this >> result.title;
        }
        if (result.flags & 1 << 1) {
            *this >> result.performer;
        }
        if (result.flags & 1 << 2) {
            *this >> result.waveform;
        }
        break;
    case TLValue::DocumentAttributeFilename:
        *this >> result.fileName;
        break;
    default:
        break;
    }

    documentAttributeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDraftMessage &draftMessageValue)
{
    TLDraftMessage result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DraftMessageEmpty:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.date;
        }
        break;
    case TLValue::DraftMessage:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.replyToMsgId;
        }
        *this >> result.message;
        if (result.flags & 1 << 3) {
            *this >> result.entities;
        }
        *this >> result.date;
        break;
    default:
        break;
    }

    draftMessageValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpConfigSimple &helpConfigSimpleValue)
{
    TLHelpConfigSimple result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpConfigSimple:
        *this >> result.date;
        *this >> result.expires;
        *this >> result.rules;
        break;
    default:
        break;
    }

    helpConfigSimpleValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpDeepLinkInfo &helpDeepLinkInfoValue)
{
    TLHelpDeepLinkInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpDeepLinkInfoEmpty:
        break;
    case TLValue::HelpDeepLinkInfo:
        *this >> result.flags;
        *this >> result.message;
        if (result.flags & 1 << 1) {
            *this >> result.entities;
        }
        break;
    default:
        break;
    }

    helpDeepLinkInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpTermsOfService &helpTermsOfServiceValue)
{
    TLHelpTermsOfService result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpTermsOfService:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.text;
        *this >> result.entities;
        if (result.flags & 1 << 1) {
            *this >> result.minAgeConfirm;
        }
        break;
    default:
        break;
    }

    helpTermsOfServiceValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpTermsOfServiceUpdate &helpTermsOfServiceUpdateValue)
{
    TLHelpTermsOfServiceUpdate result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpTermsOfServiceUpdateEmpty:
        *this >> result.expires;
        break;
    case TLValue::HelpTermsOfServiceUpdate:
        *this >> result.expires;
        *this >> result.termsOfService;
        break;
    default:
        break;
    }

    helpTermsOfServiceUpdateValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpUserInfo &helpUserInfoValue)
{
    TLHelpUserInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpUserInfoEmpty:
        break;
    case TLValue::HelpUserInfo:
        *this >> result.message;
        *this >> result.entities;
        *this >> result.author;
        *this >> result.date;
        break;
    default:
        break;
    }

    helpUserInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputChatPhoto &inputChatPhotoValue)
{
    TLInputChatPhoto result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputChatPhotoEmpty:
        break;
    case TLValue::InputChatUploadedPhoto:
        *this >> result.file;
        break;
    case TLValue::InputChatPhoto:
        *this >> result.id;
        break;
    default:
        break;
    }

    inputChatPhotoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputDialogPeer &inputDialogPeerValue)
{
    TLInputDialogPeer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputDialogPeer:
        *this >> result.peer;
        break;
    default:
        break;
    }

    inputDialogPeerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputGame &inputGameValue)
{
    TLInputGame result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputGameID:
        *this >> result.id;
        *this >> result.accessHash;
        break;
    case TLValue::InputGameShortName:
        *this >> result.botId;
        *this >> result.shortName;
        break;
    default:
        break;
    }

    inputGameValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputNotifyPeer &inputNotifyPeerValue)
{
    TLInputNotifyPeer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputNotifyPeer:
        *this >> result.peer;
        break;
    case TLValue::InputNotifyUsers:
    case TLValue::InputNotifyChats:
    case TLValue::InputNotifyBroadcasts:
        break;
    default:
        break;
    }

    inputNotifyPeerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputPaymentCredentials &inputPaymentCredentialsValue)
{
    TLInputPaymentCredentials result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputPaymentCredentialsSaved:
        *this >> result.id;
        *this >> result.tmpPassword;
        break;
    case TLValue::InputPaymentCredentials:
        *this >> result.flags;
        *this >> result.data;
        break;
    case TLValue::InputPaymentCredentialsApplePay:
        *this >> result.paymentData;
        break;
    case TLValue::InputPaymentCredentialsAndroidPay:
        *this >> result.paymentToken;
        *this >> result.googleTransactionId;
        break;
    default:
        break;
    }

    inputPaymentCredentialsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputPrivacyRule &inputPrivacyRuleValue)
{
    TLInputPrivacyRule result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputPrivacyValueAllowContacts:
    case TLValue::InputPrivacyValueAllowAll:
    case TLValue::InputPrivacyValueDisallowContacts:
    case TLValue::InputPrivacyValueDisallowAll:
        break;
    case TLValue::InputPrivacyValueAllowUsers:
    case TLValue::InputPrivacyValueDisallowUsers:
        *this >> result.users;
        break;
    default:
        break;
    }

    inputPrivacyRuleValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputSecureValue &inputSecureValueValue)
{
    TLInputSecureValue result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputSecureValue:
        *this >> result.flags;
        *this >> result.type;
        if (result.flags & 1 << 0) {
            *this >> result.data;
        }
        if (result.flags & 1 << 1) {
            *this >> result.frontSide;
        }
        if (result.flags & 1 << 2) {
            *this >> result.reverseSide;
        }
        if (result.flags & 1 << 3) {
            *this >> result.selfie;
        }
        if (result.flags & 1 << 6) {
            *this >> result.translation;
        }
        if (result.flags & 1 << 4) {
            *this >> result.files;
        }
        if (result.flags & 1 << 5) {
            *this >> result.plainData;
        }
        break;
    default:
        break;
    }

    inputSecureValueValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputStickerSetItem &inputStickerSetItemValue)
{
    TLInputStickerSetItem result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputStickerSetItem:
        *this >> result.flags;
        *this >> result.document;
        *this >> result.emoji;
        if (result.flags & 1 << 0) {
            *this >> result.maskCoords;
        }
        break;
    default:
        break;
    }

    inputStickerSetItemValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputWebDocument &inputWebDocumentValue)
{
    TLInputWebDocument result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputWebDocument:
        *this >> result.url;
        *this >> result.size;
        *this >> result.mimeType;
        *this >> result.attributes;
        break;
    default:
        break;
    }

    inputWebDocumentValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInvoice &invoiceValue)
{
    TLInvoice result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Invoice:
        *this >> result.flags;
        *this >> result.currency;
        *this >> result.prices;
        break;
    default:
        break;
    }

    invoiceValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLKeyboardButton &keyboardButtonValue)
{
    TLKeyboardButton result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::KeyboardButton:
    case TLValue::KeyboardButtonRequestPhone:
    case TLValue::KeyboardButtonRequestGeoLocation:
    case TLValue::KeyboardButtonGame:
    case TLValue::KeyboardButtonBuy:
        *this >> result.text;
        break;
    case TLValue::KeyboardButtonUrl:
        *this >> result.text;
        *this >> result.url;
        break;
    case TLValue::KeyboardButtonCallback:
        *this >> result.text;
        *this >> result.data;
        break;
    case TLValue::KeyboardButtonSwitchInline:
        *this >> result.flags;
        *this >> result.text;
        *this >> result.query;
        break;
    default:
        break;
    }

    keyboardButtonValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLKeyboardButtonRow &keyboardButtonRowValue)
{
    TLKeyboardButtonRow result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::KeyboardButtonRow:
        *this >> result.buttons;
        break;
    default:
        break;
    }

    keyboardButtonRowValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLLangPackDifference &langPackDifferenceValue)
{
    TLLangPackDifference result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::LangPackDifference:
        *this >> result.langCode;
        *this >> result.fromVersion;
        *this >> result.version;
        *this >> result.strings;
        break;
    default:
        break;
    }

    langPackDifferenceValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLLangPackLanguage &langPackLanguageValue)
{
    TLLangPackLanguage result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::LangPackLanguage:
        *this >> result.flags;
        *this >> result.name;
        *this >> result.nativeName;
        *this >> result.langCode;
        if (result.flags & 1 << 1) {
            *this >> result.baseLangCode;
        }
        *this >> result.pluralCode;
        *this >> result.stringsCount;
        *this >> result.translatedCount;
        *this >> result.translationsUrl;
        break;
    default:
        break;
    }

    langPackLanguageValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessageFwdHeader &messageFwdHeaderValue)
{
    TLMessageFwdHeader result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessageFwdHeader:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.fromId;
        }
        *this >> result.date;
        if (result.flags & 1 << 1) {
            *this >> result.channelId;
        }
        if (result.flags & 1 << 2) {
            *this >> result.channelPost;
        }
        if (result.flags & 1 << 3) {
            *this >> result.postAuthor;
        }
        if (result.flags & 1 << 4) {
            *this >> result.savedFromPeer;
        }
        if (result.flags & 1 << 4) {
            *this >> result.savedFromMsgId;
        }
        break;
    default:
        break;
    }

    messageFwdHeaderValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesBotCallbackAnswer &messagesBotCallbackAnswerValue)
{
    TLMessagesBotCallbackAnswer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesBotCallbackAnswer:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.message;
        }
        if (result.flags & 1 << 2) {
            *this >> result.url;
        }
        *this >> result.cacheTime;
        break;
    default:
        break;
    }

    messagesBotCallbackAnswerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesFilter &messagesFilterValue)
{
    TLMessagesFilter result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputMessagesFilterEmpty:
    case TLValue::InputMessagesFilterPhotos:
    case TLValue::InputMessagesFilterVideo:
    case TLValue::InputMessagesFilterPhotoVideo:
    case TLValue::InputMessagesFilterDocument:
    case TLValue::InputMessagesFilterUrl:
    case TLValue::InputMessagesFilterGif:
    case TLValue::InputMessagesFilterVoice:
    case TLValue::InputMessagesFilterMusic:
    case TLValue::InputMessagesFilterChatPhotos:
    case TLValue::InputMessagesFilterRoundVoice:
    case TLValue::InputMessagesFilterRoundVideo:
    case TLValue::InputMessagesFilterMyMentions:
    case TLValue::InputMessagesFilterGeo:
    case TLValue::InputMessagesFilterContacts:
        break;
    case TLValue::InputMessagesFilterPhoneCalls:
        *this >> result.flags;
        break;
    default:
        break;
    }

    messagesFilterValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesMessageEditData &messagesMessageEditDataValue)
{
    TLMessagesMessageEditData result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesMessageEditData:
        *this >> result.flags;
        break;
    default:
        break;
    }

    messagesMessageEditDataValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLNotifyPeer &notifyPeerValue)
{
    TLNotifyPeer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::NotifyPeer:
        *this >> result.peer;
        break;
    case TLValue::NotifyUsers:
    case TLValue::NotifyChats:
    case TLValue::NotifyBroadcasts:
        break;
    default:
        break;
    }

    notifyPeerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPageCaption &pageCaptionValue)
{
    TLPageCaption result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PageCaption:
        *this >> *result.text;
        *this >> *result.credit;
        break;
    default:
        break;
    }

    pageCaptionValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPageTableCell &pageTableCellValue)
{
    TLPageTableCell result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PageTableCell:
        *this >> result.flags;
        if (result.flags & 1 << 7) {
            *this >> result.text;
        }
        if (result.flags & 1 << 1) {
            *this >> result.colspan;
        }
        if (result.flags & 1 << 2) {
            *this >> result.rowspan;
        }
        break;
    default:
        break;
    }

    pageTableCellValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPageTableRow &pageTableRowValue)
{
    TLPageTableRow result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PageTableRow:
        *this >> result.cells;
        break;
    default:
        break;
    }

    pageTableRowValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPaymentRequestedInfo &paymentRequestedInfoValue)
{
    TLPaymentRequestedInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PaymentRequestedInfo:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.name;
        }
        if (result.flags & 1 << 1) {
            *this >> result.phone;
        }
        if (result.flags & 1 << 2) {
            *this >> result.email;
        }
        if (result.flags & 1 << 3) {
            *this >> result.shippingAddress;
        }
        break;
    default:
        break;
    }

    paymentRequestedInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPaymentsSavedInfo &paymentsSavedInfoValue)
{
    TLPaymentsSavedInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PaymentsSavedInfo:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.savedInfo;
        }
        break;
    default:
        break;
    }

    paymentsSavedInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPaymentsValidatedRequestedInfo &paymentsValidatedRequestedInfoValue)
{
    TLPaymentsValidatedRequestedInfo result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PaymentsValidatedRequestedInfo:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.id;
        }
        if (result.flags & 1 << 1) {
            *this >> result.shippingOptions;
        }
        break;
    default:
        break;
    }

    paymentsValidatedRequestedInfoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPeerSettings &peerSettingsValue)
{
    TLPeerSettings result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PeerSettings:
        *this >> result.flags;
        break;
    default:
        break;
    }

    peerSettingsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhoneCallProtocol &phoneCallProtocolValue)
{
    TLPhoneCallProtocol result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhoneCallProtocol:
        *this >> result.flags;
        *this >> result.minLayer;
        *this >> result.maxLayer;
        break;
    default:
        break;
    }

    phoneCallProtocolValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhoto &photoValue)
{
    TLPhoto result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhotoEmpty:
        *this >> result.id;
        break;
    case TLValue::Photo:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.fileReference;
        *this >> result.date;
        *this >> result.sizes;
        break;
    default:
        break;
    }

    photoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPoll &pollValue)
{
    TLPoll result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Poll:
        *this >> result.id;
        *this >> result.flags;
        *this >> result.question;
        *this >> result.answers;
        break;
    default:
        break;
    }

    pollValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPollAnswerVoters &pollAnswerVotersValue)
{
    TLPollAnswerVoters result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PollAnswerVoters:
        *this >> result.flags;
        *this >> result.option;
        *this >> result.voters;
        break;
    default:
        break;
    }

    pollAnswerVotersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPollResults &pollResultsValue)
{
    TLPollResults result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PollResults:
        *this >> result.flags;
        if (result.flags & 1 << 1) {
            *this >> result.results;
        }
        if (result.flags & 1 << 2) {
            *this >> result.totalVoters;
        }
        break;
    default:
        break;
    }

    pollResultsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLReplyMarkup &replyMarkupValue)
{
    TLReplyMarkup result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ReplyKeyboardHide:
    case TLValue::ReplyKeyboardForceReply:
        *this >> result.flags;
        break;
    case TLValue::ReplyKeyboardMarkup:
        *this >> result.flags;
        *this >> result.rows;
        break;
    case TLValue::ReplyInlineMarkup:
        *this >> result.rows;
        break;
    default:
        break;
    }

    replyMarkupValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureRequiredType &secureRequiredTypeValue)
{
    TLSecureRequiredType result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureRequiredType:
        *this >> result.flags;
        *this >> result.type;
        break;
    case TLValue::SecureRequiredTypeOneOf:
        *this >> result.types;
        break;
    default:
        break;
    }

    secureRequiredTypeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureValue &secureValueValue)
{
    TLSecureValue result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureValue:
        *this >> result.flags;
        *this >> result.type;
        if (result.flags & 1 << 0) {
            *this >> result.data;
        }
        if (result.flags & 1 << 1) {
            *this >> result.frontSide;
        }
        if (result.flags & 1 << 2) {
            *this >> result.reverseSide;
        }
        if (result.flags & 1 << 3) {
            *this >> result.selfie;
        }
        if (result.flags & 1 << 6) {
            *this >> result.translation;
        }
        if (result.flags & 1 << 4) {
            *this >> result.files;
        }
        if (result.flags & 1 << 5) {
            *this >> result.plainData;
        }
        *this >> result.hash;
        break;
    default:
        break;
    }

    secureValueValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureValueError &secureValueErrorValue)
{
    TLSecureValueError result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureValueErrorData:
        *this >> result.type;
        *this >> result.dataHash;
        *this >> result.field;
        *this >> result.text;
        break;
    case TLValue::SecureValueErrorFrontSide:
    case TLValue::SecureValueErrorReverseSide:
    case TLValue::SecureValueErrorSelfie:
    case TLValue::SecureValueErrorFile:
    case TLValue::SecureValueErrorTranslationFile:
        *this >> result.type;
        *this >> result.byteArrayFileHash;
        *this >> result.text;
        break;
    case TLValue::SecureValueErrorFiles:
    case TLValue::SecureValueErrorTranslationFiles:
        *this >> result.type;
        *this >> result.byteArrayFileHashVector;
        *this >> result.text;
        break;
    case TLValue::SecureValueError:
        *this >> result.type;
        *this >> result.hash;
        *this >> result.text;
        break;
    default:
        break;
    }

    secureValueErrorValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLSecureValueHash &secureValueHashValue)
{
    TLSecureValueHash result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::SecureValueHash:
        *this >> result.type;
        *this >> result.hash;
        break;
    default:
        break;
    }

    secureValueHashValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLStickerSet &stickerSetValue)
{
    TLStickerSet result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::StickerSet:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.installedDate;
        }
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.title;
        *this >> result.shortName;
        *this >> result.count;
        *this >> result.hash;
        break;
    default:
        break;
    }

    stickerSetValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLUser &userValue)
{
    TLUser result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::UserEmpty:
        *this >> result.id;
        break;
    case TLValue::User:
        *this >> result.flags;
        *this >> result.id;
        if (result.flags & 1 << 0) {
            *this >> result.accessHash;
        }
        if (result.flags & 1 << 1) {
            *this >> result.firstName;
        }
        if (result.flags & 1 << 2) {
            *this >> result.lastName;
        }
        if (result.flags & 1 << 3) {
            *this >> result.username;
        }
        if (result.flags & 1 << 4) {
            *this >> result.phone;
        }
        if (result.flags & 1 << 5) {
            *this >> result.photo;
        }
        if (result.flags & 1 << 6) {
            *this >> result.status;
        }
        if (result.flags & 1 << 14) {
            *this >> result.botInfoVersion;
        }
        if (result.flags & 1 << 18) {
            *this >> result.restrictionReason;
        }
        if (result.flags & 1 << 19) {
            *this >> result.botInlinePlaceholder;
        }
        if (result.flags & 1 << 22) {
            *this >> result.langCode;
        }
        break;
    default:
        break;
    }

    userValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLWebDocument &webDocumentValue)
{
    TLWebDocument result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::WebDocument:
        *this >> result.url;
        *this >> result.accessHash;
        *this >> result.size;
        *this >> result.mimeType;
        *this >> result.attributes;
        break;
    case TLValue::WebDocumentNoProxy:
        *this >> result.url;
        *this >> result.size;
        *this >> result.mimeType;
        *this >> result.attributes;
        break;
    default:
        break;
    }

    webDocumentValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountAuthorizationForm &accountAuthorizationFormValue)
{
    TLAccountAuthorizationForm result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountAuthorizationForm:
        *this >> result.flags;
        *this >> result.requiredTypes;
        *this >> result.values;
        *this >> result.errors;
        *this >> result.users;
        if (result.flags & 1 << 0) {
            *this >> result.privacyPolicyUrl;
        }
        break;
    default:
        break;
    }

    accountAuthorizationFormValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountAuthorizations &accountAuthorizationsValue)
{
    TLAccountAuthorizations result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountAuthorizations:
        *this >> result.authorizations;
        break;
    default:
        break;
    }

    accountAuthorizationsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountPrivacyRules &accountPrivacyRulesValue)
{
    TLAccountPrivacyRules result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountPrivacyRules:
        *this >> result.rules;
        *this >> result.users;
        break;
    default:
        break;
    }

    accountPrivacyRulesValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAccountWebAuthorizations &accountWebAuthorizationsValue)
{
    TLAccountWebAuthorizations result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AccountWebAuthorizations:
        *this >> result.authorizations;
        *this >> result.users;
        break;
    default:
        break;
    }

    accountWebAuthorizationsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAuthAuthorization &authAuthorizationValue)
{
    TLAuthAuthorization result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AuthAuthorization:
        *this >> result.flags;
        if (result.flags & 1 << 0) {
            *this >> result.tmpSessions;
        }
        *this >> result.user;
        break;
    default:
        break;
    }

    authAuthorizationValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLAuthSentCode &authSentCodeValue)
{
    TLAuthSentCode result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::AuthSentCode:
        *this >> result.flags;
        *this >> result.type;
        *this >> result.phoneCodeHash;
        if (result.flags & 1 << 1) {
            *this >> result.nextType;
        }
        if (result.flags & 1 << 2) {
            *this >> result.timeout;
        }
        if (result.flags & 1 << 3) {
            *this >> result.termsOfService;
        }
        break;
    default:
        break;
    }

    authSentCodeValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLBotInlineMessage &botInlineMessageValue)
{
    TLBotInlineMessage result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::BotInlineMessageMediaAuto:
    case TLValue::BotInlineMessageText:
        *this >> result.flags;
        *this >> result.message;
        if (result.flags & 1 << 1) {
            *this >> result.entities;
        }
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    case TLValue::BotInlineMessageMediaGeo:
        *this >> result.flags;
        *this >> result.geo;
        *this >> result.period;
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    case TLValue::BotInlineMessageMediaVenue:
        *this >> result.flags;
        *this >> result.geo;
        *this >> result.title;
        *this >> result.address;
        *this >> result.provider;
        *this >> result.venueId;
        *this >> result.venueType;
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    case TLValue::BotInlineMessageMediaContact:
        *this >> result.flags;
        *this >> result.phoneNumber;
        *this >> result.firstName;
        *this >> result.lastName;
        *this >> result.vcard;
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    default:
        break;
    }

    botInlineMessageValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChannelsChannelParticipant &channelsChannelParticipantValue)
{
    TLChannelsChannelParticipant result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChannelsChannelParticipant:
        *this >> result.participant;
        *this >> result.users;
        break;
    default:
        break;
    }

    channelsChannelParticipantValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChannelsChannelParticipants &channelsChannelParticipantsValue)
{
    TLChannelsChannelParticipants result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChannelsChannelParticipants:
        *this >> result.count;
        *this >> result.participants;
        *this >> result.users;
        break;
    case TLValue::ChannelsChannelParticipantsNotModified:
        break;
    default:
        break;
    }

    channelsChannelParticipantsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChat &chatValue)
{
    TLChat result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChatEmpty:
        *this >> result.id;
        break;
    case TLValue::Chat:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.title;
        *this >> result.photo;
        *this >> result.participantsCount;
        *this >> result.date;
        *this >> result.version;
        if (result.flags & 1 << 6) {
            *this >> result.migratedTo;
        }
        break;
    case TLValue::ChatForbidden:
        *this >> result.id;
        *this >> result.title;
        break;
    case TLValue::Channel:
        *this >> result.flags;
        *this >> result.id;
        if (result.flags & 1 << 13) {
            *this >> result.accessHash;
        }
        *this >> result.title;
        if (result.flags & 1 << 6) {
            *this >> result.username;
        }
        *this >> result.photo;
        *this >> result.date;
        *this >> result.version;
        if (result.flags & 1 << 9) {
            *this >> result.restrictionReason;
        }
        if (result.flags & 1 << 14) {
            *this >> result.adminRights;
        }
        if (result.flags & 1 << 15) {
            *this >> result.bannedRights;
        }
        if (result.flags & 1 << 17) {
            *this >> result.participantsCount;
        }
        break;
    case TLValue::ChannelForbidden:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.title;
        if (result.flags & 1 << 16) {
            *this >> result.untilDate;
        }
        break;
    default:
        break;
    }

    chatValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChatFull &chatFullValue)
{
    TLChatFull result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChatFull:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.participants;
        if (result.flags & 1 << 2) {
            *this >> result.chatPhoto;
        }
        *this >> result.notifySettings;
        *this >> result.exportedInvite;
        if (result.flags & 1 << 3) {
            *this >> result.botInfo;
        }
        if (result.flags & 1 << 6) {
            *this >> result.pinnedMsgId;
        }
        break;
    case TLValue::ChannelFull:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.about;
        if (result.flags & 1 << 0) {
            *this >> result.participantsCount;
        }
        if (result.flags & 1 << 1) {
            *this >> result.adminsCount;
        }
        if (result.flags & 1 << 2) {
            *this >> result.kickedCount;
        }
        if (result.flags & 1 << 2) {
            *this >> result.bannedCount;
        }
        if (result.flags & 1 << 13) {
            *this >> result.onlineCount;
        }
        *this >> result.readInboxMaxId;
        *this >> result.readOutboxMaxId;
        *this >> result.unreadCount;
        *this >> result.chatPhoto;
        *this >> result.notifySettings;
        *this >> result.exportedInvite;
        *this >> result.botInfo;
        if (result.flags & 1 << 4) {
            *this >> result.migratedFromChatId;
        }
        if (result.flags & 1 << 4) {
            *this >> result.migratedFromMaxId;
        }
        if (result.flags & 1 << 5) {
            *this >> result.pinnedMsgId;
        }
        if (result.flags & 1 << 8) {
            *this >> result.stickerset;
        }
        if (result.flags & 1 << 9) {
            *this >> result.availableMinId;
        }
        break;
    default:
        break;
    }

    chatFullValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLChatInvite &chatInviteValue)
{
    TLChatInvite result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ChatInviteAlready:
        *this >> result.chat;
        break;
    case TLValue::ChatInvite:
        *this >> result.flags;
        *this >> result.title;
        *this >> result.photo;
        *this >> result.participantsCount;
        if (result.flags & 1 << 4) {
            *this >> result.participants;
        }
        break;
    default:
        break;
    }

    chatInviteValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLConfig &configValue)
{
    TLConfig result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Config:
        *this >> result.flags;
        *this >> result.date;
        *this >> result.expires;
        *this >> result.testMode;
        *this >> result.thisDc;
        *this >> result.dcOptions;
        *this >> result.dcTxtDomainName;
        *this >> result.chatSizeMax;
        *this >> result.megagroupSizeMax;
        *this >> result.forwardedCountMax;
        *this >> result.onlineUpdatePeriodMs;
        *this >> result.offlineBlurTimeoutMs;
        *this >> result.offlineIdleTimeoutMs;
        *this >> result.onlineCloudTimeoutMs;
        *this >> result.notifyCloudDelayMs;
        *this >> result.notifyDefaultDelayMs;
        *this >> result.pushChatPeriodMs;
        *this >> result.pushChatLimit;
        *this >> result.savedGifsLimit;
        *this >> result.editTimeLimit;
        *this >> result.revokeTimeLimit;
        *this >> result.revokePmTimeLimit;
        *this >> result.ratingEDecay;
        *this >> result.stickersRecentLimit;
        *this >> result.stickersFavedLimit;
        *this >> result.channelsReadMediaPeriod;
        if (result.flags & 1 << 0) {
            *this >> result.tmpSessions;
        }
        *this >> result.pinnedDialogsCountMax;
        *this >> result.callReceiveTimeoutMs;
        *this >> result.callRingTimeoutMs;
        *this >> result.callConnectTimeoutMs;
        *this >> result.callPacketTimeoutMs;
        *this >> result.meUrlPrefix;
        if (result.flags & 1 << 7) {
            *this >> result.autoupdateUrlPrefix;
        }
        if (result.flags & 1 << 9) {
            *this >> result.gifSearchUsername;
        }
        if (result.flags & 1 << 10) {
            *this >> result.venueSearchUsername;
        }
        if (result.flags & 1 << 11) {
            *this >> result.imgSearchUsername;
        }
        if (result.flags & 1 << 12) {
            *this >> result.staticMapsProvider;
        }
        *this >> result.captionLengthMax;
        *this >> result.messageLengthMax;
        *this >> result.webfileDcId;
        if (result.flags & 1 << 2) {
            *this >> result.suggestedLangCode;
        }
        if (result.flags & 1 << 2) {
            *this >> result.langPackVersion;
        }
        if (result.flags & 1 << 2) {
            *this >> result.baseLangPackVersion;
        }
        break;
    default:
        break;
    }

    configValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactsBlocked &contactsBlockedValue)
{
    TLContactsBlocked result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactsBlocked:
        *this >> result.blocked;
        *this >> result.users;
        break;
    case TLValue::ContactsBlockedSlice:
        *this >> result.count;
        *this >> result.blocked;
        *this >> result.users;
        break;
    default:
        break;
    }

    contactsBlockedValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactsContacts &contactsContactsValue)
{
    TLContactsContacts result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactsContactsNotModified:
        break;
    case TLValue::ContactsContacts:
        *this >> result.contacts;
        *this >> result.savedCount;
        *this >> result.users;
        break;
    default:
        break;
    }

    contactsContactsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactsFound &contactsFoundValue)
{
    TLContactsFound result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactsFound:
        *this >> result.myResults;
        *this >> result.results;
        *this >> result.chats;
        *this >> result.users;
        break;
    default:
        break;
    }

    contactsFoundValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactsImportedContacts &contactsImportedContactsValue)
{
    TLContactsImportedContacts result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactsImportedContacts:
        *this >> result.imported;
        *this >> result.popularInvites;
        *this >> result.retryContacts;
        *this >> result.users;
        break;
    default:
        break;
    }

    contactsImportedContactsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactsLink &contactsLinkValue)
{
    TLContactsLink result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactsLink:
        *this >> result.myLink;
        *this >> result.foreignLink;
        *this >> result.user;
        break;
    default:
        break;
    }

    contactsLinkValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactsResolvedPeer &contactsResolvedPeerValue)
{
    TLContactsResolvedPeer result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactsResolvedPeer:
        *this >> result.peer;
        *this >> result.chats;
        *this >> result.users;
        break;
    default:
        break;
    }

    contactsResolvedPeerValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLContactsTopPeers &contactsTopPeersValue)
{
    TLContactsTopPeers result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::ContactsTopPeersNotModified:
    case TLValue::ContactsTopPeersDisabled:
        break;
    case TLValue::ContactsTopPeers:
        *this >> result.categories;
        *this >> result.chats;
        *this >> result.users;
        break;
    default:
        break;
    }

    contactsTopPeersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDialog &dialogValue)
{
    TLDialog result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Dialog:
        *this >> result.flags;
        *this >> result.peer;
        *this >> result.topMessage;
        *this >> result.readInboxMaxId;
        *this >> result.readOutboxMaxId;
        *this >> result.unreadCount;
        *this >> result.unreadMentionsCount;
        *this >> result.notifySettings;
        if (result.flags & 1 << 0) {
            *this >> result.pts;
        }
        if (result.flags & 1 << 1) {
            *this >> result.draft;
        }
        break;
    default:
        break;
    }

    dialogValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLDocument &documentValue)
{
    TLDocument result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::DocumentEmpty:
        *this >> result.id;
        break;
    case TLValue::Document:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.fileReference;
        *this >> result.date;
        *this >> result.mimeType;
        *this >> result.size;
        *this >> result.thumb;
        *this >> result.dcId;
        *this >> result.attributes;
        break;
    default:
        break;
    }

    documentValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLFoundGif &foundGifValue)
{
    TLFoundGif result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::FoundGif:
        *this >> result.url;
        *this >> result.thumbUrl;
        *this >> result.contentUrl;
        *this >> result.contentType;
        *this >> result.w;
        *this >> result.h;
        break;
    case TLValue::FoundGifCached:
        *this >> result.url;
        *this >> result.photo;
        *this >> result.document;
        break;
    default:
        break;
    }

    foundGifValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLGame &gameValue)
{
    TLGame result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::Game:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.shortName;
        *this >> result.title;
        *this >> result.description;
        *this >> result.photo;
        if (result.flags & 1 << 0) {
            *this >> result.document;
        }
        break;
    default:
        break;
    }

    gameValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpAppUpdate &helpAppUpdateValue)
{
    TLHelpAppUpdate result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpAppUpdate:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.version;
        *this >> result.text;
        *this >> result.entities;
        if (result.flags & 1 << 1) {
            *this >> result.document;
        }
        if (result.flags & 1 << 2) {
            *this >> result.url;
        }
        break;
    case TLValue::HelpNoAppUpdate:
        break;
    default:
        break;
    }

    helpAppUpdateValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpProxyData &helpProxyDataValue)
{
    TLHelpProxyData result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpProxyDataEmpty:
        *this >> result.expires;
        break;
    case TLValue::HelpProxyDataPromo:
        *this >> result.expires;
        *this >> result.peer;
        *this >> result.chats;
        *this >> result.users;
        break;
    default:
        break;
    }

    helpProxyDataValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpSupport &helpSupportValue)
{
    TLHelpSupport result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpSupport:
        *this >> result.phoneNumber;
        *this >> result.user;
        break;
    default:
        break;
    }

    helpSupportValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputBotInlineMessage &inputBotInlineMessageValue)
{
    TLInputBotInlineMessage result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputBotInlineMessageMediaAuto:
    case TLValue::InputBotInlineMessageText:
        *this >> result.flags;
        *this >> result.message;
        if (result.flags & 1 << 1) {
            *this >> result.entities;
        }
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    case TLValue::InputBotInlineMessageMediaGeo:
        *this >> result.flags;
        *this >> result.geoPoint;
        *this >> result.period;
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    case TLValue::InputBotInlineMessageMediaVenue:
        *this >> result.flags;
        *this >> result.geoPoint;
        *this >> result.title;
        *this >> result.address;
        *this >> result.provider;
        *this >> result.venueId;
        *this >> result.venueType;
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    case TLValue::InputBotInlineMessageMediaContact:
        *this >> result.flags;
        *this >> result.phoneNumber;
        *this >> result.firstName;
        *this >> result.lastName;
        *this >> result.vcard;
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    case TLValue::InputBotInlineMessageGame:
        *this >> result.flags;
        if (result.flags & 1 << 2) {
            *this >> result.replyMarkup;
        }
        break;
    default:
        break;
    }

    inputBotInlineMessageValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputBotInlineResult &inputBotInlineResultValue)
{
    TLInputBotInlineResult result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputBotInlineResult:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.type;
        if (result.flags & 1 << 1) {
            *this >> result.title;
        }
        if (result.flags & 1 << 2) {
            *this >> result.description;
        }
        if (result.flags & 1 << 3) {
            *this >> result.url;
        }
        if (result.flags & 1 << 4) {
            *this >> result.thumb;
        }
        if (result.flags & 1 << 5) {
            *this >> result.content;
        }
        *this >> result.sendMessage;
        break;
    case TLValue::InputBotInlineResultPhoto:
        *this >> result.id;
        *this >> result.type;
        *this >> result.photo;
        *this >> result.sendMessage;
        break;
    case TLValue::InputBotInlineResultDocument:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.type;
        if (result.flags & 1 << 1) {
            *this >> result.title;
        }
        if (result.flags & 1 << 2) {
            *this >> result.description;
        }
        *this >> result.document;
        *this >> result.sendMessage;
        break;
    case TLValue::InputBotInlineResultGame:
        *this >> result.id;
        *this >> result.shortName;
        *this >> result.sendMessage;
        break;
    default:
        break;
    }

    inputBotInlineResultValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputMedia &inputMediaValue)
{
    TLInputMedia result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputMediaEmpty:
        break;
    case TLValue::InputMediaUploadedPhoto:
        *this >> result.flags;
        *this >> result.file;
        if (result.flags & 1 << 0) {
            *this >> result.stickers;
        }
        if (result.flags & 1 << 1) {
            *this >> result.ttlSeconds;
        }
        break;
    case TLValue::InputMediaPhoto:
        *this >> result.flags;
        *this >> result.inputPhotoId;
        if (result.flags & 1 << 0) {
            *this >> result.ttlSeconds;
        }
        break;
    case TLValue::InputMediaGeoPoint:
        *this >> result.geoPoint;
        break;
    case TLValue::InputMediaContact:
        *this >> result.phoneNumber;
        *this >> result.firstName;
        *this >> result.lastName;
        *this >> result.vcard;
        break;
    case TLValue::InputMediaUploadedDocument:
        *this >> result.flags;
        *this >> result.file;
        if (result.flags & 1 << 2) {
            *this >> result.thumb;
        }
        *this >> result.mimeType;
        *this >> result.attributes;
        if (result.flags & 1 << 0) {
            *this >> result.stickers;
        }
        if (result.flags & 1 << 1) {
            *this >> result.ttlSeconds;
        }
        break;
    case TLValue::InputMediaDocument:
        *this >> result.flags;
        *this >> result.inputDocumentId;
        if (result.flags & 1 << 0) {
            *this >> result.ttlSeconds;
        }
        break;
    case TLValue::InputMediaVenue:
        *this >> result.geoPoint;
        *this >> result.title;
        *this >> result.address;
        *this >> result.provider;
        *this >> result.venueId;
        *this >> result.venueType;
        break;
    case TLValue::InputMediaGifExternal:
        *this >> result.url;
        *this >> result.q;
        break;
    case TLValue::InputMediaPhotoExternal:
    case TLValue::InputMediaDocumentExternal:
        *this >> result.flags;
        *this >> result.url;
        if (result.flags & 1 << 0) {
            *this >> result.ttlSeconds;
        }
        break;
    case TLValue::InputMediaGame:
        *this >> result.inputGameId;
        break;
    case TLValue::InputMediaInvoice:
        *this >> result.flags;
        *this >> result.title;
        *this >> result.description;
        if (result.flags & 1 << 0) {
            *this >> result.photo;
        }
        *this >> result.invoice;
        *this >> result.payload;
        *this >> result.provider;
        *this >> result.providerData;
        *this >> result.startParam;
        break;
    case TLValue::InputMediaGeoLive:
        *this >> result.flags;
        *this >> result.geoPoint;
        if (result.flags & 1 << 1) {
            *this >> result.period;
        }
        break;
    case TLValue::InputMediaPoll:
        *this >> result.poll;
        break;
    default:
        break;
    }

    inputMediaValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLInputSingleMedia &inputSingleMediaValue)
{
    TLInputSingleMedia result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::InputSingleMedia:
        *this >> result.flags;
        *this >> result.media;
        *this >> result.randomId;
        *this >> result.message;
        if (result.flags & 1 << 0) {
            *this >> result.entities;
        }
        break;
    default:
        break;
    }

    inputSingleMediaValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessageAction &messageActionValue)
{
    TLMessageAction result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessageActionEmpty:
    case TLValue::MessageActionChatDeletePhoto:
    case TLValue::MessageActionPinMessage:
    case TLValue::MessageActionHistoryClear:
    case TLValue::MessageActionScreenshotTaken:
    case TLValue::MessageActionContactSignUp:
        break;
    case TLValue::MessageActionChatCreate:
        *this >> result.title;
        *this >> result.users;
        break;
    case TLValue::MessageActionChatEditTitle:
    case TLValue::MessageActionChannelCreate:
        *this >> result.title;
        break;
    case TLValue::MessageActionChatEditPhoto:
        *this >> result.photo;
        break;
    case TLValue::MessageActionChatAddUser:
        *this >> result.users;
        break;
    case TLValue::MessageActionChatDeleteUser:
        *this >> result.userId;
        break;
    case TLValue::MessageActionChatJoinedByLink:
        *this >> result.inviterId;
        break;
    case TLValue::MessageActionChatMigrateTo:
        *this >> result.channelId;
        break;
    case TLValue::MessageActionChannelMigrateFrom:
        *this >> result.title;
        *this >> result.chatId;
        break;
    case TLValue::MessageActionGameScore:
        *this >> result.gameId;
        *this >> result.score;
        break;
    case TLValue::MessageActionPaymentSentMe:
        *this >> result.flags;
        *this >> result.currency;
        *this >> result.totalAmount;
        *this >> result.payload;
        if (result.flags & 1 << 0) {
            *this >> result.info;
        }
        if (result.flags & 1 << 1) {
            *this >> result.shippingOptionId;
        }
        *this >> result.charge;
        break;
    case TLValue::MessageActionPaymentSent:
        *this >> result.currency;
        *this >> result.totalAmount;
        break;
    case TLValue::MessageActionPhoneCall:
        *this >> result.flags;
        *this >> result.callId;
        if (result.flags & 1 << 0) {
            *this >> result.reason;
        }
        if (result.flags & 1 << 1) {
            *this >> result.duration;
        }
        break;
    case TLValue::MessageActionCustomAction:
        *this >> result.message;
        break;
    case TLValue::MessageActionBotAllowed:
        *this >> result.domain;
        break;
    case TLValue::MessageActionSecureValuesSentMe:
        *this >> result.values;
        *this >> result.credentials;
        break;
    case TLValue::MessageActionSecureValuesSent:
        *this >> result.types;
        break;
    default:
        break;
    }

    messageActionValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesAllStickers &messagesAllStickersValue)
{
    TLMessagesAllStickers result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesAllStickersNotModified:
        break;
    case TLValue::MessagesAllStickers:
        *this >> result.hash;
        *this >> result.sets;
        break;
    default:
        break;
    }

    messagesAllStickersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesChatFull &messagesChatFullValue)
{
    TLMessagesChatFull result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesChatFull:
        *this >> result.fullChat;
        *this >> result.chats;
        *this >> result.users;
        break;
    default:
        break;
    }

    messagesChatFullValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesChats &messagesChatsValue)
{
    TLMessagesChats result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesChats:
        *this >> result.chats;
        break;
    case TLValue::MessagesChatsSlice:
        *this >> result.count;
        *this >> result.chats;
        break;
    default:
        break;
    }

    messagesChatsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesFavedStickers &messagesFavedStickersValue)
{
    TLMessagesFavedStickers result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesFavedStickersNotModified:
        break;
    case TLValue::MessagesFavedStickers:
        *this >> result.hash;
        *this >> result.packs;
        *this >> result.stickers;
        break;
    default:
        break;
    }

    messagesFavedStickersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesFoundGifs &messagesFoundGifsValue)
{
    TLMessagesFoundGifs result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesFoundGifs:
        *this >> result.nextOffset;
        *this >> result.results;
        break;
    default:
        break;
    }

    messagesFoundGifsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesHighScores &messagesHighScoresValue)
{
    TLMessagesHighScores result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesHighScores:
        *this >> result.scores;
        *this >> result.users;
        break;
    default:
        break;
    }

    messagesHighScoresValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesRecentStickers &messagesRecentStickersValue)
{
    TLMessagesRecentStickers result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesRecentStickersNotModified:
        break;
    case TLValue::MessagesRecentStickers:
        *this >> result.hash;
        *this >> result.packs;
        *this >> result.stickers;
        *this >> result.dates;
        break;
    default:
        break;
    }

    messagesRecentStickersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesSavedGifs &messagesSavedGifsValue)
{
    TLMessagesSavedGifs result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesSavedGifsNotModified:
        break;
    case TLValue::MessagesSavedGifs:
        *this >> result.hash;
        *this >> result.gifs;
        break;
    default:
        break;
    }

    messagesSavedGifsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesStickerSet &messagesStickerSetValue)
{
    TLMessagesStickerSet result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesStickerSet:
        *this >> result.set;
        *this >> result.packs;
        *this >> result.documents;
        break;
    default:
        break;
    }

    messagesStickerSetValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesStickers &messagesStickersValue)
{
    TLMessagesStickers result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesStickersNotModified:
        break;
    case TLValue::MessagesStickers:
        *this >> result.hash;
        *this >> result.stickers;
        break;
    default:
        break;
    }

    messagesStickersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPaymentsPaymentForm &paymentsPaymentFormValue)
{
    TLPaymentsPaymentForm result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PaymentsPaymentForm:
        *this >> result.flags;
        *this >> result.botId;
        *this >> result.invoice;
        *this >> result.providerId;
        *this >> result.url;
        if (result.flags & 1 << 4) {
            *this >> result.nativeProvider;
        }
        if (result.flags & 1 << 4) {
            *this >> result.nativeParams;
        }
        if (result.flags & 1 << 0) {
            *this >> result.savedInfo;
        }
        if (result.flags & 1 << 1) {
            *this >> result.savedCredentials;
        }
        *this >> result.users;
        break;
    default:
        break;
    }

    paymentsPaymentFormValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPaymentsPaymentReceipt &paymentsPaymentReceiptValue)
{
    TLPaymentsPaymentReceipt result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PaymentsPaymentReceipt:
        *this >> result.flags;
        *this >> result.date;
        *this >> result.botId;
        *this >> result.invoice;
        *this >> result.providerId;
        if (result.flags & 1 << 0) {
            *this >> result.info;
        }
        if (result.flags & 1 << 1) {
            *this >> result.shipping;
        }
        *this >> result.currency;
        *this >> result.totalAmount;
        *this >> result.credentialsTitle;
        *this >> result.users;
        break;
    default:
        break;
    }

    paymentsPaymentReceiptValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhoneCall &phoneCallValue)
{
    TLPhoneCall result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhoneCallEmpty:
        *this >> result.id;
        break;
    case TLValue::PhoneCallWaiting:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.date;
        *this >> result.adminId;
        *this >> result.participantId;
        *this >> result.protocol;
        if (result.flags & 1 << 0) {
            *this >> result.receiveDate;
        }
        break;
    case TLValue::PhoneCallRequested:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.date;
        *this >> result.adminId;
        *this >> result.participantId;
        *this >> result.gAHash;
        *this >> result.protocol;
        break;
    case TLValue::PhoneCallAccepted:
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.date;
        *this >> result.adminId;
        *this >> result.participantId;
        *this >> result.gB;
        *this >> result.protocol;
        break;
    case TLValue::PhoneCall:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.accessHash;
        *this >> result.date;
        *this >> result.adminId;
        *this >> result.participantId;
        *this >> result.gAOrB;
        *this >> result.keyFingerprint;
        *this >> result.protocol;
        *this >> result.connection;
        *this >> result.alternativeConnections;
        *this >> result.startDate;
        break;
    case TLValue::PhoneCallDiscarded:
        *this >> result.flags;
        *this >> result.id;
        if (result.flags & 1 << 0) {
            *this >> result.reason;
        }
        if (result.flags & 1 << 1) {
            *this >> result.duration;
        }
        break;
    default:
        break;
    }

    phoneCallValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhonePhoneCall &phonePhoneCallValue)
{
    TLPhonePhoneCall result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhonePhoneCall:
        *this >> result.phoneCall;
        *this >> result.users;
        break;
    default:
        break;
    }

    phonePhoneCallValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhotosPhoto &photosPhotoValue)
{
    TLPhotosPhoto result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhotosPhoto:
        *this >> result.photo;
        *this >> result.users;
        break;
    default:
        break;
    }

    photosPhotoValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLPhotosPhotos &photosPhotosValue)
{
    TLPhotosPhotos result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::PhotosPhotos:
        *this >> result.photos;
        *this >> result.users;
        break;
    case TLValue::PhotosPhotosSlice:
        *this >> result.count;
        *this >> result.photos;
        *this >> result.users;
        break;
    default:
        break;
    }

    photosPhotosValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLStickerSetCovered &stickerSetCoveredValue)
{
    TLStickerSetCovered result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::StickerSetCovered:
        *this >> result.set;
        *this >> result.cover;
        break;
    case TLValue::StickerSetMultiCovered:
        *this >> result.set;
        *this >> result.covers;
        break;
    default:
        break;
    }

    stickerSetCoveredValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLUserFull &userFullValue)
{
    TLUserFull result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::UserFull:
        *this >> result.flags;
        *this >> result.user;
        if (result.flags & 1 << 1) {
            *this >> result.about;
        }
        *this >> result.link;
        if (result.flags & 1 << 2) {
            *this >> result.profilePhoto;
        }
        *this >> result.notifySettings;
        if (result.flags & 1 << 3) {
            *this >> result.botInfo;
        }
        if (result.flags & 1 << 6) {
            *this >> result.pinnedMsgId;
        }
        *this >> result.commonChatsCount;
        break;
    default:
        break;
    }

    userFullValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLBotInlineResult &botInlineResultValue)
{
    TLBotInlineResult result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::BotInlineResult:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.type;
        if (result.flags & 1 << 1) {
            *this >> result.title;
        }
        if (result.flags & 1 << 2) {
            *this >> result.description;
        }
        if (result.flags & 1 << 3) {
            *this >> result.url;
        }
        if (result.flags & 1 << 4) {
            *this >> result.thumb;
        }
        if (result.flags & 1 << 5) {
            *this >> result.content;
        }
        *this >> result.sendMessage;
        break;
    case TLValue::BotInlineMediaResult:
        *this >> result.flags;
        *this >> result.id;
        *this >> result.type;
        if (result.flags & 1 << 0) {
            *this >> result.photo;
        }
        if (result.flags & 1 << 1) {
            *this >> result.document;
        }
        if (result.flags & 1 << 2) {
            *this >> result.title;
        }
        if (result.flags & 1 << 3) {
            *this >> result.description;
        }
        *this >> result.sendMessage;
        break;
    default:
        break;
    }

    botInlineResultValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesArchivedStickers &messagesArchivedStickersValue)
{
    TLMessagesArchivedStickers result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesArchivedStickers:
        *this >> result.count;
        *this >> result.sets;
        break;
    default:
        break;
    }

    messagesArchivedStickersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesBotResults &messagesBotResultsValue)
{
    TLMessagesBotResults result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesBotResults:
        *this >> result.flags;
        *this >> result.queryId;
        if (result.flags & 1 << 1) {
            *this >> result.nextOffset;
        }
        if (result.flags & 1 << 2) {
            *this >> result.switchPm;
        }
        *this >> result.results;
        *this >> result.cacheTime;
        *this >> result.users;
        break;
    default:
        break;
    }

    messagesBotResultsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesFeaturedStickers &messagesFeaturedStickersValue)
{
    TLMessagesFeaturedStickers result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesFeaturedStickersNotModified:
        break;
    case TLValue::MessagesFeaturedStickers:
        *this >> result.hash;
        *this >> result.sets;
        *this >> result.unread;
        break;
    default:
        break;
    }

    messagesFeaturedStickersValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesFoundStickerSets &messagesFoundStickerSetsValue)
{
    TLMessagesFoundStickerSets result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesFoundStickerSetsNotModified:
        break;
    case TLValue::MessagesFoundStickerSets:
        *this >> result.hash;
        *this >> result.sets;
        break;
    default:
        break;
    }

    messagesFoundStickerSetsValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLMessagesStickerSetInstallResult &messagesStickerSetInstallResultValue)
{
    TLMessagesStickerSetInstallResult result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::MessagesStickerSetInstallResultSuccess:
        break;
    case TLValue::MessagesStickerSetInstallResultArchive:
        *this >> result.sets;
        break;
    default:
        break;
    }

    messagesStickerSetInstallResultValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLRecentMeUrl &recentMeUrlValue)
{
    TLRecentMeUrl result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::RecentMeUrlUnknown:
        *this >> result.url;
        break;
    case TLValue::RecentMeUrlUser:
        *this >> result.url;
        *this >> result.userId;
        break;
    case TLValue::RecentMeUrlChat:
        *this >> result.url;
        *this >> result.chatId;
        break;
    case TLValue::RecentMeUrlChatInvite:
        *this >> result.url;
        *this >> result.chatInvite;
        break;
    case TLValue::RecentMeUrlStickerSet:
        *this >> result.url;
        *this >> result.set;
        break;
    default:
        break;
    }

    recentMeUrlValue = result;

    return *this;
}

CTelegramStream &CTelegramStream::operator>>(TLHelpRecentMeUrls &helpRecentMeUrlsValue)
{
    TLHelpRecentMeUrls result;

    *this >> result.tlType;

    switch (result.tlType) {
    case TLValue::HelpRecentMeUrls:
        *this >> result.urls;
        *this >> result.chats;
        *this >> result.users;
        break;
    default:
        break;
    }

    helpRecentMeUrlsValue = result;

    return *this;
}

// End of generated read operators implementation

// Generated write operators implementation
CTelegramStream &CTelegramStream::operator<<(const TLAccountDaysTTL &accountDaysTTLValue)
{
    *this << accountDaysTTLValue.tlType;

    switch (accountDaysTTLValue.tlType) {
    case TLValue::AccountDaysTTL:
        *this << accountDaysTTLValue.days;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLChannelParticipantsFilter &channelParticipantsFilterValue)
{
    *this << channelParticipantsFilterValue.tlType;

    switch (channelParticipantsFilterValue.tlType) {
    case TLValue::ChannelParticipantsRecent:
    case TLValue::ChannelParticipantsAdmins:
    case TLValue::ChannelParticipantsBots:
        break;
    case TLValue::ChannelParticipantsKicked:
    case TLValue::ChannelParticipantsBanned:
    case TLValue::ChannelParticipantsSearch:
        *this << channelParticipantsFilterValue.q;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLDataJSON &dataJSONValue)
{
    *this << dataJSONValue.tlType;

    switch (dataJSONValue.tlType) {
    case TLValue::DataJSON:
        *this << dataJSONValue.data;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInlineBotSwitchPM &inlineBotSwitchPMValue)
{
    *this << inlineBotSwitchPMValue.tlType;

    switch (inlineBotSwitchPMValue.tlType) {
    case TLValue::InlineBotSwitchPM:
        *this << inlineBotSwitchPMValue.text;
        *this << inlineBotSwitchPMValue.startParam;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputBotInlineMessageID &inputBotInlineMessageIDValue)
{
    *this << inputBotInlineMessageIDValue.tlType;

    switch (inputBotInlineMessageIDValue.tlType) {
    case TLValue::InputBotInlineMessageID:
        *this << inputBotInlineMessageIDValue.dcId;
        *this << inputBotInlineMessageIDValue.id;
        *this << inputBotInlineMessageIDValue.accessHash;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputChannel &inputChannelValue)
{
    *this << inputChannelValue.tlType;

    switch (inputChannelValue.tlType) {
    case TLValue::InputChannelEmpty:
        break;
    case TLValue::InputChannel:
        *this << inputChannelValue.channelId;
        *this << inputChannelValue.accessHash;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputCheckPasswordSRP &inputCheckPasswordSRPValue)
{
    *this << inputCheckPasswordSRPValue.tlType;

    switch (inputCheckPasswordSRPValue.tlType) {
    case TLValue::InputCheckPasswordEmpty:
        break;
    case TLValue::InputCheckPasswordSRP:
        *this << inputCheckPasswordSRPValue.srpId;
        *this << inputCheckPasswordSRPValue.A;
        *this << inputCheckPasswordSRPValue.M1;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputContact &inputContactValue)
{
    *this << inputContactValue.tlType;

    switch (inputContactValue.tlType) {
    case TLValue::InputPhoneContact:
        *this << inputContactValue.clientId;
        *this << inputContactValue.phone;
        *this << inputContactValue.firstName;
        *this << inputContactValue.lastName;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputDocument &inputDocumentValue)
{
    *this << inputDocumentValue.tlType;

    switch (inputDocumentValue.tlType) {
    case TLValue::InputDocumentEmpty:
        break;
    case TLValue::InputDocument:
        *this << inputDocumentValue.id;
        *this << inputDocumentValue.accessHash;
        *this << inputDocumentValue.fileReference;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputEncryptedChat &inputEncryptedChatValue)
{
    *this << inputEncryptedChatValue.tlType;

    switch (inputEncryptedChatValue.tlType) {
    case TLValue::InputEncryptedChat:
        *this << inputEncryptedChatValue.chatId;
        *this << inputEncryptedChatValue.accessHash;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputEncryptedFile &inputEncryptedFileValue)
{
    *this << inputEncryptedFileValue.tlType;

    switch (inputEncryptedFileValue.tlType) {
    case TLValue::InputEncryptedFileEmpty:
        break;
    case TLValue::InputEncryptedFileUploaded:
        *this << inputEncryptedFileValue.id;
        *this << inputEncryptedFileValue.parts;
        *this << inputEncryptedFileValue.md5Checksum;
        *this << inputEncryptedFileValue.keyFingerprint;
        break;
    case TLValue::InputEncryptedFile:
        *this << inputEncryptedFileValue.id;
        *this << inputEncryptedFileValue.accessHash;
        break;
    case TLValue::InputEncryptedFileBigUploaded:
        *this << inputEncryptedFileValue.id;
        *this << inputEncryptedFileValue.parts;
        *this << inputEncryptedFileValue.keyFingerprint;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputFile &inputFileValue)
{
    *this << inputFileValue.tlType;

    switch (inputFileValue.tlType) {
    case TLValue::InputFile:
        *this << inputFileValue.id;
        *this << inputFileValue.parts;
        *this << inputFileValue.name;
        *this << inputFileValue.md5Checksum;
        break;
    case TLValue::InputFileBig:
        *this << inputFileValue.id;
        *this << inputFileValue.parts;
        *this << inputFileValue.name;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputFileLocation &inputFileLocationValue)
{
    *this << inputFileLocationValue.tlType;

    switch (inputFileLocationValue.tlType) {
    case TLValue::InputFileLocation:
        *this << inputFileLocationValue.volumeId;
        *this << inputFileLocationValue.localId;
        *this << inputFileLocationValue.secret;
        *this << inputFileLocationValue.fileReference;
        break;
    case TLValue::InputEncryptedFileLocation:
    case TLValue::InputSecureFileLocation:
        *this << inputFileLocationValue.id;
        *this << inputFileLocationValue.accessHash;
        break;
    case TLValue::InputDocumentFileLocation:
        *this << inputFileLocationValue.id;
        *this << inputFileLocationValue.accessHash;
        *this << inputFileLocationValue.fileReference;
        break;
    case TLValue::InputTakeoutFileLocation:
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputGeoPoint &inputGeoPointValue)
{
    *this << inputGeoPointValue.tlType;

    switch (inputGeoPointValue.tlType) {
    case TLValue::InputGeoPointEmpty:
        break;
    case TLValue::InputGeoPoint:
        *this << inputGeoPointValue.latitude;
        *this << inputGeoPointValue.longitude;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputMessage &inputMessageValue)
{
    *this << inputMessageValue.tlType;

    switch (inputMessageValue.tlType) {
    case TLValue::InputMessageID:
    case TLValue::InputMessageReplyTo:
        *this << inputMessageValue.id;
        break;
    case TLValue::InputMessagePinned:
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputPeer &inputPeerValue)
{
    *this << inputPeerValue.tlType;

    switch (inputPeerValue.tlType) {
    case TLValue::InputPeerEmpty:
    case TLValue::InputPeerSelf:
        break;
    case TLValue::InputPeerChat:
        *this << inputPeerValue.chatId;
        break;
    case TLValue::InputPeerUser:
        *this << inputPeerValue.userId;
        *this << inputPeerValue.accessHash;
        break;
    case TLValue::InputPeerChannel:
        *this << inputPeerValue.channelId;
        *this << inputPeerValue.accessHash;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputPeerNotifySettings &inputPeerNotifySettingsValue)
{
    *this << inputPeerNotifySettingsValue.tlType;

    switch (inputPeerNotifySettingsValue.tlType) {
    case TLValue::InputPeerNotifySettings:
        *this << inputPeerNotifySettingsValue.flags;
        if (inputPeerNotifySettingsValue.flags & 1 << 0) {
            *this << inputPeerNotifySettingsValue.showPreviews;
        }
        if (inputPeerNotifySettingsValue.flags & 1 << 1) {
            *this << inputPeerNotifySettingsValue.silent;
        }
        if (inputPeerNotifySettingsValue.flags & 1 << 2) {
            *this << inputPeerNotifySettingsValue.muteUntil;
        }
        if (inputPeerNotifySettingsValue.flags & 1 << 3) {
            *this << inputPeerNotifySettingsValue.sound;
        }
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputPhoneCall &inputPhoneCallValue)
{
    *this << inputPhoneCallValue.tlType;

    switch (inputPhoneCallValue.tlType) {
    case TLValue::InputPhoneCall:
        *this << inputPhoneCallValue.id;
        *this << inputPhoneCallValue.accessHash;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputPhoto &inputPhotoValue)
{
    *this << inputPhotoValue.tlType;

    switch (inputPhotoValue.tlType) {
    case TLValue::InputPhotoEmpty:
        break;
    case TLValue::InputPhoto:
        *this << inputPhotoValue.id;
        *this << inputPhotoValue.accessHash;
        *this << inputPhotoValue.fileReference;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputPrivacyKey &inputPrivacyKeyValue)
{
    *this << inputPrivacyKeyValue.tlType;

    switch (inputPrivacyKeyValue.tlType) {
    case TLValue::InputPrivacyKeyStatusTimestamp:
    case TLValue::InputPrivacyKeyChatInvite:
    case TLValue::InputPrivacyKeyPhoneCall:
    case TLValue::InputPrivacyKeyPhoneP2P:
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputSecureFile &inputSecureFileValue)
{
    *this << inputSecureFileValue.tlType;

    switch (inputSecureFileValue.tlType) {
    case TLValue::InputSecureFileUploaded:
        *this << inputSecureFileValue.id;
        *this << inputSecureFileValue.parts;
        *this << inputSecureFileValue.md5Checksum;
        *this << inputSecureFileValue.fileHash;
        *this << inputSecureFileValue.secret;
        break;
    case TLValue::InputSecureFile:
        *this << inputSecureFileValue.id;
        *this << inputSecureFileValue.accessHash;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputStickerSet &inputStickerSetValue)
{
    *this << inputStickerSetValue.tlType;

    switch (inputStickerSetValue.tlType) {
    case TLValue::InputStickerSetEmpty:
        break;
    case TLValue::InputStickerSetID:
        *this << inputStickerSetValue.id;
        *this << inputStickerSetValue.accessHash;
        break;
    case TLValue::InputStickerSetShortName:
        *this << inputStickerSetValue.shortName;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputStickeredMedia &inputStickeredMediaValue)
{
    *this << inputStickeredMediaValue.tlType;

    switch (inputStickeredMediaValue.tlType) {
    case TLValue::InputStickeredMediaPhoto:
        *this << inputStickeredMediaValue.inputPhotoId;
        break;
    case TLValue::InputStickeredMediaDocument:
        *this << inputStickeredMediaValue.inputDocumentId;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputUser &inputUserValue)
{
    *this << inputUserValue.tlType;

    switch (inputUserValue.tlType) {
    case TLValue::InputUserEmpty:
    case TLValue::InputUserSelf:
        break;
    case TLValue::InputUser:
        *this << inputUserValue.userId;
        *this << inputUserValue.accessHash;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputWebFileLocation &inputWebFileLocationValue)
{
    *this << inputWebFileLocationValue.tlType;

    switch (inputWebFileLocationValue.tlType) {
    case TLValue::InputWebFileLocation:
        *this << inputWebFileLocationValue.url;
        *this << inputWebFileLocationValue.accessHash;
        break;
    case TLValue::InputWebFileGeoPointLocation:
        *this << inputWebFileLocationValue.geoPoint;
        *this << inputWebFileLocationValue.accessHash;
        *this << inputWebFileLocationValue.w;
        *this << inputWebFileLocationValue.h;
        *this << inputWebFileLocationValue.zoom;
        *this << inputWebFileLocationValue.scale;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLLabeledPrice &labeledPriceValue)
{
    *this << labeledPriceValue.tlType;

    switch (labeledPriceValue.tlType) {
    case TLValue::LabeledPrice:
        *this << labeledPriceValue.label;
        *this << labeledPriceValue.amount;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLMaskCoords &maskCoordsValue)
{
    *this << maskCoordsValue.tlType;

    switch (maskCoordsValue.tlType) {
    case TLValue::MaskCoords:
        *this << maskCoordsValue.n;
        *this << maskCoordsValue.x;
        *this << maskCoordsValue.y;
        *this << maskCoordsValue.zoom;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLMessageEntity &messageEntityValue)
{
    *this << messageEntityValue.tlType;

    switch (messageEntityValue.tlType) {
    case TLValue::MessageEntityUnknown:
    case TLValue::MessageEntityMention:
    case TLValue::MessageEntityHashtag:
    case TLValue::MessageEntityBotCommand:
    case TLValue::MessageEntityUrl:
    case TLValue::MessageEntityEmail:
    case TLValue::MessageEntityBold:
    case TLValue::MessageEntityItalic:
    case TLValue::MessageEntityCode:
    case TLValue::MessageEntityPhone:
    case TLValue::MessageEntityCashtag:
        *this << messageEntityValue.offset;
        *this << messageEntityValue.length;
        break;
    case TLValue::MessageEntityPre:
        *this << messageEntityValue.offset;
        *this << messageEntityValue.length;
        *this << messageEntityValue.language;
        break;
    case TLValue::MessageEntityTextUrl:
        *this << messageEntityValue.offset;
        *this << messageEntityValue.length;
        *this << messageEntityValue.url;
        break;
    case TLValue::MessageEntityMentionName:
        *this << messageEntityValue.offset;
        *this << messageEntityValue.length;
        *this << messageEntityValue.quint32UserId;
        break;
    case TLValue::InputMessageEntityMentionName:
        *this << messageEntityValue.offset;
        *this << messageEntityValue.length;
        *this << messageEntityValue.inputUserUserId;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLMessageRange &messageRangeValue)
{
    *this << messageRangeValue.tlType;

    switch (messageRangeValue.tlType) {
    case TLValue::MessageRange:
        *this << messageRangeValue.minId;
        *this << messageRangeValue.maxId;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLPasswordKdfAlgo &passwordKdfAlgoValue)
{
    *this << passwordKdfAlgoValue.tlType;

    switch (passwordKdfAlgoValue.tlType) {
    case TLValue::PasswordKdfAlgoUnknown:
        break;
    case TLValue::PasswordKdfAlgoSHA256SHA256PBKDF2HMACSHA512iter100000SHA256ModPow:
        *this << passwordKdfAlgoValue.salt1;
        *this << passwordKdfAlgoValue.salt2;
        *this << passwordKdfAlgoValue.g;
        *this << passwordKdfAlgoValue.p;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLPhoneCallDiscardReason &phoneCallDiscardReasonValue)
{
    *this << phoneCallDiscardReasonValue.tlType;

    switch (phoneCallDiscardReasonValue.tlType) {
    case TLValue::PhoneCallDiscardReasonMissed:
    case TLValue::PhoneCallDiscardReasonDisconnect:
    case TLValue::PhoneCallDiscardReasonHangup:
    case TLValue::PhoneCallDiscardReasonBusy:
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLPollAnswer &pollAnswerValue)
{
    *this << pollAnswerValue.tlType;

    switch (pollAnswerValue.tlType) {
    case TLValue::PollAnswer:
        *this << pollAnswerValue.text;
        *this << pollAnswerValue.option;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLPostAddress &postAddressValue)
{
    *this << postAddressValue.tlType;

    switch (postAddressValue.tlType) {
    case TLValue::PostAddress:
        *this << postAddressValue.streetLine1;
        *this << postAddressValue.streetLine2;
        *this << postAddressValue.city;
        *this << postAddressValue.state;
        *this << postAddressValue.countryIso2;
        *this << postAddressValue.postCode;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLReportReason &reportReasonValue)
{
    *this << reportReasonValue.tlType;

    switch (reportReasonValue.tlType) {
    case TLValue::InputReportReasonSpam:
    case TLValue::InputReportReasonViolence:
    case TLValue::InputReportReasonPornography:
    case TLValue::InputReportReasonChildAbuse:
    case TLValue::InputReportReasonCopyright:
        break;
    case TLValue::InputReportReasonOther:
        *this << reportReasonValue.text;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSecureCredentialsEncrypted &secureCredentialsEncryptedValue)
{
    *this << secureCredentialsEncryptedValue.tlType;

    switch (secureCredentialsEncryptedValue.tlType) {
    case TLValue::SecureCredentialsEncrypted:
        *this << secureCredentialsEncryptedValue.data;
        *this << secureCredentialsEncryptedValue.hash;
        *this << secureCredentialsEncryptedValue.secret;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSecureData &secureDataValue)
{
    *this << secureDataValue.tlType;

    switch (secureDataValue.tlType) {
    case TLValue::SecureData:
        *this << secureDataValue.data;
        *this << secureDataValue.dataHash;
        *this << secureDataValue.secret;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSecurePasswordKdfAlgo &securePasswordKdfAlgoValue)
{
    *this << securePasswordKdfAlgoValue.tlType;

    switch (securePasswordKdfAlgoValue.tlType) {
    case TLValue::SecurePasswordKdfAlgoUnknown:
        break;
    case TLValue::SecurePasswordKdfAlgoPBKDF2HMACSHA512iter100000:
    case TLValue::SecurePasswordKdfAlgoSHA512:
        *this << securePasswordKdfAlgoValue.salt;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSecurePlainData &securePlainDataValue)
{
    *this << securePlainDataValue.tlType;

    switch (securePlainDataValue.tlType) {
    case TLValue::SecurePlainPhone:
        *this << securePlainDataValue.phone;
        break;
    case TLValue::SecurePlainEmail:
        *this << securePlainDataValue.email;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSecureSecretSettings &secureSecretSettingsValue)
{
    *this << secureSecretSettingsValue.tlType;

    switch (secureSecretSettingsValue.tlType) {
    case TLValue::SecureSecretSettings:
        *this << secureSecretSettingsValue.secureAlgo;
        *this << secureSecretSettingsValue.secureSecret;
        *this << secureSecretSettingsValue.secureSecretId;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSecureValueType &secureValueTypeValue)
{
    *this << secureValueTypeValue.tlType;

    switch (secureValueTypeValue.tlType) {
    case TLValue::SecureValueTypePersonalDetails:
    case TLValue::SecureValueTypePassport:
    case TLValue::SecureValueTypeDriverLicense:
    case TLValue::SecureValueTypeIdentityCard:
    case TLValue::SecureValueTypeInternalPassport:
    case TLValue::SecureValueTypeAddress:
    case TLValue::SecureValueTypeUtilityBill:
    case TLValue::SecureValueTypeBankStatement:
    case TLValue::SecureValueTypeRentalAgreement:
    case TLValue::SecureValueTypePassportRegistration:
    case TLValue::SecureValueTypeTemporaryRegistration:
    case TLValue::SecureValueTypePhone:
    case TLValue::SecureValueTypeEmail:
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSendMessageAction &sendMessageActionValue)
{
    *this << sendMessageActionValue.tlType;

    switch (sendMessageActionValue.tlType) {
    case TLValue::SendMessageTypingAction:
    case TLValue::SendMessageCancelAction:
    case TLValue::SendMessageRecordVideoAction:
    case TLValue::SendMessageRecordAudioAction:
    case TLValue::SendMessageGeoLocationAction:
    case TLValue::SendMessageChooseContactAction:
    case TLValue::SendMessageGamePlayAction:
    case TLValue::SendMessageRecordRoundAction:
        break;
    case TLValue::SendMessageUploadVideoAction:
    case TLValue::SendMessageUploadAudioAction:
    case TLValue::SendMessageUploadPhotoAction:
    case TLValue::SendMessageUploadDocumentAction:
    case TLValue::SendMessageUploadRoundAction:
        *this << sendMessageActionValue.progress;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLShippingOption &shippingOptionValue)
{
    *this << shippingOptionValue.tlType;

    switch (shippingOptionValue.tlType) {
    case TLValue::ShippingOption:
        *this << shippingOptionValue.id;
        *this << shippingOptionValue.title;
        *this << shippingOptionValue.prices;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLTopPeerCategory &topPeerCategoryValue)
{
    *this << topPeerCategoryValue.tlType;

    switch (topPeerCategoryValue.tlType) {
    case TLValue::TopPeerCategoryBotsPM:
    case TLValue::TopPeerCategoryBotsInline:
    case TLValue::TopPeerCategoryCorrespondents:
    case TLValue::TopPeerCategoryGroups:
    case TLValue::TopPeerCategoryChannels:
    case TLValue::TopPeerCategoryPhoneCalls:
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLAccountPasswordInputSettings &accountPasswordInputSettingsValue)
{
    *this << accountPasswordInputSettingsValue.tlType;

    switch (accountPasswordInputSettingsValue.tlType) {
    case TLValue::AccountPasswordInputSettings:
        *this << accountPasswordInputSettingsValue.flags;
        if (accountPasswordInputSettingsValue.flags & 1 << 0) {
            *this << accountPasswordInputSettingsValue.newAlgo;
        }
        if (accountPasswordInputSettingsValue.flags & 1 << 0) {
            *this << accountPasswordInputSettingsValue.newPasswordHash;
        }
        if (accountPasswordInputSettingsValue.flags & 1 << 0) {
            *this << accountPasswordInputSettingsValue.hint;
        }
        if (accountPasswordInputSettingsValue.flags & 1 << 1) {
            *this << accountPasswordInputSettingsValue.email;
        }
        if (accountPasswordInputSettingsValue.flags & 1 << 2) {
            *this << accountPasswordInputSettingsValue.newSecureSettings;
        }
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLChannelAdminLogEventsFilter &channelAdminLogEventsFilterValue)
{
    *this << channelAdminLogEventsFilterValue.tlType;

    switch (channelAdminLogEventsFilterValue.tlType) {
    case TLValue::ChannelAdminLogEventsFilter:
        *this << channelAdminLogEventsFilterValue.flags;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLChannelAdminRights &channelAdminRightsValue)
{
    *this << channelAdminRightsValue.tlType;

    switch (channelAdminRightsValue.tlType) {
    case TLValue::ChannelAdminRights:
        *this << channelAdminRightsValue.flags;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLChannelBannedRights &channelBannedRightsValue)
{
    *this << channelBannedRightsValue.tlType;

    switch (channelBannedRightsValue.tlType) {
    case TLValue::ChannelBannedRights:
        *this << channelBannedRightsValue.flags;
        *this << channelBannedRightsValue.untilDate;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLChannelMessagesFilter &channelMessagesFilterValue)
{
    *this << channelMessagesFilterValue.tlType;

    switch (channelMessagesFilterValue.tlType) {
    case TLValue::ChannelMessagesFilterEmpty:
        break;
    case TLValue::ChannelMessagesFilter:
        *this << channelMessagesFilterValue.flags;
        *this << channelMessagesFilterValue.ranges;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLDocumentAttribute &documentAttributeValue)
{
    *this << documentAttributeValue.tlType;

    switch (documentAttributeValue.tlType) {
    case TLValue::DocumentAttributeImageSize:
        *this << documentAttributeValue.w;
        *this << documentAttributeValue.h;
        break;
    case TLValue::DocumentAttributeAnimated:
    case TLValue::DocumentAttributeHasStickers:
        break;
    case TLValue::DocumentAttributeSticker:
        *this << documentAttributeValue.flags;
        *this << documentAttributeValue.alt;
        *this << documentAttributeValue.stickerset;
        if (documentAttributeValue.flags & 1 << 0) {
            *this << documentAttributeValue.maskCoords;
        }
        break;
    case TLValue::DocumentAttributeVideo:
        *this << documentAttributeValue.flags;
        *this << documentAttributeValue.duration;
        *this << documentAttributeValue.w;
        *this << documentAttributeValue.h;
        break;
    case TLValue::DocumentAttributeAudio:
        *this << documentAttributeValue.flags;
        *this << documentAttributeValue.duration;
        if (documentAttributeValue.flags & 1 << 0) {
            *this << documentAttributeValue.title;
        }
        if (documentAttributeValue.flags & 1 << 1) {
            *this << documentAttributeValue.performer;
        }
        if (documentAttributeValue.flags & 1 << 2) {
            *this << documentAttributeValue.waveform;
        }
        break;
    case TLValue::DocumentAttributeFilename:
        *this << documentAttributeValue.fileName;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputChatPhoto &inputChatPhotoValue)
{
    *this << inputChatPhotoValue.tlType;

    switch (inputChatPhotoValue.tlType) {
    case TLValue::InputChatPhotoEmpty:
        break;
    case TLValue::InputChatUploadedPhoto:
        *this << inputChatPhotoValue.file;
        break;
    case TLValue::InputChatPhoto:
        *this << inputChatPhotoValue.id;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputDialogPeer &inputDialogPeerValue)
{
    *this << inputDialogPeerValue.tlType;

    switch (inputDialogPeerValue.tlType) {
    case TLValue::InputDialogPeer:
        *this << inputDialogPeerValue.peer;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputGame &inputGameValue)
{
    *this << inputGameValue.tlType;

    switch (inputGameValue.tlType) {
    case TLValue::InputGameID:
        *this << inputGameValue.id;
        *this << inputGameValue.accessHash;
        break;
    case TLValue::InputGameShortName:
        *this << inputGameValue.botId;
        *this << inputGameValue.shortName;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputNotifyPeer &inputNotifyPeerValue)
{
    *this << inputNotifyPeerValue.tlType;

    switch (inputNotifyPeerValue.tlType) {
    case TLValue::InputNotifyPeer:
        *this << inputNotifyPeerValue.peer;
        break;
    case TLValue::InputNotifyUsers:
    case TLValue::InputNotifyChats:
    case TLValue::InputNotifyBroadcasts:
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputPaymentCredentials &inputPaymentCredentialsValue)
{
    *this << inputPaymentCredentialsValue.tlType;

    switch (inputPaymentCredentialsValue.tlType) {
    case TLValue::InputPaymentCredentialsSaved:
        *this << inputPaymentCredentialsValue.id;
        *this << inputPaymentCredentialsValue.tmpPassword;
        break;
    case TLValue::InputPaymentCredentials:
        *this << inputPaymentCredentialsValue.flags;
        *this << inputPaymentCredentialsValue.data;
        break;
    case TLValue::InputPaymentCredentialsApplePay:
        *this << inputPaymentCredentialsValue.paymentData;
        break;
    case TLValue::InputPaymentCredentialsAndroidPay:
        *this << inputPaymentCredentialsValue.paymentToken;
        *this << inputPaymentCredentialsValue.googleTransactionId;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputPrivacyRule &inputPrivacyRuleValue)
{
    *this << inputPrivacyRuleValue.tlType;

    switch (inputPrivacyRuleValue.tlType) {
    case TLValue::InputPrivacyValueAllowContacts:
    case TLValue::InputPrivacyValueAllowAll:
    case TLValue::InputPrivacyValueDisallowContacts:
    case TLValue::InputPrivacyValueDisallowAll:
        break;
    case TLValue::InputPrivacyValueAllowUsers:
    case TLValue::InputPrivacyValueDisallowUsers:
        *this << inputPrivacyRuleValue.users;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputSecureValue &inputSecureValueValue)
{
    *this << inputSecureValueValue.tlType;

    switch (inputSecureValueValue.tlType) {
    case TLValue::InputSecureValue:
        *this << inputSecureValueValue.flags;
        *this << inputSecureValueValue.type;
        if (inputSecureValueValue.flags & 1 << 0) {
            *this << inputSecureValueValue.data;
        }
        if (inputSecureValueValue.flags & 1 << 1) {
            *this << inputSecureValueValue.frontSide;
        }
        if (inputSecureValueValue.flags & 1 << 2) {
            *this << inputSecureValueValue.reverseSide;
        }
        if (inputSecureValueValue.flags & 1 << 3) {
            *this << inputSecureValueValue.selfie;
        }
        if (inputSecureValueValue.flags & 1 << 6) {
            *this << inputSecureValueValue.translation;
        }
        if (inputSecureValueValue.flags & 1 << 4) {
            *this << inputSecureValueValue.files;
        }
        if (inputSecureValueValue.flags & 1 << 5) {
            *this << inputSecureValueValue.plainData;
        }
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputStickerSetItem &inputStickerSetItemValue)
{
    *this << inputStickerSetItemValue.tlType;

    switch (inputStickerSetItemValue.tlType) {
    case TLValue::InputStickerSetItem:
        *this << inputStickerSetItemValue.flags;
        *this << inputStickerSetItemValue.document;
        *this << inputStickerSetItemValue.emoji;
        if (inputStickerSetItemValue.flags & 1 << 0) {
            *this << inputStickerSetItemValue.maskCoords;
        }
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputWebDocument &inputWebDocumentValue)
{
    *this << inputWebDocumentValue.tlType;

    switch (inputWebDocumentValue.tlType) {
    case TLValue::InputWebDocument:
        *this << inputWebDocumentValue.url;
        *this << inputWebDocumentValue.size;
        *this << inputWebDocumentValue.mimeType;
        *this << inputWebDocumentValue.attributes;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInvoice &invoiceValue)
{
    *this << invoiceValue.tlType;

    switch (invoiceValue.tlType) {
    case TLValue::Invoice:
        *this << invoiceValue.flags;
        *this << invoiceValue.currency;
        *this << invoiceValue.prices;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLKeyboardButton &keyboardButtonValue)
{
    *this << keyboardButtonValue.tlType;

    switch (keyboardButtonValue.tlType) {
    case TLValue::KeyboardButton:
    case TLValue::KeyboardButtonRequestPhone:
    case TLValue::KeyboardButtonRequestGeoLocation:
    case TLValue::KeyboardButtonGame:
    case TLValue::KeyboardButtonBuy:
        *this << keyboardButtonValue.text;
        break;
    case TLValue::KeyboardButtonUrl:
        *this << keyboardButtonValue.text;
        *this << keyboardButtonValue.url;
        break;
    case TLValue::KeyboardButtonCallback:
        *this << keyboardButtonValue.text;
        *this << keyboardButtonValue.data;
        break;
    case TLValue::KeyboardButtonSwitchInline:
        *this << keyboardButtonValue.flags;
        *this << keyboardButtonValue.text;
        *this << keyboardButtonValue.query;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLKeyboardButtonRow &keyboardButtonRowValue)
{
    *this << keyboardButtonRowValue.tlType;

    switch (keyboardButtonRowValue.tlType) {
    case TLValue::KeyboardButtonRow:
        *this << keyboardButtonRowValue.buttons;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLMessagesFilter &messagesFilterValue)
{
    *this << messagesFilterValue.tlType;

    switch (messagesFilterValue.tlType) {
    case TLValue::InputMessagesFilterEmpty:
    case TLValue::InputMessagesFilterPhotos:
    case TLValue::InputMessagesFilterVideo:
    case TLValue::InputMessagesFilterPhotoVideo:
    case TLValue::InputMessagesFilterDocument:
    case TLValue::InputMessagesFilterUrl:
    case TLValue::InputMessagesFilterGif:
    case TLValue::InputMessagesFilterVoice:
    case TLValue::InputMessagesFilterMusic:
    case TLValue::InputMessagesFilterChatPhotos:
    case TLValue::InputMessagesFilterRoundVoice:
    case TLValue::InputMessagesFilterRoundVideo:
    case TLValue::InputMessagesFilterMyMentions:
    case TLValue::InputMessagesFilterGeo:
    case TLValue::InputMessagesFilterContacts:
        break;
    case TLValue::InputMessagesFilterPhoneCalls:
        *this << messagesFilterValue.flags;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLPaymentRequestedInfo &paymentRequestedInfoValue)
{
    *this << paymentRequestedInfoValue.tlType;

    switch (paymentRequestedInfoValue.tlType) {
    case TLValue::PaymentRequestedInfo:
        *this << paymentRequestedInfoValue.flags;
        if (paymentRequestedInfoValue.flags & 1 << 0) {
            *this << paymentRequestedInfoValue.name;
        }
        if (paymentRequestedInfoValue.flags & 1 << 1) {
            *this << paymentRequestedInfoValue.phone;
        }
        if (paymentRequestedInfoValue.flags & 1 << 2) {
            *this << paymentRequestedInfoValue.email;
        }
        if (paymentRequestedInfoValue.flags & 1 << 3) {
            *this << paymentRequestedInfoValue.shippingAddress;
        }
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLPhoneCallProtocol &phoneCallProtocolValue)
{
    *this << phoneCallProtocolValue.tlType;

    switch (phoneCallProtocolValue.tlType) {
    case TLValue::PhoneCallProtocol:
        *this << phoneCallProtocolValue.flags;
        *this << phoneCallProtocolValue.minLayer;
        *this << phoneCallProtocolValue.maxLayer;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLPoll &pollValue)
{
    *this << pollValue.tlType;

    switch (pollValue.tlType) {
    case TLValue::Poll:
        *this << pollValue.id;
        *this << pollValue.flags;
        *this << pollValue.question;
        *this << pollValue.answers;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLReplyMarkup &replyMarkupValue)
{
    *this << replyMarkupValue.tlType;

    switch (replyMarkupValue.tlType) {
    case TLValue::ReplyKeyboardHide:
    case TLValue::ReplyKeyboardForceReply:
        *this << replyMarkupValue.flags;
        break;
    case TLValue::ReplyKeyboardMarkup:
        *this << replyMarkupValue.flags;
        *this << replyMarkupValue.rows;
        break;
    case TLValue::ReplyInlineMarkup:
        *this << replyMarkupValue.rows;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSecureValueError &secureValueErrorValue)
{
    *this << secureValueErrorValue.tlType;

    switch (secureValueErrorValue.tlType) {
    case TLValue::SecureValueErrorData:
        *this << secureValueErrorValue.type;
        *this << secureValueErrorValue.dataHash;
        *this << secureValueErrorValue.field;
        *this << secureValueErrorValue.text;
        break;
    case TLValue::SecureValueErrorFrontSide:
    case TLValue::SecureValueErrorReverseSide:
    case TLValue::SecureValueErrorSelfie:
    case TLValue::SecureValueErrorFile:
    case TLValue::SecureValueErrorTranslationFile:
        *this << secureValueErrorValue.type;
        *this << secureValueErrorValue.byteArrayFileHash;
        *this << secureValueErrorValue.text;
        break;
    case TLValue::SecureValueErrorFiles:
    case TLValue::SecureValueErrorTranslationFiles:
        *this << secureValueErrorValue.type;
        *this << secureValueErrorValue.byteArrayFileHashVector;
        *this << secureValueErrorValue.text;
        break;
    case TLValue::SecureValueError:
        *this << secureValueErrorValue.type;
        *this << secureValueErrorValue.hash;
        *this << secureValueErrorValue.text;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLSecureValueHash &secureValueHashValue)
{
    *this << secureValueHashValue.tlType;

    switch (secureValueHashValue.tlType) {
    case TLValue::SecureValueHash:
        *this << secureValueHashValue.type;
        *this << secureValueHashValue.hash;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputBotInlineMessage &inputBotInlineMessageValue)
{
    *this << inputBotInlineMessageValue.tlType;

    switch (inputBotInlineMessageValue.tlType) {
    case TLValue::InputBotInlineMessageMediaAuto:
    case TLValue::InputBotInlineMessageText:
        *this << inputBotInlineMessageValue.flags;
        *this << inputBotInlineMessageValue.message;
        if (inputBotInlineMessageValue.flags & 1 << 1) {
            *this << inputBotInlineMessageValue.entities;
        }
        if (inputBotInlineMessageValue.flags & 1 << 2) {
            *this << inputBotInlineMessageValue.replyMarkup;
        }
        break;
    case TLValue::InputBotInlineMessageMediaGeo:
        *this << inputBotInlineMessageValue.flags;
        *this << inputBotInlineMessageValue.geoPoint;
        *this << inputBotInlineMessageValue.period;
        if (inputBotInlineMessageValue.flags & 1 << 2) {
            *this << inputBotInlineMessageValue.replyMarkup;
        }
        break;
    case TLValue::InputBotInlineMessageMediaVenue:
        *this << inputBotInlineMessageValue.flags;
        *this << inputBotInlineMessageValue.geoPoint;
        *this << inputBotInlineMessageValue.title;
        *this << inputBotInlineMessageValue.address;
        *this << inputBotInlineMessageValue.provider;
        *this << inputBotInlineMessageValue.venueId;
        *this << inputBotInlineMessageValue.venueType;
        if (inputBotInlineMessageValue.flags & 1 << 2) {
            *this << inputBotInlineMessageValue.replyMarkup;
        }
        break;
    case TLValue::InputBotInlineMessageMediaContact:
        *this << inputBotInlineMessageValue.flags;
        *this << inputBotInlineMessageValue.phoneNumber;
        *this << inputBotInlineMessageValue.firstName;
        *this << inputBotInlineMessageValue.lastName;
        *this << inputBotInlineMessageValue.vcard;
        if (inputBotInlineMessageValue.flags & 1 << 2) {
            *this << inputBotInlineMessageValue.replyMarkup;
        }
        break;
    case TLValue::InputBotInlineMessageGame:
        *this << inputBotInlineMessageValue.flags;
        if (inputBotInlineMessageValue.flags & 1 << 2) {
            *this << inputBotInlineMessageValue.replyMarkup;
        }
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputBotInlineResult &inputBotInlineResultValue)
{
    *this << inputBotInlineResultValue.tlType;

    switch (inputBotInlineResultValue.tlType) {
    case TLValue::InputBotInlineResult:
        *this << inputBotInlineResultValue.flags;
        *this << inputBotInlineResultValue.id;
        *this << inputBotInlineResultValue.type;
        if (inputBotInlineResultValue.flags & 1 << 1) {
            *this << inputBotInlineResultValue.title;
        }
        if (inputBotInlineResultValue.flags & 1 << 2) {
            *this << inputBotInlineResultValue.description;
        }
        if (inputBotInlineResultValue.flags & 1 << 3) {
            *this << inputBotInlineResultValue.url;
        }
        if (inputBotInlineResultValue.flags & 1 << 4) {
            *this << inputBotInlineResultValue.thumb;
        }
        if (inputBotInlineResultValue.flags & 1 << 5) {
            *this << inputBotInlineResultValue.content;
        }
        *this << inputBotInlineResultValue.sendMessage;
        break;
    case TLValue::InputBotInlineResultPhoto:
        *this << inputBotInlineResultValue.id;
        *this << inputBotInlineResultValue.type;
        *this << inputBotInlineResultValue.photo;
        *this << inputBotInlineResultValue.sendMessage;
        break;
    case TLValue::InputBotInlineResultDocument:
        *this << inputBotInlineResultValue.flags;
        *this << inputBotInlineResultValue.id;
        *this << inputBotInlineResultValue.type;
        if (inputBotInlineResultValue.flags & 1 << 1) {
            *this << inputBotInlineResultValue.title;
        }
        if (inputBotInlineResultValue.flags & 1 << 2) {
            *this << inputBotInlineResultValue.description;
        }
        *this << inputBotInlineResultValue.document;
        *this << inputBotInlineResultValue.sendMessage;
        break;
    case TLValue::InputBotInlineResultGame:
        *this << inputBotInlineResultValue.id;
        *this << inputBotInlineResultValue.shortName;
        *this << inputBotInlineResultValue.sendMessage;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputMedia &inputMediaValue)
{
    *this << inputMediaValue.tlType;

    switch (inputMediaValue.tlType) {
    case TLValue::InputMediaEmpty:
        break;
    case TLValue::InputMediaUploadedPhoto:
        *this << inputMediaValue.flags;
        *this << inputMediaValue.file;
        if (inputMediaValue.flags & 1 << 0) {
            *this << inputMediaValue.stickers;
        }
        if (inputMediaValue.flags & 1 << 1) {
            *this << inputMediaValue.ttlSeconds;
        }
        break;
    case TLValue::InputMediaPhoto:
        *this << inputMediaValue.flags;
        *this << inputMediaValue.inputPhotoId;
        if (inputMediaValue.flags & 1 << 0) {
            *this << inputMediaValue.ttlSeconds;
        }
        break;
    case TLValue::InputMediaGeoPoint:
        *this << inputMediaValue.geoPoint;
        break;
    case TLValue::InputMediaContact:
        *this << inputMediaValue.phoneNumber;
        *this << inputMediaValue.firstName;
        *this << inputMediaValue.lastName;
        *this << inputMediaValue.vcard;
        break;
    case TLValue::InputMediaUploadedDocument:
        *this << inputMediaValue.flags;
        *this << inputMediaValue.file;
        if (inputMediaValue.flags & 1 << 2) {
            *this << inputMediaValue.thumb;
        }
        *this << inputMediaValue.mimeType;
        *this << inputMediaValue.attributes;
        if (inputMediaValue.flags & 1 << 0) {
            *this << inputMediaValue.stickers;
        }
        if (inputMediaValue.flags & 1 << 1) {
            *this << inputMediaValue.ttlSeconds;
        }
        break;
    case TLValue::InputMediaDocument:
        *this << inputMediaValue.flags;
        *this << inputMediaValue.inputDocumentId;
        if (inputMediaValue.flags & 1 << 0) {
            *this << inputMediaValue.ttlSeconds;
        }
        break;
    case TLValue::InputMediaVenue:
        *this << inputMediaValue.geoPoint;
        *this << inputMediaValue.title;
        *this << inputMediaValue.address;
        *this << inputMediaValue.provider;
        *this << inputMediaValue.venueId;
        *this << inputMediaValue.venueType;
        break;
    case TLValue::InputMediaGifExternal:
        *this << inputMediaValue.url;
        *this << inputMediaValue.q;
        break;
    case TLValue::InputMediaPhotoExternal:
    case TLValue::InputMediaDocumentExternal:
        *this << inputMediaValue.flags;
        *this << inputMediaValue.url;
        if (inputMediaValue.flags & 1 << 0) {
            *this << inputMediaValue.ttlSeconds;
        }
        break;
    case TLValue::InputMediaGame:
        *this << inputMediaValue.inputGameId;
        break;
    case TLValue::InputMediaInvoice:
        *this << inputMediaValue.flags;
        *this << inputMediaValue.title;
        *this << inputMediaValue.description;
        if (inputMediaValue.flags & 1 << 0) {
            *this << inputMediaValue.photo;
        }
        *this << inputMediaValue.invoice;
        *this << inputMediaValue.payload;
        *this << inputMediaValue.provider;
        *this << inputMediaValue.providerData;
        *this << inputMediaValue.startParam;
        break;
    case TLValue::InputMediaGeoLive:
        *this << inputMediaValue.flags;
        *this << inputMediaValue.geoPoint;
        if (inputMediaValue.flags & 1 << 1) {
            *this << inputMediaValue.period;
        }
        break;
    case TLValue::InputMediaPoll:
        *this << inputMediaValue.poll;
        break;
    default:
        break;
    }

    return *this;
}

CTelegramStream &CTelegramStream::operator<<(const TLInputSingleMedia &inputSingleMediaValue)
{
    *this << inputSingleMediaValue.tlType;

    switch (inputSingleMediaValue.tlType) {
    case TLValue::InputSingleMedia:
        *this << inputSingleMediaValue.flags;
        *this << inputSingleMediaValue.media;
        *this << inputSingleMediaValue.randomId;
        *this << inputSingleMediaValue.message;
        if (inputSingleMediaValue.flags & 1 << 0) {
            *this << inputSingleMediaValue.entities;
        }
        break;
    default:
        break;
    }

    return *this;
}

// End of generated write operators implementation
