#pragma once

#include <QHash>
#include <QObject>

#include "Bullet.h"
#include "Player.h"

namespace controller::messages {

class Move;
using MoveShp = QSharedPointer<Move>;

class Shot;
using ShotShp = QSharedPointer<Shot>;

class Rename;
using RenameShp = QSharedPointer<Rename>;

class MessageBase;
using MessageBaseShp = QSharedPointer<MessageBase>;

class State;
using StateShp = QSharedPointer<State>;

}

namespace logic {

class LogicWorker : public QObject {
    Q_OBJECT
public:
    explicit LogicWorker(QObject* parent = nullptr);

public slots:
    void run();
    void processPlayerConnect(const QString& t_playerKey);
    void processPlayerDisconnect(const QString& t_playerKey);
    void processCommand(const QString& t_playerKey, const controller::messages::MessageBaseShp& t_message);

signals:
    void notifyPlayers(const QSharedPointer<QList<QString>>& t_players, const controller::messages::MessageBaseShp& t_message);

private:
    void processPlayerMove(const QString& t_playerKey, const controller::messages::MoveShp& t_playerMove);
    void processPlayerRename(const QString& t_playerKey, const controller::messages::RenameShp& t_rename);
    void processFire(const QString& t_playerKey, const controller::messages::ShotShp& t_fire);

    controller::messages::StateShp generateStatus();
    void notifyAllPlayers(const controller::messages::MessageBaseShp& t_message);

private:
    bool m_needUpdate;
    QList<Bullet> m_bullets;
    QList<QString> m_playersKeys;
    QHash<QString, Player> m_players;
};

}
