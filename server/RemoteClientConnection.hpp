#ifndef TELEGRAM_SERVER_CLIENT_CONNECTION_HPP
#define TELEGRAM_SERVER_CLIENT_CONNECTION_HPP

#include <QObject>
#include <QVector>

#include "Connection.hpp"

namespace Telegram {

namespace Server {

class ServerApi;
class RpcLayer;
class RpcOperationFactory;
class Session;

class RemoteClientConnection : public BaseConnection
{
    Q_OBJECT
public:
    explicit RemoteClientConnection(QObject *parent = nullptr);

    RpcLayer *rpcLayer() const;

    void setRpcFactories(const QVector<RpcOperationFactory*> &rpcFactories);

    ServerApi *api() const;
    void setServerApi(ServerApi *api);

    Session *session() const;
    void setSession(Session *session);

protected slots:
    void onClientDhStateChanged();

    void sendKeyError();

};

} // Server

} // Telegram

#endif // TELEGRAM_SERVER_CLIENT_CONNECTION_HPP
