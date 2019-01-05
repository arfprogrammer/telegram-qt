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

#ifndef CTELEGRAMSTREAM_HPP
#define CTELEGRAMSTREAM_HPP

#include <QVector>
#include <QString>

#include "CRawStream.hpp"
#include "TLNumbers.hpp"
#include "TLTypes.hpp"

class CTelegramStream : public CRawStreamEx
{
public:
    using CRawStreamEx::CRawStreamEx;
    using CRawStreamEx::operator <<;
    using CRawStreamEx::operator >>;

    CTelegramStream &operator>>(TLValue &v);
    CTelegramStream &operator<<(TLValue v);

    CTelegramStream &operator>>(QString &str);
    CTelegramStream &operator<<(const QString &str);

    CTelegramStream &operator>>(bool &data);
    CTelegramStream &operator<<(const bool &data);

    CTelegramStream &operator<<(const TLDcOption &dcOption);

    template <typename T>
    CTelegramStream &operator>>(TLVector<T> &v);
    template <typename T>
    CTelegramStream &operator>>(TLVector<T*> &v);
    template <typename T>
    CTelegramStream &operator<<(const TLVector<T> &v);
    template <typename T>
    CTelegramStream &operator<<(const TLVector<T*> &v);

    template <int Size>
    CTelegramStream &operator<<(const TLNumber<Size> &n);
    template <int Size>
    CTelegramStream &operator>>(TLNumber<Size> &n);

