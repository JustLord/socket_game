#include "Move.h"

using namespace controller::messages;

REGISTRY_DESERIALIZER(MessageType::Move, [](const QJsonObject& t_jsonObject) {
    return qSharedPointerCast<MessageBase>(fromJsonObject<Move>(t_jsonObject));
})

Move::Move(double t_x, double t_y)
    : x {t_x}
    , y {t_y}
{
}
