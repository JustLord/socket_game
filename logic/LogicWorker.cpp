#include "LogicWorker.h"

#include <QtConcurrent>

#include "../controller/messages/Move.h"
#include "../controller/messages/Rename.h"
#include "../controller/messages/Shot.h"
#include "../controller/messages/Status.h"

using namespace controller::messages;
using namespace logic;

constexpr auto playerSize = 0.05;

LogicWorker::LogicWorker(QObject* parent)
    : QObject(parent)
    , m_timer {std::make_unique<QTimer>(this)}
{
    connect(m_timer.get(), &QTimer::timeout, this, &LogicWorker::logicIteration);
}

void LogicWorker::run()
{
    m_timer->start(10);
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

void LogicWorker::logicIteration()
{
    auto future = QtConcurrent::map(m_bullets, [&](Bullet& t_bullet) {
        const auto collisionWithPlayer = [&](const QString& t_player) {
            const auto& player = m_players[t_player];
            return player.x - playerSize / 2 <= t_bullet.x
                && player.x + playerSize / 2 >= t_bullet.x
                && player.y - playerSize / 2 <= t_bullet.y
                && player.y + playerSize / 2 >= t_bullet.y;
        };

        t_bullet.x += cos(t_bullet.route) * 0.02;
        t_bullet.y += sin(t_bullet.route) * 0.02;

        if (t_bullet.x < 0 || t_bullet.x > 1 || t_bullet.y < 0 || t_bullet.y > 1) {
            t_bullet.damage = -1;
            return;
        }

        if (t_bullet.isNew && !collisionWithPlayer(t_bullet.fromUser)) {
            t_bullet.isNew = false;
        }

        if (t_bullet.isNew) {
            return;
        }

        for (const auto& playerKey : m_playersKeys) {
            if (collisionWithPlayer(playerKey)) {
                _ = std::unique_lock(m_callisionMutex);
                m_callisions.push_back({t_bullet.fromUser, playerKey, t_bullet.damage});
                t_bullet.damage = -1;
                m_needUpdate = true;
                break;
            }
        }
    });

    future.waitForFinished();

    if (!m_needUpdate) {
        return;
    }

    auto removeBulletFuture = QtConcurrent::run([&] {
        if (m_bullets.empty()) {
            return;
        }

        const auto end = std::remove_if(m_bullets.begin(), m_bullets.end(), [](const auto& t_bullet) {
            return t_bullet.damage == -1;
        });
        m_bullets.erase(end);
    });

    notifyAllPlayers(qSharedPointerCast<controller::messages::MessageBase>(generateStatus()));
    m_needUpdate = false;

    if (!m_callisions.empty()) {
        qDebug() << QStringLiteral("Есть попадание!");
    }

    m_callisions.clear();

    removeBulletFuture.waitForFinished();
}

void LogicWorker::processPlayerMove(const QString& t_playerKey, const controller::messages::MoveShp& t_playerMove)
{
    auto& player = m_players[t_playerKey];
    if (player.x != t_playerMove->x || player.y != t_playerMove->y) {
        player.x = t_playerMove->x;
        player.y = t_playerMove->y;
        m_needUpdate = true;
    }
}

void LogicWorker::processPlayerRename(const QString& t_playerKey, const controller::messages::RenameShp& t_rename)
{
    m_players[t_playerKey].name = t_rename->name;
}

void LogicWorker::processFire(const QString& t_playerKey, const controller::messages::ShotShp& t_fire)
{
    m_bullets.emplace_back(t_playerKey, t_fire->x, t_fire->y, t_fire->route, t_fire->damage);
    notifyAllPlayers(qSharedPointerCast<controller::messages::MessageBase>(t_fire));
}

controller::messages::StateShp LogicWorker::generateStatus()
{
    auto status = controller::messages::StateShp::create();
    for (const auto& key : m_players.keys()) {
        const auto& player = m_players[key];
        status->players.push_back({player.x, player.y, player.name, key});
    }

    for (const auto& callision : m_callisions) {
        status->damages.push_back({callision.fromUser, callision.toUser, callision.damage});
    }

    return status;
}

void LogicWorker::notifyAllPlayers(const MessageBaseShp& t_message)
{
    emit notifyPlayers(QSharedPointer<QList<QString>>::create(m_playersKeys), t_message);
}
