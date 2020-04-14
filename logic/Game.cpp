#include "Game.h"

#include <QThread>
#include <QtConcurrent>

using namespace controller::command;
using namespace controller::notification;
using namespace controller;
using namespace logic;

Game::Game(const GameControllerShp& t_controller)
    : m_controller {t_controller}
{
    connect(m_controller.data(), &GameController::playerConnected, this, &Game::processPlayerConnect);
    connect(m_controller.data(), &GameController::playerDissconnected, this, &Game::processPlayerDisconnect);
    connect(m_controller.data(), &GameController::hasCommand, this, &Game::processCommand);
}

void Game::processPlayerConnect(const QString& t_playerKey)
{
    m_players.insert(t_playerKey, {});
    m_players[t_playerKey].name = t_playerKey;
}

void Game::processPlayerDisconnect(const QString& t_playerKey)
{
    m_players.remove(t_playerKey);
}

void Game::processCommand(const QString& t_playerKey, const CommandShp& t_command)
{
    switch (t_command->commandType()) {
    case Command::CommandType::Move:
        updatePlayerState(t_playerKey, qSharedPointerCast<CommandMove>(t_command));
        break;
    case Command::CommandType::Fire:
        fire(t_playerKey, qSharedPointerCast<CommandFire>(t_command));
        break;
    case Command::CommandType::Rename:
        renamePlayer(t_playerKey, qSharedPointerCast<CommandRename>(t_command));
        break;
    default:
        break;
    }
}

void Game::updatePlayerState(const QString& t_playerKey, const CommandMoveShp& t_playerMove)
{
    auto& player = m_players[t_playerKey];
    player.x = t_playerMove->x;
    player.y = t_playerMove->y;

    updateAllPlayers();
}

void Game::fire(const QString& t_playerKey, const CommandFireShp& t_fire)
{
    Q_UNUSED(t_playerKey)
    m_bullets.push_back({t_fire->x, t_fire->y, t_fire->radians});
}

void Game::renamePlayer(const QString& t_playerKey, const CommandRenameShp& t_rename)
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

void Game::updateAllPlayers()
{
    const auto status = generateStatus();
    for (const auto& key : m_players.keys()) {
        m_controller->notifyPlayer(key, status);
    }
}

void Game::startLogic()
{
    //QtConcurrent::run([]{})
}
