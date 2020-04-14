#pragma once

#include <QWebSocket>
#include <QWebSocketServer>
#include <memory>

namespace network {

class Server : public QObject {
    Q_OBJECT
public:
    enum class ServerStatus {
        Stopped,
        Started,
    };

    void startListen();
    void notifyPlayer(const QString& t_clientKey, const QString& t_message);

signals:
    void connected(const QString& t_clientUid);
    void hasMessage(const QString& t_clientUid, const QString& t_message);
    void disconnected(const QString& t_clientUid);

private:
    void newUser();
    void processTextMessage(const QString& t_message);
    void socketDisconnected();

private:
    ServerStatus m_serverStatus {ServerStatus::Stopped};
    std::unique_ptr<QWebSocketServer> m_server;
    QHash<QString, QWebSocket*> m_clients;
    QHash<QWebSocket*, QString> m_clientToClientUid;
};

using ServerShp = QSharedPointer<Server>;

}
