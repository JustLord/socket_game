#pragma once

#include <QObject>

#include "../network/Server.h"
#include "Command.h"
#include "Notifications.h"

namespace controller {

class GameController : public QObject {
    Q_OBJECT
public:
    GameController(const network::ServerShp& t_server, QObject* t_parent = nullptr);
    void notifyPlayer(const QString& t_clientKey, const notification::GameStatusShp& t_status);

signals:
    void playerConnected(const QString& t_playerUid);
    void playerDissconnected(const QString& t_playerUid);
    void hasCommand(const QString& t_playerUid, const command::CommandShp& t_command);

private:
    void processConnected(const QString& t_playerUid);
    void processDisconnected(const QString& t_playerUid);
    void processMessage(const QString& t_playerUid, const QString& t_message);

private:
    network::ServerShp m_server;
    QSet<QString> m_playersWithName;
};

using GameControllerShp = QSharedPointer<GameController>;
}
