#pragma once

#include "MessageBase.h"

namespace controller::messages {

struct Shot final : public MessageBase {
    Shot() = default;
    Shot(double t_x, double t_y, double t_route, int t_damage);
    double x;
    double y;
    double route;
    int damage;
    MessageType messageType() const override { return MessageType::Shot; }
};

using ShotShp = QSharedPointer<Shot>;

template <>
[[nodiscard]] inline ShotShp fromJsonObject(const QJsonObject& t_jsonObject)
{
    const auto data = t_jsonObject["data"];
    return ShotShp::create(
        data["x"].toDouble(),
        data["y"].toDouble(),
        data["route"].toDouble(),
        data["damage"].toInt());
}

template <>
inline QJsonObject toJsonObject(const Shot& t_object)
{
    QJsonObject jsonObject;
    jsonObject.insert("type", "shot");

    QJsonObject data;
    data.insert("x", t_object.x);
    data.insert("y", t_object.y);
    data.insert("route", t_object.route);
    data.insert("damage", t_object.damage);

    jsonObject.insert("data", data);
    return jsonObject;
}

}
