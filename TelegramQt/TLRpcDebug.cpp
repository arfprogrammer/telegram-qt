/*
   Copyright (C) 2017 Alexandr Akulich <akulichalexander@gmail.com>

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

#include "TLRpcDebug.hpp"

#include "TLTypesDebug.hpp"

#include "CTelegramStream.hpp"

void dumpRpc(CTelegramStream &stream)
{
    TLValue request;
    stream >> request;

    switch (request) {
    // Generated RPC debug cases
    case TLValue::AccountAcceptAuthorization: {
        quint32 botId;
        stream >> botId;
        QString scope;
        stream >> scope;
        QString publicKey;
        stream >> publicKey;
        TLVector<TLSecureValueHash> valueHashes;
        stream >> valueHashes;
        TLSecureCredentialsEncrypted credentials;
        stream >> credentials;
        qDebug() << request << "botId" << botId << "scope" << scope << "publicKey" << publicKey << "valueHashes" << valueHashes << "credentials" << credentials;
    }
        break;

    case TLValue::AccountCancelPasswordEmail: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountChangePhone: {
        QString phoneNumber;
        stream >> phoneNumber;
        QString phoneCodeHash;
        stream >> phoneCodeHash;
        QString phoneCode;
        stream >> phoneCode;
        qDebug() << request << "phoneNumber" << phoneNumber << "phoneCodeHash" << phoneCodeHash << "phoneCode" << phoneCode;
    }
        break;

    case TLValue::AccountCheckUsername: {
        QString username;
        stream >> username;
        qDebug() << request << "username" << username;
    }
        break;

    case TLValue::AccountConfirmPasswordEmail: {
        QString code;
        stream >> code;
        qDebug() << request << "code" << code;
    }
        break;

    case TLValue::AccountConfirmPhone: {
        QString phoneCodeHash;
        stream >> phoneCodeHash;
        QString phoneCode;
        stream >> phoneCode;
        qDebug() << request << "phoneCodeHash" << phoneCodeHash << "phoneCode" << phoneCode;
    }
        break;

    case TLValue::AccountDeleteAccount: {
        QString reason;
        stream >> reason;
        qDebug() << request << "reason" << reason;
    }
        break;

    case TLValue::AccountDeleteSecureValue: {
        TLVector<TLSecureValueType> types;
        stream >> types;
        qDebug() << request << "types" << types;
    }
        break;

    case TLValue::AccountGetAccountTTL: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountGetAllSecureValues: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountGetAuthorizationForm: {
        quint32 botId;
        stream >> botId;
        QString scope;
        stream >> scope;
        QString publicKey;
        stream >> publicKey;
        qDebug() << request << "botId" << botId << "scope" << scope << "publicKey" << publicKey;
    }
        break;

    case TLValue::AccountGetAuthorizations: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountGetContactSignUpNotification: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountGetNotifySettings: {
        TLInputNotifyPeer peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::AccountGetPassword: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountGetPasswordSettings: {
        TLInputCheckPasswordSRP password;
        stream >> password;
        qDebug() << request << "password" << password;
    }
        break;

    case TLValue::AccountGetPrivacy: {
        TLInputPrivacyKey key;
        stream >> key;
        qDebug() << request << "key" << key;
    }
        break;

    case TLValue::AccountGetSecureValue: {
        TLVector<TLSecureValueType> types;
        stream >> types;
        qDebug() << request << "types" << types;
    }
        break;

    case TLValue::AccountGetTmpPassword: {
        TLInputCheckPasswordSRP password;
        stream >> password;
        quint32 period;
        stream >> period;
        qDebug() << request << "password" << password << "period" << period;
    }
        break;

    case TLValue::AccountGetWallPapers: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountGetWebAuthorizations: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountRegisterDevice: {
        quint32 tokenType;
        stream >> tokenType;
        QString token;
        stream >> token;
        bool appSandbox;
        stream >> appSandbox;
        QByteArray secret;
        stream >> secret;
        TLVector<quint32> otherUids;
        stream >> otherUids;
        qDebug() << request << "tokenType" << tokenType << "token" << token << "appSandbox" << appSandbox << "secret" << secret << "otherUids" << otherUids;
    }
        break;

    case TLValue::AccountReportPeer: {
        TLInputPeer peer;
        stream >> peer;
        TLReportReason reason;
        stream >> reason;
        qDebug() << request << "peer" << peer << "reason" << reason;
    }
        break;

    case TLValue::AccountResendPasswordEmail: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountResetAuthorization: {
        quint64 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::AccountResetNotifySettings: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountResetWebAuthorization: {
        quint64 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::AccountResetWebAuthorizations: {
        qDebug() << request;
    }
        break;

    case TLValue::AccountSaveSecureValue: {
        TLInputSecureValue value;
        stream >> value;
        quint64 secureSecretId;
        stream >> secureSecretId;
        qDebug() << request << "value" << value << "secureSecretId" << secureSecretId;
    }
        break;

    case TLValue::AccountSendVerifyEmailCode: {
        QString email;
        stream >> email;
        qDebug() << request << "email" << email;
    }
        break;

    case TLValue::AccountSetAccountTTL: {
        TLAccountDaysTTL ttl;
        stream >> ttl;
        qDebug() << request << "ttl" << ttl;
    }
        break;

    case TLValue::AccountSetContactSignUpNotification: {
        bool silent;
        stream >> silent;
        qDebug() << request << "silent" << silent;
    }
        break;

    case TLValue::AccountSetPrivacy: {
        TLInputPrivacyKey key;
        stream >> key;
        TLVector<TLInputPrivacyRule> rules;
        stream >> rules;
        qDebug() << request << "key" << key << "rules" << rules;
    }
        break;

    case TLValue::AccountUnregisterDevice: {
        quint32 tokenType;
        stream >> tokenType;
        QString token;
        stream >> token;
        TLVector<quint32> otherUids;
        stream >> otherUids;
        qDebug() << request << "tokenType" << tokenType << "token" << token << "otherUids" << otherUids;
    }
        break;

    case TLValue::AccountUpdateDeviceLocked: {
        quint32 period;
        stream >> period;
        qDebug() << request << "period" << period;
    }
        break;

    case TLValue::AccountUpdateNotifySettings: {
        TLInputNotifyPeer peer;
        stream >> peer;
        TLInputPeerNotifySettings settings;
        stream >> settings;
        qDebug() << request << "peer" << peer << "settings" << settings;
    }
        break;

    case TLValue::AccountUpdatePasswordSettings: {
        TLInputCheckPasswordSRP password;
        stream >> password;
        TLAccountPasswordInputSettings newSettings;
        stream >> newSettings;
        qDebug() << request << "password" << password << "newSettings" << newSettings;
    }
        break;

    case TLValue::AccountUpdateStatus: {
        bool offline;
        stream >> offline;
        qDebug() << request << "offline" << offline;
    }
        break;

    case TLValue::AccountUpdateUsername: {
        QString username;
        stream >> username;
        qDebug() << request << "username" << username;
    }
        break;

    case TLValue::AccountVerifyEmail: {
        QString email;
        stream >> email;
        QString code;
        stream >> code;
        qDebug() << request << "email" << email << "code" << code;
    }
        break;

    case TLValue::AccountVerifyPhone: {
        QString phoneNumber;
        stream >> phoneNumber;
        QString phoneCodeHash;
        stream >> phoneCodeHash;
        QString phoneCode;
        stream >> phoneCode;
        qDebug() << request << "phoneNumber" << phoneNumber << "phoneCodeHash" << phoneCodeHash << "phoneCode" << phoneCode;
    }
        break;

    case TLValue::AuthBindTempAuthKey: {
        quint64 permAuthKeyId;
        stream >> permAuthKeyId;
        quint64 nonce;
        stream >> nonce;
        quint32 expiresAt;
        stream >> expiresAt;
        QByteArray encryptedMessage;
        stream >> encryptedMessage;
        qDebug() << request << "permAuthKeyId" << permAuthKeyId << "nonce" << nonce << "expiresAt" << expiresAt << "encryptedMessage" << encryptedMessage;
    }
        break;

    case TLValue::AuthCancelCode: {
        QString phoneNumber;
        stream >> phoneNumber;
        QString phoneCodeHash;
        stream >> phoneCodeHash;
        qDebug() << request << "phoneNumber" << phoneNumber << "phoneCodeHash" << phoneCodeHash;
    }
        break;

    case TLValue::AuthCheckPassword: {
        TLInputCheckPasswordSRP password;
        stream >> password;
        qDebug() << request << "password" << password;
    }
        break;

    case TLValue::AuthDropTempAuthKeys: {
        TLVector<quint64> exceptAuthKeys;
        stream >> exceptAuthKeys;
        qDebug() << request << "exceptAuthKeys" << exceptAuthKeys;
    }
        break;

    case TLValue::AuthExportAuthorization: {
        quint32 dcId;
        stream >> dcId;
        qDebug() << request << "dcId" << dcId;
    }
        break;

    case TLValue::AuthImportAuthorization: {
        quint32 id;
        stream >> id;
        QByteArray bytes;
        stream >> bytes;
        qDebug() << request << "id" << id << "bytes" << bytes;
    }
        break;

    case TLValue::AuthImportBotAuthorization: {
        quint32 flags;
        stream >> flags;
        quint32 apiId;
        stream >> apiId;
        QString apiHash;
        stream >> apiHash;
        QString botAuthToken;
        stream >> botAuthToken;
        qDebug() << request << "flags" << flags << "apiId" << apiId << "apiHash" << apiHash << "botAuthToken" << botAuthToken;
    }
        break;

    case TLValue::AuthLogOut: {
        qDebug() << request;
    }
        break;

    case TLValue::AuthRecoverPassword: {
        QString code;
        stream >> code;
        qDebug() << request << "code" << code;
    }
        break;

    case TLValue::AuthRequestPasswordRecovery: {
        qDebug() << request;
    }
        break;

    case TLValue::AuthResendCode: {
        QString phoneNumber;
        stream >> phoneNumber;
        QString phoneCodeHash;
        stream >> phoneCodeHash;
        qDebug() << request << "phoneNumber" << phoneNumber << "phoneCodeHash" << phoneCodeHash;
    }
        break;

    case TLValue::AuthResetAuthorizations: {
        qDebug() << request;
    }
        break;

    case TLValue::AuthSignIn: {
        QString phoneNumber;
        stream >> phoneNumber;
        QString phoneCodeHash;
        stream >> phoneCodeHash;
        QString phoneCode;
        stream >> phoneCode;
        qDebug() << request << "phoneNumber" << phoneNumber << "phoneCodeHash" << phoneCodeHash << "phoneCode" << phoneCode;
    }
        break;

    case TLValue::AuthSignUp: {
        QString phoneNumber;
        stream >> phoneNumber;
        QString phoneCodeHash;
        stream >> phoneCodeHash;
        QString phoneCode;
        stream >> phoneCode;
        QString firstName;
        stream >> firstName;
        QString lastName;
        stream >> lastName;
        qDebug() << request << "phoneNumber" << phoneNumber << "phoneCodeHash" << phoneCodeHash << "phoneCode" << phoneCode << "firstName" << firstName << "lastName" << lastName;
    }
        break;

    case TLValue::BotsAnswerWebhookJSONQuery: {
        quint64 queryId;
        stream >> queryId;
        TLDataJSON data;
        stream >> data;
        qDebug() << request << "queryId" << queryId << "data" << data;
    }
        break;

    case TLValue::BotsSendCustomRequest: {
        QString customMethod;
        stream >> customMethod;
        TLDataJSON params;
        stream >> params;
        qDebug() << request << "customMethod" << customMethod << "params" << params;
    }
        break;

    case TLValue::ChannelsCheckUsername: {
        TLInputChannel channel;
        stream >> channel;
        QString username;
        stream >> username;
        qDebug() << request << "channel" << channel << "username" << username;
    }
        break;

    case TLValue::ChannelsDeleteChannel: {
        TLInputChannel channel;
        stream >> channel;
        qDebug() << request << "channel" << channel;
    }
        break;

    case TLValue::ChannelsDeleteHistory: {
        TLInputChannel channel;
        stream >> channel;
        quint32 maxId;
        stream >> maxId;
        qDebug() << request << "channel" << channel << "maxId" << maxId;
    }
        break;

    case TLValue::ChannelsDeleteMessages: {
        TLInputChannel channel;
        stream >> channel;
        TLVector<quint32> id;
        stream >> id;
        qDebug() << request << "channel" << channel << "id" << id;
    }
        break;

    case TLValue::ChannelsDeleteUserHistory: {
        TLInputChannel channel;
        stream >> channel;
        TLInputUser userId;
        stream >> userId;
        qDebug() << request << "channel" << channel << "userId" << userId;
    }
        break;

    case TLValue::ChannelsEditAbout: {
        TLInputChannel channel;
        stream >> channel;
        QString about;
        stream >> about;
        qDebug() << request << "channel" << channel << "about" << about;
    }
        break;

    case TLValue::ChannelsEditAdmin: {
        TLInputChannel channel;
        stream >> channel;
        TLInputUser userId;
        stream >> userId;
        TLChannelAdminRights adminRights;
        stream >> adminRights;
        qDebug() << request << "channel" << channel << "userId" << userId << "adminRights" << adminRights;
    }
        break;

    case TLValue::ChannelsEditBanned: {
        TLInputChannel channel;
        stream >> channel;
        TLInputUser userId;
        stream >> userId;
        TLChannelBannedRights bannedRights;
        stream >> bannedRights;
        qDebug() << request << "channel" << channel << "userId" << userId << "bannedRights" << bannedRights;
    }
        break;

    case TLValue::ChannelsEditPhoto: {
        TLInputChannel channel;
        stream >> channel;
        TLInputChatPhoto photo;
        stream >> photo;
        qDebug() << request << "channel" << channel << "photo" << photo;
    }
        break;

    case TLValue::ChannelsEditTitle: {
        TLInputChannel channel;
        stream >> channel;
        QString title;
        stream >> title;
        qDebug() << request << "channel" << channel << "title" << title;
    }
        break;

    case TLValue::ChannelsExportInvite: {
        TLInputChannel channel;
        stream >> channel;
        qDebug() << request << "channel" << channel;
    }
        break;

    case TLValue::ChannelsExportMessageLink: {
        TLInputChannel channel;
        stream >> channel;
        quint32 id;
        stream >> id;
        bool grouped;
        stream >> grouped;
        qDebug() << request << "channel" << channel << "id" << id << "grouped" << grouped;
    }
        break;

    case TLValue::ChannelsGetAdminedPublicChannels: {
        qDebug() << request;
    }
        break;

    case TLValue::ChannelsGetChannels: {
        TLVector<TLInputChannel> id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::ChannelsGetFullChannel: {
        TLInputChannel channel;
        stream >> channel;
        qDebug() << request << "channel" << channel;
    }
        break;

    case TLValue::ChannelsGetLeftChannels: {
        quint32 offset;
        stream >> offset;
        qDebug() << request << "offset" << offset;
    }
        break;

    case TLValue::ChannelsGetMessages: {
        TLInputChannel channel;
        stream >> channel;
        TLVector<TLInputMessage> id;
        stream >> id;
        qDebug() << request << "channel" << channel << "id" << id;
    }
        break;

    case TLValue::ChannelsGetParticipant: {
        TLInputChannel channel;
        stream >> channel;
        TLInputUser userId;
        stream >> userId;
        qDebug() << request << "channel" << channel << "userId" << userId;
    }
        break;

    case TLValue::ChannelsGetParticipants: {
        TLInputChannel channel;
        stream >> channel;
        TLChannelParticipantsFilter filter;
        stream >> filter;
        quint32 offset;
        stream >> offset;
        quint32 limit;
        stream >> limit;
        quint32 hash;
        stream >> hash;
        qDebug() << request << "channel" << channel << "filter" << filter << "offset" << offset << "limit" << limit << "hash" << hash;
    }
        break;

    case TLValue::ChannelsInviteToChannel: {
        TLInputChannel channel;
        stream >> channel;
        TLVector<TLInputUser> users;
        stream >> users;
        qDebug() << request << "channel" << channel << "users" << users;
    }
        break;

    case TLValue::ChannelsJoinChannel: {
        TLInputChannel channel;
        stream >> channel;
        qDebug() << request << "channel" << channel;
    }
        break;

    case TLValue::ChannelsLeaveChannel: {
        TLInputChannel channel;
        stream >> channel;
        qDebug() << request << "channel" << channel;
    }
        break;

    case TLValue::ChannelsReadHistory: {
        TLInputChannel channel;
        stream >> channel;
        quint32 maxId;
        stream >> maxId;
        qDebug() << request << "channel" << channel << "maxId" << maxId;
    }
        break;

    case TLValue::ChannelsReadMessageContents: {
        TLInputChannel channel;
        stream >> channel;
        TLVector<quint32> id;
        stream >> id;
        qDebug() << request << "channel" << channel << "id" << id;
    }
        break;

    case TLValue::ChannelsReportSpam: {
        TLInputChannel channel;
        stream >> channel;
        TLInputUser userId;
        stream >> userId;
        TLVector<quint32> id;
        stream >> id;
        qDebug() << request << "channel" << channel << "userId" << userId << "id" << id;
    }
        break;

    case TLValue::ChannelsSetStickers: {
        TLInputChannel channel;
        stream >> channel;
        TLInputStickerSet stickerset;
        stream >> stickerset;
        qDebug() << request << "channel" << channel << "stickerset" << stickerset;
    }
        break;

    case TLValue::ChannelsToggleInvites: {
        TLInputChannel channel;
        stream >> channel;
        bool enabled;
        stream >> enabled;
        qDebug() << request << "channel" << channel << "enabled" << enabled;
    }
        break;

    case TLValue::ChannelsTogglePreHistoryHidden: {
        TLInputChannel channel;
        stream >> channel;
        bool enabled;
        stream >> enabled;
        qDebug() << request << "channel" << channel << "enabled" << enabled;
    }
        break;

    case TLValue::ChannelsToggleSignatures: {
        TLInputChannel channel;
        stream >> channel;
        bool enabled;
        stream >> enabled;
        qDebug() << request << "channel" << channel << "enabled" << enabled;
    }
        break;

    case TLValue::ChannelsUpdateUsername: {
        TLInputChannel channel;
        stream >> channel;
        QString username;
        stream >> username;
        qDebug() << request << "channel" << channel << "username" << username;
    }
        break;

    case TLValue::ContactsBlock: {
        TLInputUser id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::ContactsDeleteByPhones: {
        TLVector<QString> phones;
        stream >> phones;
        qDebug() << request << "phones" << phones;
    }
        break;

    case TLValue::ContactsDeleteContact: {
        TLInputUser id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::ContactsDeleteContacts: {
        TLVector<TLInputUser> id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::ContactsGetBlocked: {
        quint32 offset;
        stream >> offset;
        quint32 limit;
        stream >> limit;
        qDebug() << request << "offset" << offset << "limit" << limit;
    }
        break;

    case TLValue::ContactsGetContactIDs: {
        quint32 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::ContactsGetContacts: {
        quint32 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::ContactsGetSaved: {
        qDebug() << request;
    }
        break;

    case TLValue::ContactsGetStatuses: {
        qDebug() << request;
    }
        break;

    case TLValue::ContactsImportContacts: {
        TLVector<TLInputContact> contacts;
        stream >> contacts;
        qDebug() << request << "contacts" << contacts;
    }
        break;

    case TLValue::ContactsResetSaved: {
        qDebug() << request;
    }
        break;

    case TLValue::ContactsResetTopPeerRating: {
        TLTopPeerCategory category;
        stream >> category;
        TLInputPeer peer;
        stream >> peer;
        qDebug() << request << "category" << category << "peer" << peer;
    }
        break;

    case TLValue::ContactsResolveUsername: {
        QString username;
        stream >> username;
        qDebug() << request << "username" << username;
    }
        break;

    case TLValue::ContactsSearch: {
        QString q;
        stream >> q;
        quint32 limit;
        stream >> limit;
        qDebug() << request << "q" << q << "limit" << limit;
    }
        break;

    case TLValue::ContactsToggleTopPeers: {
        bool enabled;
        stream >> enabled;
        qDebug() << request << "enabled" << enabled;
    }
        break;

    case TLValue::ContactsUnblock: {
        TLInputUser id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::ContestSaveDeveloperInfo: {
        quint32 vkId;
        stream >> vkId;
        QString name;
        stream >> name;
        QString phoneNumber;
        stream >> phoneNumber;
        quint32 age;
        stream >> age;
        QString city;
        stream >> city;
        qDebug() << request << "vkId" << vkId << "name" << name << "phoneNumber" << phoneNumber << "age" << age << "city" << city;
    }
        break;

    case TLValue::DestroyAuthKey: {
        qDebug() << request;
    }
        break;

    case TLValue::DestroySession: {
        quint64 sessionId;
        stream >> sessionId;
        qDebug() << request << "sessionId" << sessionId;
    }
        break;

    case TLValue::GetFutureSalts: {
        quint32 num;
        stream >> num;
        qDebug() << request << "num" << num;
    }
        break;

    case TLValue::HelpAcceptTermsOfService: {
        TLDataJSON id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::HelpEditUserInfo: {
        TLInputUser userId;
        stream >> userId;
        QString message;
        stream >> message;
        TLVector<TLMessageEntity> entities;
        stream >> entities;
        qDebug() << request << "userId" << userId << "message" << message << "entities" << entities;
    }
        break;

    case TLValue::HelpGetAppChangelog: {
        QString prevAppVersion;
        stream >> prevAppVersion;
        qDebug() << request << "prevAppVersion" << prevAppVersion;
    }
        break;

    case TLValue::HelpGetAppConfig: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetAppUpdate: {
        QString source;
        stream >> source;
        qDebug() << request << "source" << source;
    }
        break;

    case TLValue::HelpGetCdnConfig: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetConfig: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetDeepLinkInfo: {
        QString path;
        stream >> path;
        qDebug() << request << "path" << path;
    }
        break;

    case TLValue::HelpGetInviteText: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetNearestDc: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetPassportConfig: {
        quint32 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::HelpGetProxyData: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetRecentMeUrls: {
        QString referer;
        stream >> referer;
        qDebug() << request << "referer" << referer;
    }
        break;

    case TLValue::HelpGetSupport: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetSupportName: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetTermsOfServiceUpdate: {
        qDebug() << request;
    }
        break;

    case TLValue::HelpGetUserInfo: {
        TLInputUser userId;
        stream >> userId;
        qDebug() << request << "userId" << userId;
    }
        break;

    case TLValue::HelpSaveAppLog: {
        TLVector<TLInputAppEvent> events;
        stream >> events;
        qDebug() << request << "events" << events;
    }
        break;

    case TLValue::HelpSetBotUpdatesStatus: {
        quint32 pendingUpdatesCount;
        stream >> pendingUpdatesCount;
        QString message;
        stream >> message;
        qDebug() << request << "pendingUpdatesCount" << pendingUpdatesCount << "message" << message;
    }
        break;

    case TLValue::InitConnection: {
        qDebug() << request;
    }
        break;

    case TLValue::InvokeAfterMsg: {
        qDebug() << request;
    }
        break;

    case TLValue::InvokeAfterMsgs: {
        qDebug() << request;
    }
        break;

    case TLValue::InvokeWithLayer: {
        qDebug() << request;
    }
        break;

    case TLValue::InvokeWithMessagesRange: {
        qDebug() << request;
    }
        break;

    case TLValue::InvokeWithTakeout: {
        qDebug() << request;
    }
        break;

    case TLValue::InvokeWithoutUpdates: {
        qDebug() << request;
    }
        break;

    case TLValue::LangpackGetDifference: {
        QString langCode;
        stream >> langCode;
        quint32 fromVersion;
        stream >> fromVersion;
        qDebug() << request << "langCode" << langCode << "fromVersion" << fromVersion;
    }
        break;

    case TLValue::LangpackGetLangPack: {
        QString langPack;
        stream >> langPack;
        QString langCode;
        stream >> langCode;
        qDebug() << request << "langPack" << langPack << "langCode" << langCode;
    }
        break;

    case TLValue::LangpackGetLanguage: {
        QString langPack;
        stream >> langPack;
        QString langCode;
        stream >> langCode;
        qDebug() << request << "langPack" << langPack << "langCode" << langCode;
    }
        break;

    case TLValue::LangpackGetLanguages: {
        QString langPack;
        stream >> langPack;
        qDebug() << request << "langPack" << langPack;
    }
        break;

    case TLValue::LangpackGetStrings: {
        QString langPack;
        stream >> langPack;
        QString langCode;
        stream >> langCode;
        TLVector<QString> keys;
        stream >> keys;
        qDebug() << request << "langPack" << langPack << "langCode" << langCode << "keys" << keys;
    }
        break;

    case TLValue::MessagesAcceptEncryption: {
        TLInputEncryptedChat peer;
        stream >> peer;
        QByteArray gB;
        stream >> gB;
        quint64 keyFingerprint;
        stream >> keyFingerprint;
        qDebug() << request << "peer" << peer << "gB" << gB << "keyFingerprint" << keyFingerprint;
    }
        break;

    case TLValue::MessagesAddChatUser: {
        quint32 chatId;
        stream >> chatId;
        TLInputUser userId;
        stream >> userId;
        quint32 fwdLimit;
        stream >> fwdLimit;
        qDebug() << request << "chatId" << chatId << "userId" << userId << "fwdLimit" << fwdLimit;
    }
        break;

    case TLValue::MessagesCheckChatInvite: {
        QString hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::MessagesClearAllDrafts: {
        qDebug() << request;
    }
        break;

    case TLValue::MessagesCreateChat: {
        TLVector<TLInputUser> users;
        stream >> users;
        QString title;
        stream >> title;
        qDebug() << request << "users" << users << "title" << title;
    }
        break;

    case TLValue::MessagesDeleteChatUser: {
        quint32 chatId;
        stream >> chatId;
        TLInputUser userId;
        stream >> userId;
        qDebug() << request << "chatId" << chatId << "userId" << userId;
    }
        break;

    case TLValue::MessagesDiscardEncryption: {
        quint32 chatId;
        stream >> chatId;
        qDebug() << request << "chatId" << chatId;
    }
        break;

    case TLValue::MessagesEditChatAdmin: {
        quint32 chatId;
        stream >> chatId;
        TLInputUser userId;
        stream >> userId;
        bool isAdmin;
        stream >> isAdmin;
        qDebug() << request << "chatId" << chatId << "userId" << userId << "isAdmin" << isAdmin;
    }
        break;

    case TLValue::MessagesEditChatPhoto: {
        quint32 chatId;
        stream >> chatId;
        TLInputChatPhoto photo;
        stream >> photo;
        qDebug() << request << "chatId" << chatId << "photo" << photo;
    }
        break;

    case TLValue::MessagesEditChatTitle: {
        quint32 chatId;
        stream >> chatId;
        QString title;
        stream >> title;
        qDebug() << request << "chatId" << chatId << "title" << title;
    }
        break;

    case TLValue::MessagesExportChatInvite: {
        quint32 chatId;
        stream >> chatId;
        qDebug() << request << "chatId" << chatId;
    }
        break;

    case TLValue::MessagesFaveSticker: {
        TLInputDocument id;
        stream >> id;
        bool unfave;
        stream >> unfave;
        qDebug() << request << "id" << id << "unfave" << unfave;
    }
        break;

    case TLValue::MessagesGetAllChats: {
        TLVector<quint32> exceptIds;
        stream >> exceptIds;
        qDebug() << request << "exceptIds" << exceptIds;
    }
        break;

    case TLValue::MessagesGetAllDrafts: {
        qDebug() << request;
    }
        break;

    case TLValue::MessagesGetAllStickers: {
        quint32 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::MessagesGetAttachedStickers: {
        TLInputStickeredMedia media;
        stream >> media;
        qDebug() << request << "media" << media;
    }
        break;

    case TLValue::MessagesGetChats: {
        TLVector<quint32> id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::MessagesGetCommonChats: {
        TLInputUser userId;
        stream >> userId;
        quint32 maxId;
        stream >> maxId;
        quint32 limit;
        stream >> limit;
        qDebug() << request << "userId" << userId << "maxId" << maxId << "limit" << limit;
    }
        break;

    case TLValue::MessagesGetDhConfig: {
        quint32 version;
        stream >> version;
        quint32 randomLength;
        stream >> randomLength;
        qDebug() << request << "version" << version << "randomLength" << randomLength;
    }
        break;

    case TLValue::MessagesGetDialogUnreadMarks: {
        qDebug() << request;
    }
        break;

    case TLValue::MessagesGetDocumentByHash: {
        QByteArray sha256;
        stream >> sha256;
        quint32 size;
        stream >> size;
        QString mimeType;
        stream >> mimeType;
        qDebug() << request << "sha256" << sha256 << "size" << size << "mimeType" << mimeType;
    }
        break;

    case TLValue::MessagesGetFavedStickers: {
        quint32 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::MessagesGetFeaturedStickers: {
        quint32 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::MessagesGetFullChat: {
        quint32 chatId;
        stream >> chatId;
        qDebug() << request << "chatId" << chatId;
    }
        break;

    case TLValue::MessagesGetGameHighScores: {
        TLInputPeer peer;
        stream >> peer;
        quint32 id;
        stream >> id;
        TLInputUser userId;
        stream >> userId;
        qDebug() << request << "peer" << peer << "id" << id << "userId" << userId;
    }
        break;

    case TLValue::MessagesGetHistory: {
        TLInputPeer peer;
        stream >> peer;
        quint32 offsetId;
        stream >> offsetId;
        quint32 offsetDate;
        stream >> offsetDate;
        quint32 addOffset;
        stream >> addOffset;
        quint32 limit;
        stream >> limit;
        quint32 maxId;
        stream >> maxId;
        quint32 minId;
        stream >> minId;
        quint32 hash;
        stream >> hash;
        qDebug() << request << "peer" << peer << "offsetId" << offsetId << "offsetDate" << offsetDate << "addOffset" << addOffset << "limit" << limit << "maxId" << maxId << "minId" << minId << "hash" << hash;
    }
        break;

    case TLValue::MessagesGetInlineGameHighScores: {
        TLInputBotInlineMessageID id;
        stream >> id;
        TLInputUser userId;
        stream >> userId;
        qDebug() << request << "id" << id << "userId" << userId;
    }
        break;

    case TLValue::MessagesGetMaskStickers: {
        quint32 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::MessagesGetMessageEditData: {
        TLInputPeer peer;
        stream >> peer;
        quint32 id;
        stream >> id;
        qDebug() << request << "peer" << peer << "id" << id;
    }
        break;

    case TLValue::MessagesGetMessages: {
        TLVector<TLInputMessage> id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::MessagesGetMessagesViews: {
        TLInputPeer peer;
        stream >> peer;
        TLVector<quint32> id;
        stream >> id;
        bool increment;
        stream >> increment;
        qDebug() << request << "peer" << peer << "id" << id << "increment" << increment;
    }
        break;

    case TLValue::MessagesGetOnlines: {
        TLInputPeer peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::MessagesGetPeerDialogs: {
        TLVector<TLInputDialogPeer> peers;
        stream >> peers;
        qDebug() << request << "peers" << peers;
    }
        break;

    case TLValue::MessagesGetPeerSettings: {
        TLInputPeer peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::MessagesGetPinnedDialogs: {
        qDebug() << request;
    }
        break;

    case TLValue::MessagesGetPollResults: {
        TLInputPeer peer;
        stream >> peer;
        quint32 msgId;
        stream >> msgId;
        qDebug() << request << "peer" << peer << "msgId" << msgId;
    }
        break;

    case TLValue::MessagesGetRecentLocations: {
        TLInputPeer peer;
        stream >> peer;
        quint32 limit;
        stream >> limit;
        quint32 hash;
        stream >> hash;
        qDebug() << request << "peer" << peer << "limit" << limit << "hash" << hash;
    }
        break;

    case TLValue::MessagesGetSavedGifs: {
        quint32 hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::MessagesGetSplitRanges: {
        qDebug() << request;
    }
        break;

    case TLValue::MessagesGetStatsURL: {
        TLInputPeer peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::MessagesGetStickerSet: {
        TLInputStickerSet stickerset;
        stream >> stickerset;
        qDebug() << request << "stickerset" << stickerset;
    }
        break;

    case TLValue::MessagesGetStickers: {
        QString emoticon;
        stream >> emoticon;
        quint32 hash;
        stream >> hash;
        qDebug() << request << "emoticon" << emoticon << "hash" << hash;
    }
        break;

    case TLValue::MessagesGetUnreadMentions: {
        TLInputPeer peer;
        stream >> peer;
        quint32 offsetId;
        stream >> offsetId;
        quint32 addOffset;
        stream >> addOffset;
        quint32 limit;
        stream >> limit;
        quint32 maxId;
        stream >> maxId;
        quint32 minId;
        stream >> minId;
        qDebug() << request << "peer" << peer << "offsetId" << offsetId << "addOffset" << addOffset << "limit" << limit << "maxId" << maxId << "minId" << minId;
    }
        break;

    case TLValue::MessagesGetWebPage: {
        QString url;
        stream >> url;
        quint32 hash;
        stream >> hash;
        qDebug() << request << "url" << url << "hash" << hash;
    }
        break;

    case TLValue::MessagesHideReportSpam: {
        TLInputPeer peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::MessagesImportChatInvite: {
        QString hash;
        stream >> hash;
        qDebug() << request << "hash" << hash;
    }
        break;

    case TLValue::MessagesInstallStickerSet: {
        TLInputStickerSet stickerset;
        stream >> stickerset;
        bool archived;
        stream >> archived;
        qDebug() << request << "stickerset" << stickerset << "archived" << archived;
    }
        break;

    case TLValue::MessagesMigrateChat: {
        quint32 chatId;
        stream >> chatId;
        qDebug() << request << "chatId" << chatId;
    }
        break;

    case TLValue::MessagesReadEncryptedHistory: {
        TLInputEncryptedChat peer;
        stream >> peer;
        quint32 maxDate;
        stream >> maxDate;
        qDebug() << request << "peer" << peer << "maxDate" << maxDate;
    }
        break;

    case TLValue::MessagesReadFeaturedStickers: {
        TLVector<quint64> id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::MessagesReadHistory: {
        TLInputPeer peer;
        stream >> peer;
        quint32 maxId;
        stream >> maxId;
        qDebug() << request << "peer" << peer << "maxId" << maxId;
    }
        break;

    case TLValue::MessagesReadMentions: {
        TLInputPeer peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::MessagesReadMessageContents: {
        TLVector<quint32> id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::MessagesReceivedMessages: {
        quint32 maxId;
        stream >> maxId;
        qDebug() << request << "maxId" << maxId;
    }
        break;

    case TLValue::MessagesReceivedQueue: {
        quint32 maxQts;
        stream >> maxQts;
        qDebug() << request << "maxQts" << maxQts;
    }
        break;

    case TLValue::MessagesReport: {
        TLInputPeer peer;
        stream >> peer;
        TLVector<quint32> id;
        stream >> id;
        TLReportReason reason;
        stream >> reason;
        qDebug() << request << "peer" << peer << "id" << id << "reason" << reason;
    }
        break;

    case TLValue::MessagesReportEncryptedSpam: {
        TLInputEncryptedChat peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::MessagesReportSpam: {
        TLInputPeer peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::MessagesRequestEncryption: {
        TLInputUser userId;
        stream >> userId;
        quint32 randomId;
        stream >> randomId;
        QByteArray gA;
        stream >> gA;
        qDebug() << request << "userId" << userId << "randomId" << randomId << "gA" << gA;
    }
        break;

    case TLValue::MessagesSaveGif: {
        TLInputDocument id;
        stream >> id;
        bool unsave;
        stream >> unsave;
        qDebug() << request << "id" << id << "unsave" << unsave;
    }
        break;

    case TLValue::MessagesSearchGifs: {
        QString q;
        stream >> q;
        quint32 offset;
        stream >> offset;
        qDebug() << request << "q" << q << "offset" << offset;
    }
        break;

    case TLValue::MessagesSearchGlobal: {
        QString q;
        stream >> q;
        quint32 offsetDate;
        stream >> offsetDate;
        TLInputPeer offsetPeer;
        stream >> offsetPeer;
        quint32 offsetId;
        stream >> offsetId;
        quint32 limit;
        stream >> limit;
        qDebug() << request << "q" << q << "offsetDate" << offsetDate << "offsetPeer" << offsetPeer << "offsetId" << offsetId << "limit" << limit;
    }
        break;

    case TLValue::MessagesSendEncrypted: {
        TLInputEncryptedChat peer;
        stream >> peer;
        quint64 randomId;
        stream >> randomId;
        QByteArray data;
        stream >> data;
        qDebug() << request << "peer" << peer << "randomId" << randomId << "data" << data;
    }
        break;

    case TLValue::MessagesSendEncryptedFile: {
        TLInputEncryptedChat peer;
        stream >> peer;
        quint64 randomId;
        stream >> randomId;
        QByteArray data;
        stream >> data;
        TLInputEncryptedFile file;
        stream >> file;
        qDebug() << request << "peer" << peer << "randomId" << randomId << "data" << data << "file" << file;
    }
        break;

    case TLValue::MessagesSendEncryptedService: {
        TLInputEncryptedChat peer;
        stream >> peer;
        quint64 randomId;
        stream >> randomId;
        QByteArray data;
        stream >> data;
        qDebug() << request << "peer" << peer << "randomId" << randomId << "data" << data;
    }
        break;

    case TLValue::MessagesSendScreenshotNotification: {
        TLInputPeer peer;
        stream >> peer;
        quint32 replyToMsgId;
        stream >> replyToMsgId;
        quint64 randomId;
        stream >> randomId;
        qDebug() << request << "peer" << peer << "replyToMsgId" << replyToMsgId << "randomId" << randomId;
    }
        break;

    case TLValue::MessagesSendVote: {
        TLInputPeer peer;
        stream >> peer;
        quint32 msgId;
        stream >> msgId;
        TLVector<QByteArray> options;
        stream >> options;
        qDebug() << request << "peer" << peer << "msgId" << msgId << "options" << options;
    }
        break;

    case TLValue::MessagesSetEncryptedTyping: {
        TLInputEncryptedChat peer;
        stream >> peer;
        bool typing;
        stream >> typing;
        qDebug() << request << "peer" << peer << "typing" << typing;
    }
        break;

    case TLValue::MessagesSetTyping: {
        TLInputPeer peer;
        stream >> peer;
        TLSendMessageAction action;
        stream >> action;
        qDebug() << request << "peer" << peer << "action" << action;
    }
        break;

    case TLValue::MessagesStartBot: {
        TLInputUser bot;
        stream >> bot;
        TLInputPeer peer;
        stream >> peer;
        quint64 randomId;
        stream >> randomId;
        QString startParam;
        stream >> startParam;
        qDebug() << request << "bot" << bot << "peer" << peer << "randomId" << randomId << "startParam" << startParam;
    }
        break;

    case TLValue::MessagesToggleChatAdmins: {
        quint32 chatId;
        stream >> chatId;
        bool enabled;
        stream >> enabled;
        qDebug() << request << "chatId" << chatId << "enabled" << enabled;
    }
        break;

    case TLValue::MessagesUninstallStickerSet: {
        TLInputStickerSet stickerset;
        stream >> stickerset;
        qDebug() << request << "stickerset" << stickerset;
    }
        break;

    case TLValue::MessagesUploadEncryptedFile: {
        TLInputEncryptedChat peer;
        stream >> peer;
        TLInputEncryptedFile file;
        stream >> file;
        qDebug() << request << "peer" << peer << "file" << file;
    }
        break;

    case TLValue::MessagesUploadMedia: {
        TLInputPeer peer;
        stream >> peer;
        TLInputMedia media;
        stream >> media;
        qDebug() << request << "peer" << peer << "media" << media;
    }
        break;

    case TLValue::PaymentsGetPaymentForm: {
        quint32 msgId;
        stream >> msgId;
        qDebug() << request << "msgId" << msgId;
    }
        break;

    case TLValue::PaymentsGetPaymentReceipt: {
        quint32 msgId;
        stream >> msgId;
        qDebug() << request << "msgId" << msgId;
    }
        break;

    case TLValue::PaymentsGetSavedInfo: {
        qDebug() << request;
    }
        break;

    case TLValue::PhoneAcceptCall: {
        TLInputPhoneCall peer;
        stream >> peer;
        QByteArray gB;
        stream >> gB;
        TLPhoneCallProtocol protocol;
        stream >> protocol;
        qDebug() << request << "peer" << peer << "gB" << gB << "protocol" << protocol;
    }
        break;

    case TLValue::PhoneConfirmCall: {
        TLInputPhoneCall peer;
        stream >> peer;
        QByteArray gA;
        stream >> gA;
        quint64 keyFingerprint;
        stream >> keyFingerprint;
        TLPhoneCallProtocol protocol;
        stream >> protocol;
        qDebug() << request << "peer" << peer << "gA" << gA << "keyFingerprint" << keyFingerprint << "protocol" << protocol;
    }
        break;

    case TLValue::PhoneDiscardCall: {
        TLInputPhoneCall peer;
        stream >> peer;
        quint32 duration;
        stream >> duration;
        TLPhoneCallDiscardReason reason;
        stream >> reason;
        quint64 connectionId;
        stream >> connectionId;
        qDebug() << request << "peer" << peer << "duration" << duration << "reason" << reason << "connectionId" << connectionId;
    }
        break;

    case TLValue::PhoneGetCallConfig: {
        qDebug() << request;
    }
        break;

    case TLValue::PhoneReceivedCall: {
        TLInputPhoneCall peer;
        stream >> peer;
        qDebug() << request << "peer" << peer;
    }
        break;

    case TLValue::PhoneRequestCall: {
        TLInputUser userId;
        stream >> userId;
        quint32 randomId;
        stream >> randomId;
        QByteArray gAHash;
        stream >> gAHash;
        TLPhoneCallProtocol protocol;
        stream >> protocol;
        qDebug() << request << "userId" << userId << "randomId" << randomId << "gAHash" << gAHash << "protocol" << protocol;
    }
        break;

    case TLValue::PhoneSaveCallDebug: {
        TLInputPhoneCall peer;
        stream >> peer;
        TLDataJSON debug;
        stream >> debug;
        qDebug() << request << "peer" << peer << "debug" << debug;
    }
        break;

    case TLValue::PhoneSetCallRating: {
        TLInputPhoneCall peer;
        stream >> peer;
        quint32 rating;
        stream >> rating;
        QString comment;
        stream >> comment;
        qDebug() << request << "peer" << peer << "rating" << rating << "comment" << comment;
    }
        break;

    case TLValue::PhotosDeletePhotos: {
        TLVector<TLInputPhoto> id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::PhotosGetUserPhotos: {
        TLInputUser userId;
        stream >> userId;
        quint32 offset;
        stream >> offset;
        quint64 maxId;
        stream >> maxId;
        quint32 limit;
        stream >> limit;
        qDebug() << request << "userId" << userId << "offset" << offset << "maxId" << maxId << "limit" << limit;
    }
        break;

    case TLValue::PhotosUpdateProfilePhoto: {
        TLInputPhoto id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::PhotosUploadProfilePhoto: {
        TLInputFile file;
        stream >> file;
        qDebug() << request << "file" << file;
    }
        break;

    case TLValue::Ping: {
        quint64 pingId;
        stream >> pingId;
        qDebug() << request << "pingId" << pingId;
    }
        break;

    case TLValue::PingDelayDisconnect: {
        quint64 pingId;
        stream >> pingId;
        quint32 disconnectDelay;
        stream >> disconnectDelay;
        qDebug() << request << "pingId" << pingId << "disconnectDelay" << disconnectDelay;
    }
        break;

    case TLValue::ReqDHParams: {
        TLNumber128 nonce;
        stream >> nonce;
        TLNumber128 serverNonce;
        stream >> serverNonce;
        QString p;
        stream >> p;
        QString q;
        stream >> q;
        quint64 publicKeyFingerprint;
        stream >> publicKeyFingerprint;
        QString encryptedData;
        stream >> encryptedData;
        qDebug() << request << "nonce" << nonce << "serverNonce" << serverNonce << "p" << p << "q" << q << "publicKeyFingerprint" << publicKeyFingerprint << "encryptedData" << encryptedData;
    }
        break;

    case TLValue::ReqPq: {
        TLNumber128 nonce;
        stream >> nonce;
        qDebug() << request << "nonce" << nonce;
    }
        break;

    case TLValue::ReqPqMulti: {
        TLNumber128 nonce;
        stream >> nonce;
        qDebug() << request << "nonce" << nonce;
    }
        break;

    case TLValue::RpcDropAnswer: {
        quint64 reqMsgId;
        stream >> reqMsgId;
        qDebug() << request << "reqMsgId" << reqMsgId;
    }
        break;

    case TLValue::SetClientDHParams: {
        TLNumber128 nonce;
        stream >> nonce;
        TLNumber128 serverNonce;
        stream >> serverNonce;
        QString encryptedData;
        stream >> encryptedData;
        qDebug() << request << "nonce" << nonce << "serverNonce" << serverNonce << "encryptedData" << encryptedData;
    }
        break;

    case TLValue::StickersAddStickerToSet: {
        TLInputStickerSet stickerset;
        stream >> stickerset;
        TLInputStickerSetItem sticker;
        stream >> sticker;
        qDebug() << request << "stickerset" << stickerset << "sticker" << sticker;
    }
        break;

    case TLValue::StickersChangeStickerPosition: {
        TLInputDocument sticker;
        stream >> sticker;
        quint32 position;
        stream >> position;
        qDebug() << request << "sticker" << sticker << "position" << position;
    }
        break;

    case TLValue::StickersRemoveStickerFromSet: {
        TLInputDocument sticker;
        stream >> sticker;
        qDebug() << request << "sticker" << sticker;
    }
        break;

    case TLValue::UpdatesGetState: {
        qDebug() << request;
    }
        break;

    case TLValue::UploadGetCdnFile: {
        QByteArray fileToken;
        stream >> fileToken;
        quint32 offset;
        stream >> offset;
        quint32 limit;
        stream >> limit;
        qDebug() << request << "fileToken" << fileToken << "offset" << offset << "limit" << limit;
    }
        break;

    case TLValue::UploadGetCdnFileHashes: {
        QByteArray fileToken;
        stream >> fileToken;
        quint32 offset;
        stream >> offset;
        qDebug() << request << "fileToken" << fileToken << "offset" << offset;
    }
        break;

    case TLValue::UploadGetFile: {
        TLInputFileLocation location;
        stream >> location;
        quint32 offset;
        stream >> offset;
        quint32 limit;
        stream >> limit;
        qDebug() << request << "location" << location << "offset" << offset << "limit" << limit;
    }
        break;

    case TLValue::UploadGetFileHashes: {
        TLInputFileLocation location;
        stream >> location;
        quint32 offset;
        stream >> offset;
        qDebug() << request << "location" << location << "offset" << offset;
    }
        break;

    case TLValue::UploadGetWebFile: {
        TLInputWebFileLocation location;
        stream >> location;
        quint32 offset;
        stream >> offset;
        quint32 limit;
        stream >> limit;
        qDebug() << request << "location" << location << "offset" << offset << "limit" << limit;
    }
        break;

    case TLValue::UploadReuploadCdnFile: {
        QByteArray fileToken;
        stream >> fileToken;
        QByteArray requestToken;
        stream >> requestToken;
        qDebug() << request << "fileToken" << fileToken << "requestToken" << requestToken;
    }
        break;

    case TLValue::UploadSaveBigFilePart: {
        quint64 fileId;
        stream >> fileId;
        quint32 filePart;
        stream >> filePart;
        quint32 fileTotalParts;
        stream >> fileTotalParts;
        QByteArray bytes;
        stream >> bytes;
        qDebug() << request << "fileId" << fileId << "filePart" << filePart << "fileTotalParts" << fileTotalParts << "bytes" << bytes;
    }
        break;

    case TLValue::UploadSaveFilePart: {
        quint64 fileId;
        stream >> fileId;
        quint32 filePart;
        stream >> filePart;
        QByteArray bytes;
        stream >> bytes;
        qDebug() << request << "fileId" << fileId << "filePart" << filePart << "bytes" << bytes;
    }
        break;

    case TLValue::UsersGetFullUser: {
        TLInputUser id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::UsersGetUsers: {
        TLVector<TLInputUser> id;
        stream >> id;
        qDebug() << request << "id" << id;
    }
        break;

    case TLValue::UsersSetSecureValueErrors: {
        TLInputUser id;
        stream >> id;
        TLVector<TLSecureValueError> errors;
        stream >> errors;
        qDebug() << request << "id" << id << "errors" << errors;
    }
        break;

    // End of generated RPC debug cases
    default:
        break;
    }
}
