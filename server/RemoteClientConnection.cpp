#include "RemoteClientConnection.hpp"

#include "ApiUtils.hpp"
#include "TLTypes.hpp"
#include "ConnectionError.hpp"
#include "CTelegramTransport.hpp"
#include "SendPackageHelper.hpp"
#include "ServerApi.hpp"
#include "ServerDhLayer.hpp"
#include "ServerRpcLayer.hpp"
#include "Session.hpp"
#include "TelegramServerUser.hpp"

#include <QDateTime>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(loggingCategoryRemoteClientConnection, "telegram.server.remoteclient.connection", QtInfoMsg)

namespace Telegram {

namespace Server {

class SendPackageHelper : public BaseSendPackageHelper
{
public:
    explicit SendPackageHelper(BaseConnection *connection) :
        BaseSendPackageHelper()
    {
        m_connection = connection;
    }

    quint64 newMessageId(SendMode mode) override
    {
        quint64 ts = Telegram::Utils::formatTimeStamp(QDateTime::currentMSecsSinceEpoch());
        ts &= ~quint64(3);
        if (mode == SendMode::ServerReply) {
            ts |= 1;
        } else if (mode == SendMode::ServerInitiative) {
            ts |= 3;
        } else {
            qCWarning(loggingCategoryRemoteClientConnection) << Q_FUNC_INFO << "Invalid mode";
        }
        return m_connection->transport()->getNewMessageId(ts);
    }

    void sendPackage(const QByteArray &package) override
    {
        return m_connection->transport()->sendPackage(package);
    }
};

RemoteClientConnection::RemoteClientConnection(QObject *parent) :
    BaseConnection(parent)
{
    m_sendHelper = new SendPackageHelper(this);
    m_dhLayer = new DhLayer(this);
    m_dhLayer->setSendPackageHelper(m_sendHelper);
    connect(m_dhLayer, &BaseDhLayer::stateChanged, this, &RemoteClientConnection::onClientDhStateChanged);
    m_rpcLayer = new RpcLayer(this);
    m_rpcLayer->setSendPackageHelper(m_sendHelper);
}

RpcLayer *RemoteClientConnection::rpcLayer() const
{
    return reinterpret_cast<RpcLayer*>(m_rpcLayer);
}

void RemoteClientConnection::setRpcFactories(const QVector<RpcOperationFactory *> &rpcFactories)
{
    rpcLayer()->setRpcFactories(rpcFactories);
}

ServerApi *RemoteClientConnection::api() const
{
    return rpcLayer()->api();
}

void RemoteClientConnection::setServerApi(ServerApi *api)
{
    rpcLayer()->setServerApi(api);
}

Session *RemoteClientConnection::session() const
{
    return rpcLayer()->session();
}

void RemoteClientConnection::setSession(Session *session)
{
    session->setConnection(this);
    rpcLayer()->setSession(session);
}

void RemoteClientConnection::onClientDhStateChanged()
{
    if (m_dhLayer->state() == BaseDhLayer::State::HasKey) {
        Session *session = api()->createSession(m_sendHelper->authId(), m_sendHelper->authKey(), m_transport->remoteAddress());
        session->lastSequenceNumber = 6;
        session->setInitialServerSalt(m_dhLayer->serverSalt());
        setSession(session);
    }
}

void RemoteClientConnection::sendKeyError()
{
    qCInfo(loggingCategoryRemoteClientConnection) << this << transport()->remoteAddress() << "InvalidAuthKey";
    m_transport->sendPackage(ConnectionError(ConnectionError::InvalidAuthKey).toByteArray());
}

} // Server

} // Telegram
