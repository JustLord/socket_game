#pragma once

#include "../controller/Controller.h"
#include "../controller/messages/Move.h"
#include "../controller/messages/Rename.h"
#include "../controller/messages/Shot.h"
#include "../controller/messages/Status.h"
#include "Bullet.h"
#include "Player.h"

namespace logic {

class Game : public QObject {
    Q_OBJECT
public:
    Game(const controller::GameControllerShp& t_controller, QObject* t_parent = nullptr);

private slots:
    void processPlayerConnect(const QString& t_playerKey);
    void processPlayerDisconnect(const QString& t_playerKey);
    void processCommand(const QString& t_playerKey, const controller::messages::MessageBaseShp& t_message);

signals:
    void notifyPlayers(const QSharedPointer<QList<QString>>& t_players, const controller::messages::MessageBaseShp& t_message);

private:
    void updatePlayerState(const QString& t_playerKey, const controller::messages::MoveShp& t_playerMove);
    void fire(const QString& t_playerKey, const controller::messages::ShotShp& t_fire);
    void renamePlayer(const QString& t_playerKey, const controller::messages::RenameShp& t_rename);
    controller::messages::GameStatusShp generateStatus();
    void notifyAllPlayers(const controller::messages::MessageBaseShp& t_message);

private:
    QList<Bullet> m_bullets;
    QList<QString> m_playersKeys;
    QHash<QString, Player> m_players;
    controller::GameControllerShp m_controller;
};

using GameShp = QSharedPointer<Game>;

}
