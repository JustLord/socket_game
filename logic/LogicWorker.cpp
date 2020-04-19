#include "LogicWorker.h"

#include "../controller/messages/Move.h"
#include "../controller/messages/Rename.h"
#include "../controller/messages/Shot.h"
#include "../controller/messages/Status.h"

using namespace controller::messages;
using namespace logic;

LogicWorker::LogicWorker(QObject* parent)
    : QObject(parent)
{
}

void LogicWorker::run()
{
}

void logic::LogicWorker::processPlayerConnect(const QString& t_playerKey)
{
    m_playersKeys.push_back(t_playerKey);
    m_players.insert(t_playerKey, {});
    m_players[t_playerKey].name = t_playerKey;
}

void LogicWorker::processPlayerDisconnect(const QString& t_playerKey)
{
    m_playersKeys.removeOne(t_playerKey);
    m_players.remove(t_playerKey);
}

void LogicWorker::processCommand(const QString& t_playerKey, const MessageBaseShp& t_message)
{
    switch (t_message->messageType()) {
    case MessageType::Move:
        processPlayerMove(t_playerKey, qSharedPointerCast<Move>(t_message));
        break;
    case MessageType::Shot:
        processFire(t_playerKey, qSharedPointerCast<Shot>(t_message));
        break;
    case MessageType::Rename:
        processPlayerRename(t_playerKey, qSharedPointerCast<Rename>(t_message));
        break;
    default:
        break;
    }
}

void LogicWorker::processPlayerMove(const QString& t_playerKey, const controller::messages::MoveShp& t_playerMove)
{
    auto& player = m_players[t_playerKey];
    player.x = t_playerMove->x;
    player.y = t_playerMove->y;

    notifyAllPlayers(qSharedPointerCast<controller::messages::MessageBase>(generateStatus()));
    m_needUpdate = true;
}

void LogicWorker::processPlayerRename(const QString& t_playerKey, const controller::messages::RenameShp& t_rename)
{
    m_players[t_playerKey].name = t_rename->name;
}

void LogicWorker::processFire(const QString& t_playerKey, const controller::messages::ShotShp& t_fire)
{
    Q_UNUSED(t_playerKey)
    m_bullets.push_back({t_fire->x, t_fire->y, t_fire->route, t_fire->damage});
    notifyAllPlayers(qSharedPointerCast<controller::messages::MessageBase>(t_fire));
}

controller::messages::StateShp LogicWorker::generateStatus()
{
    auto status = controller::messages::StateShp::create();
    for (const auto& key : m_players.keys()) {
        const auto& player = m_players[key];
        status->players.push_back({player.x, player.y, player.name, key});
    }

    return status;
}

void LogicWorker::notifyAllPlayers(const MessageBaseShp& t_message)
{
    emit notifyPlayers(QSharedPointer<QList<QString>>::create(m_playersKeys), t_message);
}
