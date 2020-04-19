#include "Controller.h"

#include <QJsonDocument>

using namespace controller;

GameController::GameController(const network::ServerShp& t_server, QObject* t_parent)
    : QObject {t_parent}
    , m_server {t_server}
{
    connect(m_server.data(), &network::Server::hasMessage, this, &GameController::processMessage);
    connect(m_server.data(), &network::Server::connected, this, &GameController::processConnected);
    connect(m_server.data(), &network::Server::disconnected, this, &GameController::processDisconnected);
}

void GameController::notifyPlayer(const QString& t_playerUid, const messages::MessageBaseShp& t_message)
{
    QJsonDocument document;
    document.setObject(messages::MessageBase::serialize(t_message));
    const auto byteArray = document.toJson(QJsonDocument::Compact);
    m_server->notifyPlayer(t_playerUid, QString(byteArray));
}

void GameController::notifyPlayers(const QSharedPointer<QList<QString>>& t_playersUids, const messages::MessageBaseShp& t_message)
{
    QJsonDocument document; //rapidjson
    document.setObject(messages::MessageBase::serialize(t_message));
    const auto message = QString(document.toJson(QJsonDocument::Compact));

    for (const auto& playerUid : *t_playersUids) {
        m_server->notifyPlayer(playerUid, message);
    }
}

void GameController::processConnected(const QString& t_playerUid)
{
    emit playerConnected(t_playerUid);
}

void GameController::processDisconnected(const QString& t_playerUid)
{
    emit playerDissconnected(t_playerUid);
}

void GameController::processMessage(const QString& t_playerUid, const QString& t_message)
{
    using namespace messages;

    const auto document = QJsonDocument::fromJson(t_message.toLatin1());
    const auto jsonObject = document.object();

    if (const auto command = messages::MessageBase::deserialize(jsonObject); command) {
        emit hasCommand(t_playerUid, command);
    }
}
