#pragma once

#include "../controller/Controller.h"
#include "Bullet.h"
#include "Player.h"

namespace logic {

class Game : public QObject {
public:
    Game(const controller::GameControllerShp& t_controller);

private:
    void processPlayerConnect(const QString& t_playerKey);
    void processPlayerDisconnect(const QString& t_playerKey);
    void processCommand(const QString& t_playerKey, const controller::command::CommandShp& t_command);
    void updatePlayerState(const QString& t_playerKey, const controller::command::CommandMoveShp& t_playerMove);
    void fire(const QString& t_playerKey, const controller::command::CommandFireShp& t_fire);
    void renamePlayer(const QString& t_playerKey, const controller::command::CommandRenameShp& t_rename);
    controller::notification::GameStatusShp generateStatus();
    void updateAllPlayers();

    void startLogic();

private:
    QList<Bullet> m_bullets;
    QHash<QString, Player> m_players;
    controller::GameControllerShp m_controller;
};

using GameShp = QSharedPointer<Game>;

}
