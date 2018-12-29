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

#include "TelegramServerUser.hpp"
#include "DcConfiguration.hpp"
#include "LocalCluster.hpp"
#include "Session.hpp"

#include "Utils.hpp"
#include <QCoreApplication>
#include <QDebug>
#include <QStandardPaths>

using namespace Telegram::Server;

#ifdef USE_DBUS_NOTIFIER
#include <QDBusConnection>
#include <QDBusMessage>

#include "ServerApi.hpp"
#include "DefaultAuthorizationProvider.hpp"

class DBusCodeAuthProvider : public Authorization::DefaultProvider
{
protected:
    Authorization::Code generateCode(Session *session, const QString &identifier) override;
};

Authorization::Code DBusCodeAuthProvider::generateCode(Session *session, const QString &identifier)
{
    Authorization::Code code = DefaultProvider::generateCode(session, identifier);
    QDBusMessage message = QDBusMessage::createMethodCall(QStringLiteral("org.freedesktop.Notifications"),
                                                          QStringLiteral("/org/freedesktop/Notifications"),
                                                          QStringLiteral("org.freedesktop.Notifications"),
                                                          QStringLiteral("Notify"));
    message.setArguments({
                             QCoreApplication::applicationName(),
                             QVariant::fromValue(0u),
                             QString(),
                             QStringLiteral("New auth code request"),
                             QStringLiteral("Auth code for account %1 is %2. Peer IP: %3").arg(identifier, code.code, session->ip),
                             QStringList(),
                             QVariantMap(),
                             3000
                         });

    // QString app_name, uint replaces_id, QString app_icon, QString summary,
    // QString body, QStringList actions, QVariantMap hints, int timeout
    QDBusConnection::sessionBus().send(message);
    return code;
}
#endif // USE_DBUS_NOTIFIER

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName(QStringLiteral("TelegramQt Server"));
    Telegram::initialize();

    Telegram::DcConfiguration configuration;
    const QVector<Telegram::DcOption> dcOptions = {
        Telegram::DcOption(QStringLiteral("127.0.0.1"), 11441, 1),
        Telegram::DcOption(QStringLiteral("127.0.0.2"), 11442, 2),
        Telegram::DcOption(QStringLiteral("127.0.0.3"), 11443, 3),
    };
    configuration.dcOptions = dcOptions;

    const Telegram::RsaKey key = Telegram::Utils::loadRsaPrivateKeyFromFile(
                QStandardPaths::standardLocations(QStandardPaths::HomeLocation).first()
                + QStringLiteral("/TelegramServer/private_key.pem"));
    if (!key.isValid()) {
        qCritical() << "Unable to read RSA key. Please read README.md for more information.";
        return -1;
    }

    LocalCluster cluster;
    cluster.setServerPrivateRsaKey(key);
    cluster.setServerConfiguration(configuration);
#ifdef USE_DBUS_NOTIFIER
    DBusCodeAuthProvider authProvider;
    cluster.setAuthorizationProvider(&authProvider);
    qInfo() << "DBus auth code provider enabled";
#endif
    cluster.start();

    if (User *u = cluster.addUser(QStringLiteral("+79222988314"), /* dc */ 1)) {
        u->setFirstName(QStringLiteral("Dc1User1"));
        u->setLastName(QStringLiteral("Dc1"));
        u->setPlainPassword(QStringLiteral("mypassword"));
    }

    if (User *u = cluster.addUser(QStringLiteral("6432101"), /* dc */ 1)) {
        u->setFirstName(QStringLiteral("Dc1User2"));
        u->setLastName(QStringLiteral("Dc1"));
        u->setPlainPassword(QStringLiteral("mypassword"));
    }

    if (User *u = cluster.addUser(QStringLiteral("5432102"), /* dc */ 2)) {
        u->setFirstName(QStringLiteral("Dc2User1"));
        u->setLastName(QStringLiteral("Dc2"));
    }

    if (User *u = cluster.addUser(QStringLiteral("5432103"), /* dc */ 3)) {
        u->setFirstName(QStringLiteral("Dc3User1"));
        u->setLastName(QStringLiteral("Dc3"));
        u->setPlainPassword(QStringLiteral("hispassword"));
    }

    User *user1dc1 = cluster.getUser(QStringLiteral("+79222988314"));
    User *user2dc1 = cluster.getUser(QStringLiteral("6432101"));
    User *user3dc2 = cluster.getUser(QStringLiteral("5432102"));
    user1dc1->importContact(user2dc1->toContact());
    user1dc1->importContact(user3dc2->toContact());
    user2dc1->importContact(user1dc1->toContact());
    user3dc2->importContact(user1dc1->toContact());

    User *admin = cluster.addUser(QStringLiteral("admin"), /* dc */ 3);
    admin->setFirstName(QStringLiteral("admin"));
    admin->setLastName(QStringLiteral("admin"));
    admin->setUserName(QStringLiteral("admin"));
    admin->importContact(user1dc1->toContact());

    return a.exec();
}
