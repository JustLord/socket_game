#include "Server.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpSocket>
#include <QUuid>

using namespace network;

void Server::startListen()
{
    m_server = std::make_unique<QWebSocketServer>(QStringLiteral("Echo Server"), QWebSocketServer::NonSecureMode);
    connect(m_server.get(), &QWebSocketServer::newConnection, this, &Server::newUser);

    if (!m_server->listen(QHostAddress::Any, 1333) && m_serverStatus == ServerStatus::Stopped) {
        qDebug() << QObject::tr("Unable to start the server: %1.").arg(m_server->errorString());
    } else {
        m_serverStatus = ServerStatus::Started;
        qDebug() << m_server->isListening() << "TCPSocket listen on port";
        qDebug() << QString::fromUtf8("Сервер запущен!");
    }
}

void Server::notifyPlayer(const QString& t_clientKey, const QString& t_message)
{
    m_clients[t_clientKey]->sendTextMessage(t_message);
}

void Server::newUser()
{
    if (m_serverStatus == ServerStatus::Started) {
        qDebug() << QString::fromUtf8("У нас новое соединение!");
        const auto client = m_server->nextPendingConnection();
        connect(client, &QWebSocket::textMessageReceived, this, &Server::processTextMessage);
        connect(client, &QWebSocket::disconnected, this, &Server::socketDisconnected);

        const auto clientUid = QUuid::createUuid().toString();
        m_clientToClientUid.insert(client, clientUid);
        m_clients.insert(clientUid, client);

        emit connected(clientUid);
    }
}

void Server::processTextMessage(const QString& t_message)
{
    auto client = qobject_cast<QWebSocket*>(sender());
    const auto& clientUid = m_clientToClientUid[client];

    emit hasMessage(clientUid, t_message);
}

void Server::socketDisconnected()
{
    auto client = qobject_cast<QWebSocket*>(sender());
    qDebug() << "socketDisconnected:" << client;
    if (client) {
        const auto clientUid = m_clientToClientUid.take(client);
        m_clients.remove(clientUid);
        client->deleteLater();

        emit disconnected(clientUid);
    }
}
