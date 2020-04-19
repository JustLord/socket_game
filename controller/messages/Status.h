#pragma once

#include <QJsonArray>

#include "MessageBase.h"

namespace controller::messages {

struct PlayerState {
    double x;
    double y;
    QString name;
    QString uid;
};

struct Damage {
    QString fromUser;
    QString toUser;
    int damage;
};

template <>
inline QJsonObject toJsonObject(const PlayerState& t_object)
{
    QJsonObject jsonObject;
    jsonObject.insert("x", t_object.x);
    jsonObject.insert("y", t_object.y);
    jsonObject.insert("name", t_object.name);
    jsonObject.insert("uid", t_object.uid);

    return jsonObject;
}

template <>
inline QJsonObject toJsonObject(const Damage& t_object)
{
    QJsonObject jsonObject;
    jsonObject.insert("from_user", t_object.fromUser);
    jsonObject.insert("to_user", t_object.toUser);
    jsonObject.insert("damage", t_object.damage);

    return jsonObject;
}

class State final : public MessageBase {
public:
    State() = default;
    State(QList<PlayerState> t_players, QList<Damage> t_damages);
    [[nodiscard]] MessageType messageType() const override;

public:
    QList<PlayerState> players;
    QList<Damage> damages;
};

using StateShp = QSharedPointer<State>;

template <>
inline QJsonObject toJsonObject(const State& t_object)
{
    QJsonArray jsonPlayers;
    QJsonArray jsonDamages;

    for (const auto& playerStatus : t_object.players) {
        jsonPlayers.push_back(toJsonObject(playerStatus));
    }

    for (const auto& damage : t_object.damages) {
        jsonDamages.push_back(toJsonObject(damage));
    }

    QJsonObject jsonObject;
    jsonObject.insert("type", "state");

    QJsonObject data;
    data.insert("players", jsonPlayers);
    if (!t_object.damages.empty()) {
        data.insert("damages", jsonDamages);
    }

    jsonObject.insert("data", data);
    return jsonObject;
}

}
