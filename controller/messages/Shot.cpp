#include "Shot.h"

using namespace controller::messages;

REGISTRY_SERIALIZER(MessageType::Shot, [](const QSharedPointer<MessageBase>& t_baseObject) {
    const auto object = qSharedPointerCast<Shot>(t_baseObject);
    return toJsonObject(object);
})

REGISTRY_DESERIALIZER(MessageType::Shot, [](const QJsonObject& t_jsonObject) {
    return qSharedPointerCast<MessageBase>(fromJsonObject<Shot>(t_jsonObject));
})

Shot::Shot(double t_x, double t_y, double t_route, int t_damage)
    : x {t_x}
    , y {t_y}
    , route {t_route}
    , damage {t_damage}
{
}
