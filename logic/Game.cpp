#include "Game.h"

#include <QThread>
#include <QtConcurrent>

using namespace controller::messages;
using namespace controller;
using namespace logic;

Game::Game(const GameControllerShp& t_controller, QObject* t_parent)
    : QObject(t_parent)
    , m_controller {t_controller}
{
    qRegisterMetaType<QSharedPointer<controller::messages::MessageBase>>("QSharedPointer<controller::messages::MessageBase>");
    qRegisterMetaType<controller::messages::MessageBaseShp>("controller::messages::MessageBaseShp");
    qRegisterMetaType<QSharedPointer<QList<QString>>>("QSharedPointer<QList<QString>>");
    connect(m_controller.data(), &GameController::playerConnected, this, &Game::processPlayerConnect);
    connect(m_controller.data(), &GameController::playerDissconnected, this, &Game::processPlayerDisconnect);
    connect(m_controller.data(), &GameController::hasCommand, this, &Game::processCommand);
    connect(this, &Game::notifyPlayers, m_controller.data(), &GameController::notifyPlayers);
}

void Game::processPlayerConnect(const QString& t_playerKey)
{
    m_players.insert(t_playerKey, {});
    m_players[t_playerKey].name = t_playerKey;
}

void Game::processPlayerDisconnect(const QString& t_playerKey)
{
    m_players.remove(t_playerKey);
    m_playersKeys.removeOne(t_playerKey);
}

void Game::processCommand(const QString& t_playerKey, const MessageBaseShp& t_message)
{
    switch (t_message->messageType()) {
    case MessageType::Move:
        updatePlayerState(t_playerKey, qSharedPointerCast<Move>(t_message));
        break;
    case MessageType::Shot:
        fire(t_playerKey, qSharedPointerCast<Shot>(t_message));
        break;
    case MessageType::Rename:
        renamePlayer(t_playerKey, qSharedPointerCast<Rename>(t_message));
        break;
    default:
        break;
    }
}

void Game::updatePlayerState(const QString& t_playerKey, const MoveShp& t_playerMove)
{
    auto& player = m_players[t_playerKey];
    player.x = t_playerMove->x;
    player.y = t_playerMove->y;

    notifyAllPlayers(qSharedPointerCast<MessageBase>(generateStatus()));
}

void Game::fire(const QString& t_playerKey, const ShotShp& t_fire)
{
    Q_UNUSED(t_playerKey)
    //m_bullets.push_back({t_fire->x, t_fire->y, t_fire->route});
    notifyAllPlayers(qSharedPointerCast<MessageBase>(t_fire));
}

void Game::renamePlayer(const QString& t_playerKey, const RenameShp& t_rename)
{
    m_players[t_playerKey].name = t_rename->name;
}

GameStatusShp Game::generateStatus()
{
    auto status = GameStatusShp::create();
    for (const auto& key : m_players.keys()) {
        const auto& player = m_players[key];
        status->players.push_back({player.x, player.y, player.name, key});
    }

    return status;
}

void Game::notifyAllPlayers(const MessageBaseShp& t_message)
{
    const auto allPlayers = QSharedPointer<QList<QString>>::create(m_players.keys());
    emit notifyPlayers(allPlayers, t_message);
    //    m_controller->notifyPlayers(m_players.keys(), t_message);
}