    // Generated read operators
    CTelegramStream &operator>>(TLAccountDaysTTL &accountDaysTTLValue);
    CTelegramStream &operator>>(TLAccountSentEmailCode &accountSentEmailCodeValue);
    CTelegramStream &operator>>(TLAccountTakeout &accountTakeoutValue);
    CTelegramStream &operator>>(TLAccountTmpPassword &accountTmpPasswordValue);
    CTelegramStream &operator>>(TLAuthCheckedPhone &authCheckedPhoneValue);
    CTelegramStream &operator>>(TLAuthCodeType &authCodeTypeValue);
    CTelegramStream &operator>>(TLAuthExportedAuthorization &authExportedAuthorizationValue);
    CTelegramStream &operator>>(TLAuthPasswordRecovery &authPasswordRecoveryValue);
    CTelegramStream &operator>>(TLAuthSentCodeType &authSentCodeTypeValue);
    CTelegramStream &operator>>(TLBadMsgNotification &badMsgNotificationValue);
    CTelegramStream &operator>>(TLBotCommand &botCommandValue);
    CTelegramStream &operator>>(TLBotInfo &botInfoValue);
    CTelegramStream &operator>>(TLCdnPublicKey &cdnPublicKeyValue);
    CTelegramStream &operator>>(TLChannelParticipantsFilter &channelParticipantsFilterValue);
    CTelegramStream &operator>>(TLChatOnlines &chatOnlinesValue);
    CTelegramStream &operator>>(TLChatParticipant &chatParticipantValue);
    CTelegramStream &operator>>(TLChatParticipants &chatParticipantsValue);
    CTelegramStream &operator>>(TLClientDHInnerData &clientDHInnerDataValue);
    CTelegramStream &operator>>(TLContact &contactValue);
    CTelegramStream &operator>>(TLContactBlocked &contactBlockedValue);
    CTelegramStream &operator>>(TLContactLink &contactLinkValue);
    CTelegramStream &operator>>(TLDataJSON &dataJSONValue);
    CTelegramStream &operator>>(TLDestroyAuthKeyRes &destroyAuthKeyResValue);
    CTelegramStream &operator>>(TLDestroySessionRes &destroySessionResValue);
    CTelegramStream &operator>>(TLEncryptedChat &encryptedChatValue);
    CTelegramStream &operator>>(TLEncryptedFile &encryptedFileValue);
    CTelegramStream &operator>>(TLEncryptedMessage &encryptedMessageValue);
    CTelegramStream &operator>>(TLError &errorValue);
    CTelegramStream &operator>>(TLExportedChatInvite &exportedChatInviteValue);
    CTelegramStream &operator>>(TLExportedMessageLink &exportedMessageLinkValue);
    CTelegramStream &operator>>(TLFileHash &fileHashValue);
    CTelegramStream &operator>>(TLFileLocation &fileLocationValue);
    CTelegramStream &operator>>(TLFutureSalt &futureSaltValue);
    CTelegramStream &operator>>(TLFutureSalts &futureSaltsValue);
    CTelegramStream &operator>>(TLGeoPoint &geoPointValue);
    CTelegramStream &operator>>(TLHelpInviteText &helpInviteTextValue);
    CTelegramStream &operator>>(TLHelpPassportConfig &helpPassportConfigValue);
    CTelegramStream &operator>>(TLHelpSupportName &helpSupportNameValue);
    CTelegramStream &operator>>(TLHighScore &highScoreValue);
    CTelegramStream &operator>>(TLHttpWait &httpWaitValue);
    CTelegramStream &operator>>(TLImportedContact &importedContactValue);
    CTelegramStream &operator>>(TLInlineBotSwitchPM &inlineBotSwitchPMValue);
    CTelegramStream &operator>>(TLInputBotInlineMessageID &inputBotInlineMessageIDValue);
    CTelegramStream &operator>>(TLInputChannel &inputChannelValue);
    CTelegramStream &operator>>(TLInputCheckPasswordSRP &inputCheckPasswordSRPValue);
    CTelegramStream &operator>>(TLInputClientProxy &inputClientProxyValue);
    CTelegramStream &operator>>(TLInputContact &inputContactValue);
    CTelegramStream &operator>>(TLInputDocument &inputDocumentValue);
    CTelegramStream &operator>>(TLInputEncryptedChat &inputEncryptedChatValue);
    CTelegramStream &operator>>(TLInputEncryptedFile &inputEncryptedFileValue);
    CTelegramStream &operator>>(TLInputFile &inputFileValue);
    CTelegramStream &operator>>(TLInputFileLocation &inputFileLocationValue);
    CTelegramStream &operator>>(TLInputGeoPoint &inputGeoPointValue);
    CTelegramStream &operator>>(TLInputMessage &inputMessageValue);
    CTelegramStream &operator>>(TLInputPeer &inputPeerValue);
    CTelegramStream &operator>>(TLInputPeerNotifySettings &inputPeerNotifySettingsValue);
    CTelegramStream &operator>>(TLInputPhoneCall &inputPhoneCallValue);
    CTelegramStream &operator>>(TLInputPhoto &inputPhotoValue);
    CTelegramStream &operator>>(TLInputPrivacyKey &inputPrivacyKeyValue);
    CTelegramStream &operator>>(TLInputSecureFile &inputSecureFileValue);
    CTelegramStream &operator>>(TLInputStickerSet &inputStickerSetValue);
    CTelegramStream &operator>>(TLInputStickeredMedia &inputStickeredMediaValue);
    CTelegramStream &operator>>(TLInputUser &inputUserValue);
    CTelegramStream &operator>>(TLInputWebFileLocation &inputWebFileLocationValue);
    CTelegramStream &operator>>(TLIpPort &ipPortValue);
    CTelegramStream &operator>>(TLLabeledPrice &labeledPriceValue);
    CTelegramStream &operator>>(TLLangPackString &langPackStringValue);
    CTelegramStream &operator>>(TLMaskCoords &maskCoordsValue);
    CTelegramStream &operator>>(TLMessageEntity &messageEntityValue);
    CTelegramStream &operator>>(TLMessageRange &messageRangeValue);
    CTelegramStream &operator>>(TLMessagesAffectedHistory &messagesAffectedHistoryValue);
    CTelegramStream &operator>>(TLMessagesAffectedMessages &messagesAffectedMessagesValue);
    CTelegramStream &operator>>(TLMessagesDhConfig &messagesDhConfigValue);
    CTelegramStream &operator>>(TLMessagesSentEncryptedMessage &messagesSentEncryptedMessageValue);
    CTelegramStream &operator>>(TLMsgDetailedInfo &msgDetailedInfoValue);
    CTelegramStream &operator>>(TLMsgResendReq &msgResendReqValue);
    CTelegramStream &operator>>(TLMsgsAck &msgsAckValue);
    CTelegramStream &operator>>(TLMsgsAllInfo &msgsAllInfoValue);
    CTelegramStream &operator>>(TLMsgsStateInfo &msgsStateInfoValue);
    CTelegramStream &operator>>(TLMsgsStateReq &msgsStateReqValue);
    CTelegramStream &operator>>(TLNearestDc &nearestDcValue);
    CTelegramStream &operator>>(TLNewSession &newSessionValue);
    CTelegramStream &operator>>(TLPQInnerData &pQInnerDataValue);
    CTelegramStream &operator>>(TLPageRelatedArticle &pageRelatedArticleValue);
    CTelegramStream &operator>>(TLPasswordKdfAlgo &passwordKdfAlgoValue);
    CTelegramStream &operator>>(TLPaymentCharge &paymentChargeValue);
    CTelegramStream &operator>>(TLPaymentSavedCredentials &paymentSavedCredentialsValue);
    CTelegramStream &operator>>(TLPeer &peerValue);
    CTelegramStream &operator>>(TLPeerNotifySettings &peerNotifySettingsValue);
    CTelegramStream &operator>>(TLPhoneCallDiscardReason &phoneCallDiscardReasonValue);
    CTelegramStream &operator>>(TLPhoneConnection &phoneConnectionValue);
    CTelegramStream &operator>>(TLPhotoSize &photoSizeValue);
    CTelegramStream &operator>>(TLPollAnswer &pollAnswerValue);
    CTelegramStream &operator>>(TLPong &pongValue);
    CTelegramStream &operator>>(TLPopularContact &popularContactValue);
    CTelegramStream &operator>>(TLPostAddress &postAddressValue);
    CTelegramStream &operator>>(TLPrivacyKey &privacyKeyValue);
    CTelegramStream &operator>>(TLPrivacyRule &privacyRuleValue);
    CTelegramStream &operator>>(TLReceivedNotifyMessage &receivedNotifyMessageValue);
    CTelegramStream &operator>>(TLReportReason &reportReasonValue);
    CTelegramStream &operator>>(TLResPQ &resPQValue);
    CTelegramStream &operator>>(TLRichText &richTextValue);
    CTelegramStream &operator>>(TLRpcDropAnswer &rpcDropAnswerValue);
    CTelegramStream &operator>>(TLRpcError &rpcErrorValue);
    CTelegramStream &operator>>(TLSavedContact &savedContactValue);
    CTelegramStream &operator>>(TLSecureCredentialsEncrypted &secureCredentialsEncryptedValue);
    CTelegramStream &operator>>(TLSecureData &secureDataValue);
    CTelegramStream &operator>>(TLSecureFile &secureFileValue);
    CTelegramStream &operator>>(TLSecurePasswordKdfAlgo &securePasswordKdfAlgoValue);
    CTelegramStream &operator>>(TLSecurePlainData &securePlainDataValue);
    CTelegramStream &operator>>(TLSecureSecretSettings &secureSecretSettingsValue);
    CTelegramStream &operator>>(TLSecureValueType &secureValueTypeValue);
    CTelegramStream &operator>>(TLSendMessageAction &sendMessageActionValue);
    CTelegramStream &operator>>(TLServerDHInnerData &serverDHInnerDataValue);
    CTelegramStream &operator>>(TLServerDHParams &serverDHParamsValue);
    CTelegramStream &operator>>(TLSetClientDHParamsAnswer &setClientDHParamsAnswerValue);
    CTelegramStream &operator>>(TLShippingOption &shippingOptionValue);
    CTelegramStream &operator>>(TLStatsURL &statsURLValue);
    CTelegramStream &operator>>(TLStickerPack &stickerPackValue);
    CTelegramStream &operator>>(TLStorageFileType &storageFileTypeValue);
    CTelegramStream &operator>>(TLTopPeer &topPeerValue);
    CTelegramStream &operator>>(TLTopPeerCategory &topPeerCategoryValue);
    CTelegramStream &operator>>(TLTopPeerCategoryPeers &topPeerCategoryPeersValue);
    CTelegramStream &operator>>(TLUpdatesState &updatesStateValue);
    CTelegramStream &operator>>(TLUploadCdnFile &uploadCdnFileValue);
    CTelegramStream &operator>>(TLUploadFile &uploadFileValue);
    CTelegramStream &operator>>(TLUploadWebFile &uploadWebFileValue);
    CTelegramStream &operator>>(TLUserProfilePhoto &userProfilePhotoValue);
    CTelegramStream &operator>>(TLUserStatus &userStatusValue);
    CTelegramStream &operator>>(TLWallPaper &wallPaperValue);
    CTelegramStream &operator>>(TLWebAuthorization &webAuthorizationValue);
    CTelegramStream &operator>>(TLAccessPointRule &accessPointRuleValue);
    CTelegramStream &operator>>(TLAccountPassword &accountPasswordValue);
    CTelegramStream &operator>>(TLAccountPasswordInputSettings &accountPasswordInputSettingsValue);
    CTelegramStream &operator>>(TLAccountPasswordSettings &accountPasswordSettingsValue);
    CTelegramStream &operator>>(TLAuthorization &authorizationValue);
    CTelegramStream &operator>>(TLCdnConfig &cdnConfigValue);
    CTelegramStream &operator>>(TLChannelAdminLogEventsFilter &channelAdminLogEventsFilterValue);
    CTelegramStream &operator>>(TLChannelAdminRights &channelAdminRightsValue);
    CTelegramStream &operator>>(TLChannelBannedRights &channelBannedRightsValue);
    CTelegramStream &operator>>(TLChannelMessagesFilter &channelMessagesFilterValue);
    CTelegramStream &operator>>(TLChannelParticipant &channelParticipantValue);
    CTelegramStream &operator>>(TLChatPhoto &chatPhotoValue);
    CTelegramStream &operator>>(TLContactStatus &contactStatusValue);
    CTelegramStream &operator>>(TLDcOption &dcOptionValue);
    CTelegramStream &operator>>(TLDialogPeer &dialogPeerValue);
    CTelegramStream &operator>>(TLDocumentAttribute &documentAttributeValue);
    CTelegramStream &operator>>(TLDraftMessage &draftMessageValue);
    CTelegramStream &operator>>(TLHelpConfigSimple &helpConfigSimpleValue);
    CTelegramStream &operator>>(TLHelpDeepLinkInfo &helpDeepLinkInfoValue);
    CTelegramStream &operator>>(TLHelpTermsOfService &helpTermsOfServiceValue);
    CTelegramStream &operator>>(TLHelpTermsOfServiceUpdate &helpTermsOfServiceUpdateValue);
    CTelegramStream &operator>>(TLHelpUserInfo &helpUserInfoValue);
    CTelegramStream &operator>>(TLInputChatPhoto &inputChatPhotoValue);
    CTelegramStream &operator>>(TLInputDialogPeer &inputDialogPeerValue);
    CTelegramStream &operator>>(TLInputGame &inputGameValue);
    CTelegramStream &operator>>(TLInputNotifyPeer &inputNotifyPeerValue);
    CTelegramStream &operator>>(TLInputPaymentCredentials &inputPaymentCredentialsValue);
    CTelegramStream &operator>>(TLInputPrivacyRule &inputPrivacyRuleValue);
    CTelegramStream &operator>>(TLInputSecureValue &inputSecureValueValue);
    CTelegramStream &operator>>(TLInputStickerSetItem &inputStickerSetItemValue);
    CTelegramStream &operator>>(TLInputWebDocument &inputWebDocumentValue);
    CTelegramStream &operator>>(TLInvoice &invoiceValue);
    CTelegramStream &operator>>(TLKeyboardButton &keyboardButtonValue);
    CTelegramStream &operator>>(TLKeyboardButtonRow &keyboardButtonRowValue);
    CTelegramStream &operator>>(TLLangPackDifference &langPackDifferenceValue);
    CTelegramStream &operator>>(TLLangPackLanguage &langPackLanguageValue);
    CTelegramStream &operator>>(TLMessageFwdHeader &messageFwdHeaderValue);
    CTelegramStream &operator>>(TLMessagesBotCallbackAnswer &messagesBotCallbackAnswerValue);
    CTelegramStream &operator>>(TLMessagesFilter &messagesFilterValue);
    CTelegramStream &operator>>(TLMessagesMessageEditData &messagesMessageEditDataValue);
    CTelegramStream &operator>>(TLNotifyPeer &notifyPeerValue);
    CTelegramStream &operator>>(TLPageCaption &pageCaptionValue);
    CTelegramStream &operator>>(TLPageTableCell &pageTableCellValue);
    CTelegramStream &operator>>(TLPageTableRow &pageTableRowValue);
    CTelegramStream &operator>>(TLPaymentRequestedInfo &paymentRequestedInfoValue);
    CTelegramStream &operator>>(TLPaymentsSavedInfo &paymentsSavedInfoValue);
    CTelegramStream &operator>>(TLPaymentsValidatedRequestedInfo &paymentsValidatedRequestedInfoValue);
    CTelegramStream &operator>>(TLPeerSettings &peerSettingsValue);
    CTelegramStream &operator>>(TLPhoneCallProtocol &phoneCallProtocolValue);
    CTelegramStream &operator>>(TLPhoto &photoValue);
    CTelegramStream &operator>>(TLPoll &pollValue);
    CTelegramStream &operator>>(TLPollAnswerVoters &pollAnswerVotersValue);
    CTelegramStream &operator>>(TLPollResults &pollResultsValue);
    CTelegramStream &operator>>(TLReplyMarkup &replyMarkupValue);
    CTelegramStream &operator>>(TLSecureRequiredType &secureRequiredTypeValue);
    CTelegramStream &operator>>(TLSecureValue &secureValueValue);
    CTelegramStream &operator>>(TLSecureValueError &secureValueErrorValue);
    CTelegramStream &operator>>(TLSecureValueHash &secureValueHashValue);
    CTelegramStream &operator>>(TLStickerSet &stickerSetValue);
    CTelegramStream &operator>>(TLUser &userValue);
    CTelegramStream &operator>>(TLWebDocument &webDocumentValue);
    CTelegramStream &operator>>(TLAccountAuthorizationForm &accountAuthorizationFormValue);
    CTelegramStream &operator>>(TLAccountAuthorizations &accountAuthorizationsValue);
    CTelegramStream &operator>>(TLAccountPrivacyRules &accountPrivacyRulesValue);
    CTelegramStream &operator>>(TLAccountWebAuthorizations &accountWebAuthorizationsValue);
    CTelegramStream &operator>>(TLAuthAuthorization &authAuthorizationValue);
    CTelegramStream &operator>>(TLAuthSentCode &authSentCodeValue);
    CTelegramStream &operator>>(TLBotInlineMessage &botInlineMessageValue);
    CTelegramStream &operator>>(TLChannelsChannelParticipant &channelsChannelParticipantValue);
    CTelegramStream &operator>>(TLChannelsChannelParticipants &channelsChannelParticipantsValue);
    CTelegramStream &operator>>(TLChat &chatValue);
    CTelegramStream &operator>>(TLChatFull &chatFullValue);
    CTelegramStream &operator>>(TLChatInvite &chatInviteValue);
    CTelegramStream &operator>>(TLConfig &configValue);
    CTelegramStream &operator>>(TLContactsBlocked &contactsBlockedValue);
    CTelegramStream &operator>>(TLContactsContacts &contactsContactsValue);
    CTelegramStream &operator>>(TLContactsFound &contactsFoundValue);
    CTelegramStream &operator>>(TLContactsImportedContacts &contactsImportedContactsValue);
    CTelegramStream &operator>>(TLContactsLink &contactsLinkValue);
    CTelegramStream &operator>>(TLContactsResolvedPeer &contactsResolvedPeerValue);
    CTelegramStream &operator>>(TLContactsTopPeers &contactsTopPeersValue);
    CTelegramStream &operator>>(TLDialog &dialogValue);
    CTelegramStream &operator>>(TLDocument &documentValue);
    CTelegramStream &operator>>(TLFoundGif &foundGifValue);
    CTelegramStream &operator>>(TLGame &gameValue);
    CTelegramStream &operator>>(TLHelpAppUpdate &helpAppUpdateValue);
    CTelegramStream &operator>>(TLHelpProxyData &helpProxyDataValue);
    CTelegramStream &operator>>(TLHelpSupport &helpSupportValue);
    CTelegramStream &operator>>(TLInputBotInlineMessage &inputBotInlineMessageValue);
    CTelegramStream &operator>>(TLInputBotInlineResult &inputBotInlineResultValue);
    CTelegramStream &operator>>(TLInputMedia &inputMediaValue);
    CTelegramStream &operator>>(TLInputSingleMedia &inputSingleMediaValue);
    CTelegramStream &operator>>(TLMessageAction &messageActionValue);
    CTelegramStream &operator>>(TLMessagesAllStickers &messagesAllStickersValue);
    CTelegramStream &operator>>(TLMessagesChatFull &messagesChatFullValue);
    CTelegramStream &operator>>(TLMessagesChats &messagesChatsValue);
    CTelegramStream &operator>>(TLMessagesFavedStickers &messagesFavedStickersValue);
    CTelegramStream &operator>>(TLMessagesFoundGifs &messagesFoundGifsValue);
    CTelegramStream &operator>>(TLMessagesHighScores &messagesHighScoresValue);
    CTelegramStream &operator>>(TLMessagesRecentStickers &messagesRecentStickersValue);
    CTelegramStream &operator>>(TLMessagesSavedGifs &messagesSavedGifsValue);
    CTelegramStream &operator>>(TLMessagesStickerSet &messagesStickerSetValue);
    CTelegramStream &operator>>(TLMessagesStickers &messagesStickersValue);
    CTelegramStream &operator>>(TLPaymentsPaymentForm &paymentsPaymentFormValue);
    CTelegramStream &operator>>(TLPaymentsPaymentReceipt &paymentsPaymentReceiptValue);
    CTelegramStream &operator>>(TLPhoneCall &phoneCallValue);
    CTelegramStream &operator>>(TLPhonePhoneCall &phonePhoneCallValue);
    CTelegramStream &operator>>(TLPhotosPhoto &photosPhotoValue);
    CTelegramStream &operator>>(TLPhotosPhotos &photosPhotosValue);
    CTelegramStream &operator>>(TLStickerSetCovered &stickerSetCoveredValue);
    CTelegramStream &operator>>(TLUserFull &userFullValue);
    CTelegramStream &operator>>(TLBotInlineResult &botInlineResultValue);
    CTelegramStream &operator>>(TLMessagesArchivedStickers &messagesArchivedStickersValue);
    CTelegramStream &operator>>(TLMessagesBotResults &messagesBotResultsValue);
    CTelegramStream &operator>>(TLMessagesFeaturedStickers &messagesFeaturedStickersValue);
    CTelegramStream &operator>>(TLMessagesFoundStickerSets &messagesFoundStickerSetsValue);
    CTelegramStream &operator>>(TLMessagesStickerSetInstallResult &messagesStickerSetInstallResultValue);
    CTelegramStream &operator>>(TLRecentMeUrl &recentMeUrlValue);
    CTelegramStream &operator>>(TLHelpRecentMeUrls &helpRecentMeUrlsValue);
    // End of generated read operators

