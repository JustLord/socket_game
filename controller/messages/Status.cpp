#include "Status.h"

using namespace controller::messages;

REGISTRY_SERIALIZER(MessageType::State, [](const QSharedPointer<MessageBase>& t_baseObject) {
    const auto object = qSharedPointerCast<State>(t_baseObject);
    return toJsonObject(object);
})

State::State(QList<PlayerState> t_players, QList<Damage> t_damages)
    : players {std::move(t_players)}
    , damages {std::move(t_damages)}
{
}

MessageType State::messageType() const
{
    return MessageType::State;
}
