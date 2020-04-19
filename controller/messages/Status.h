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

class State final : public MessageBase {
public:
    State() = default;
    State(QList<PlayerState> t_players);
    [[nodiscard]] MessageType messageType() const override;

public:
    QList<PlayerState> players;
};

using StateShp = QSharedPointer<State>;

template <>
inline QJsonObject toJsonObject(const State& t_object)
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
