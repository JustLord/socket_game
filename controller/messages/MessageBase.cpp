#include "MessageBase.h"

#include <QMap>

using namespace controller::messages;

MessageType MessageBase::messageType() const
{
    return MessageType::Unknown;
}

MessageType MessageBase::parseMessageType(const QString& t_string)
{
    static const QMap<QString, MessageType> messageType {
        {"move", MessageType::Move},
        {"shot", MessageType::Shot},
        {"state", MessageType::State},
        {"rename", MessageType::Rename}};

    return messageType[t_string];
}

std::unordered_map<MessageType, std::function<QJsonObject(const QSharedPointer<MessageBase>&)>> MessageBase::m_serializers;
std::unordered_map<MessageType, std::function<QSharedPointer<MessageBase>(const QJsonObject&)>> MessageBase::m_deserializers;

void MessageBase::registrySerializer(MessageType t_messageType, std::function<QJsonObject(const QSharedPointer<MessageBase>&)> t_fun)
{
    m_serializers[t_messageType] = std::move(t_fun);
}

void MessageBase::registryDeserializer(MessageType t_messageType, std::function<QSharedPointer<MessageBase>(const QJsonObject&)> t_fun)
{
    m_deserializers[t_messageType] = std::move(t_fun);
}

QJsonObject MessageBase::serialize(const QSharedPointer<MessageBase>& t_message)
{
    return m_serializers[t_message->messageType()](t_message);
}

QSharedPointer<MessageBase> MessageBase::deserialize(const QJsonObject& t_jsonObject)
{
    if (const auto type = t_jsonObject["type"].toString(); !type.isEmpty()) {
        return m_deserializers[parseMessageType(type)](t_jsonObject);
    }

    return nullptr;
}
