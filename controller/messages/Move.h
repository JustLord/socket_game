#pragma once

#include "MessageBase.h"

namespace controller::messages {

struct Move final : public MessageBase {
    Move() = default;
    Move(double t_x, double t_y);
    double x;
    double y;
    MessageType messageType() const override { return MessageType::Move; }
};

using MoveShp = QSharedPointer<Move>;

template <>
[[nodiscard]] inline MoveShp fromJsonObject(const QJsonObject& t_jsonObject)
{
    const auto data = t_jsonObject["data"];
    return MoveShp::create(data["x"].toDouble(), data["y"].toDouble());
}

}
