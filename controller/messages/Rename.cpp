#include "Rename.h"

using namespace controller::messages;

REGISTRY_DESERIALIZER(MessageType::Rename, [](const QJsonObject& t_jsonObject) {
    return qSharedPointerCast<MessageBase>(fromJsonObject<Rename>(t_jsonObject));
})

Rename::Rename(QString t_name)
    : name {std::move(t_name)}
{
}