    // Generated write operators
    CTelegramStream &operator<<(const TLAccountDaysTTL &accountDaysTTLValue);
    CTelegramStream &operator<<(const TLChannelParticipantsFilter &channelParticipantsFilterValue);
    CTelegramStream &operator<<(const TLDataJSON &dataJSONValue);
    CTelegramStream &operator<<(const TLInlineBotSwitchPM &inlineBotSwitchPMValue);
    CTelegramStream &operator<<(const TLInputBotInlineMessageID &inputBotInlineMessageIDValue);
    CTelegramStream &operator<<(const TLInputChannel &inputChannelValue);
    CTelegramStream &operator<<(const TLInputCheckPasswordSRP &inputCheckPasswordSRPValue);
    CTelegramStream &operator<<(const TLInputContact &inputContactValue);
    CTelegramStream &operator<<(const TLInputDocument &inputDocumentValue);
    CTelegramStream &operator<<(const TLInputEncryptedChat &inputEncryptedChatValue);
    CTelegramStream &operator<<(const TLInputEncryptedFile &inputEncryptedFileValue);
    CTelegramStream &operator<<(const TLInputFile &inputFileValue);
    CTelegramStream &operator<<(const TLInputFileLocation &inputFileLocationValue);
    CTelegramStream &operator<<(const TLInputGeoPoint &inputGeoPointValue);
    CTelegramStream &operator<<(const TLInputMessage &inputMessageValue);
    CTelegramStream &operator<<(const TLInputPeer &inputPeerValue);
    CTelegramStream &operator<<(const TLInputPeerNotifySettings &inputPeerNotifySettingsValue);
    CTelegramStream &operator<<(const TLInputPhoneCall &inputPhoneCallValue);
    CTelegramStream &operator<<(const TLInputPhoto &inputPhotoValue);
    CTelegramStream &operator<<(const TLInputPrivacyKey &inputPrivacyKeyValue);
    CTelegramStream &operator<<(const TLInputSecureFile &inputSecureFileValue);
    CTelegramStream &operator<<(const TLInputStickerSet &inputStickerSetValue);
    CTelegramStream &operator<<(const TLInputStickeredMedia &inputStickeredMediaValue);
    CTelegramStream &operator<<(const TLInputUser &inputUserValue);
    CTelegramStream &operator<<(const TLInputWebFileLocation &inputWebFileLocationValue);
    CTelegramStream &operator<<(const TLLabeledPrice &labeledPriceValue);
    CTelegramStream &operator<<(const TLMaskCoords &maskCoordsValue);
    CTelegramStream &operator<<(const TLMessageEntity &messageEntityValue);
    CTelegramStream &operator<<(const TLMessageRange &messageRangeValue);
    CTelegramStream &operator<<(const TLPasswordKdfAlgo &passwordKdfAlgoValue);
    CTelegramStream &operator<<(const TLPhoneCallDiscardReason &phoneCallDiscardReasonValue);
    CTelegramStream &operator<<(const TLPollAnswer &pollAnswerValue);
    CTelegramStream &operator<<(const TLPostAddress &postAddressValue);
    CTelegramStream &operator<<(const TLReportReason &reportReasonValue);
    CTelegramStream &operator<<(const TLSecureCredentialsEncrypted &secureCredentialsEncryptedValue);
    CTelegramStream &operator<<(const TLSecureData &secureDataValue);
    CTelegramStream &operator<<(const TLSecurePasswordKdfAlgo &securePasswordKdfAlgoValue);
    CTelegramStream &operator<<(const TLSecurePlainData &securePlainDataValue);
    CTelegramStream &operator<<(const TLSecureSecretSettings &secureSecretSettingsValue);
    CTelegramStream &operator<<(const TLSecureValueType &secureValueTypeValue);
    CTelegramStream &operator<<(const TLSendMessageAction &sendMessageActionValue);
    CTelegramStream &operator<<(const TLShippingOption &shippingOptionValue);
    CTelegramStream &operator<<(const TLTopPeerCategory &topPeerCategoryValue);
    CTelegramStream &operator<<(const TLAccountPasswordInputSettings &accountPasswordInputSettingsValue);
    CTelegramStream &operator<<(const TLChannelAdminLogEventsFilter &channelAdminLogEventsFilterValue);
    CTelegramStream &operator<<(const TLChannelAdminRights &channelAdminRightsValue);
    CTelegramStream &operator<<(const TLChannelBannedRights &channelBannedRightsValue);
    CTelegramStream &operator<<(const TLChannelMessagesFilter &channelMessagesFilterValue);
    CTelegramStream &operator<<(const TLDocumentAttribute &documentAttributeValue);
    CTelegramStream &operator<<(const TLInputChatPhoto &inputChatPhotoValue);
    CTelegramStream &operator<<(const TLInputDialogPeer &inputDialogPeerValue);
    CTelegramStream &operator<<(const TLInputGame &inputGameValue);
    CTelegramStream &operator<<(const TLInputNotifyPeer &inputNotifyPeerValue);
    CTelegramStream &operator<<(const TLInputPaymentCredentials &inputPaymentCredentialsValue);
    CTelegramStream &operator<<(const TLInputPrivacyRule &inputPrivacyRuleValue);
    CTelegramStream &operator<<(const TLInputSecureValue &inputSecureValueValue);
    CTelegramStream &operator<<(const TLInputStickerSetItem &inputStickerSetItemValue);
    CTelegramStream &operator<<(const TLInputWebDocument &inputWebDocumentValue);
    CTelegramStream &operator<<(const TLInvoice &invoiceValue);
    CTelegramStream &operator<<(const TLKeyboardButton &keyboardButtonValue);
    CTelegramStream &operator<<(const TLKeyboardButtonRow &keyboardButtonRowValue);
    CTelegramStream &operator<<(const TLMessagesFilter &messagesFilterValue);
    CTelegramStream &operator<<(const TLPaymentRequestedInfo &paymentRequestedInfoValue);
    CTelegramStream &operator<<(const TLPhoneCallProtocol &phoneCallProtocolValue);
    CTelegramStream &operator<<(const TLPoll &pollValue);
    CTelegramStream &operator<<(const TLReplyMarkup &replyMarkupValue);
    CTelegramStream &operator<<(const TLSecureValueError &secureValueErrorValue);
    CTelegramStream &operator<<(const TLSecureValueHash &secureValueHashValue);
    CTelegramStream &operator<<(const TLInputBotInlineMessage &inputBotInlineMessageValue);
    CTelegramStream &operator<<(const TLInputBotInlineResult &inputBotInlineResultValue);
    CTelegramStream &operator<<(const TLInputMedia &inputMediaValue);
    CTelegramStream &operator<<(const TLInputSingleMedia &inputSingleMediaValue);
    // End of generated write operators
};

inline CTelegramStream &CTelegramStream::operator>>(QString &str)
{
    QByteArray data;
    *this >> data;
    str = QString::fromUtf8(data);
    return *this;
}

inline CTelegramStream &CTelegramStream::operator<<(const QString &str)
{
    *this << str.toUtf8();
    return *this;
}

inline CTelegramStream &CTelegramStream::operator>>(bool &data)
{
    TLValue val;
    *this >> val;

    if (val == TLValue::BoolTrue) {
        data = true;
    } else if (val == TLValue::BoolFalse) {
        data = false;
    }

    return *this;
}

inline CTelegramStream &CTelegramStream::operator<<(const bool &data)
{
    if (data) {
        *this << TLValue::BoolTrue;
    } else {
        *this << TLValue::BoolFalse;
    }

    return *this;
}

inline CTelegramStream &CTelegramStream::operator>>(TLValue &v)
{
    quint32 i;
    *this >> i;
    v = TLValue(i);
    return *this;
}

inline CTelegramStream &CTelegramStream::operator<<(TLValue v)
{
    *this << quint32(v);
    return *this;
}

#endif // CTELEGRAMSTREAM_HPP
