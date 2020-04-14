#pragma once

#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QSharedPointer>

#include "global.h"

namespace controller::notification {

struct PlayerStatus {
    double x;
    double y;
    QString name;
    QString uid;
};

struct GameStatus {
    QList<PlayerStatus> players;
};

using GameStatusShp = QSharedPointer<GameStatus>;

template <class T>
QJsonObject toJsonObject(const QSharedPointer<T>& t_ptr)
{
    return toJsonObject<T>(*t_ptr);
}

template <class T>
QJsonObject toJsonObject(const T& t_object);

template <>
inline QJsonObject toJsonObject(const PlayerStatus& t_object)
{
    QJsonObject jsonObject;
    jsonObject.insert("x", t_object.x);
    jsonObject.insert("y", t_object.y);
    jsonObject.insert("name", t_object.name);
    jsonObject.insert("uid", t_object.uid);

    return jsonObject;
}

template <>
inline QJsonObject toJsonObject(const GameStatus& t_object)
{
    QJsonArray jsonArray;
    for (const auto& playerStatus : t_object.players) {
        jsonArray.push_back(toJsonObject(playerStatus));
    }

    QJsonObject jsonObject;
    jsonObject.insert("type", "state");

    QJsonObject data;
    data.insert("players", jsonArray);

    jsonObject.insert("data", data);
    return jsonObject;
}

}
