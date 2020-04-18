#pragma once

#include <QJsonObject>
#include <QSharedPointer>

#include "global.h"

namespace controller::messages {

enum class MessageType {
    Unknown,
    Move,
    Shot,
    Rename,
    State,
};

class MessageBase {
public:
    [[nodiscard]] virtual MessageType messageType() const;
    [[nodiscard]] static MessageType parseMessageType(const QString& t_string);
    static void registrySerializer(MessageType t_messageType, std::function<QJsonObject(const QSharedPointer<MessageBase>&)> t_fun);
    static void registryDeserializer(MessageType t_messageType, std::function<QSharedPointer<MessageBase>(const QJsonObject&)> t_fun);
    [[nodiscard]] static QJsonObject serialize(const QSharedPointer<MessageBase>& t_message);
    [[nodiscard]] static QSharedPointer<MessageBase> deserialize(const QJsonObject& t_jsonObject);

private:
    static std::unordered_map<MessageType, std::function<QJsonObject(const QSharedPointer<MessageBase>&)>> m_serializers;
    static std::unordered_map<MessageType, std::function<QSharedPointer<MessageBase>(const QJsonObject&)>> m_deserializers;
};

using MessageBaseShp = QSharedPointer<MessageBase>;

template <class T>
[[nodiscard]] QJsonObject toJsonObject(const T& t_object);

template <class T>
[[nodiscard]] QJsonObject toJsonObject(const QSharedPointer<T>& t_ptr)
{
    return toJsonObject<T>(*t_ptr);
}

template <class T>
[[nodiscard]] QSharedPointer<T> fromJsonObject(const QJsonObject& t_jsonObject);

struct SerializerRegistrator {
    template <class T>
    SerializerRegistrator(MessageType t_messageType, T t_function)
    {
        MessageBase::registrySerializer(t_messageType, std::move(t_function));
    }
};

#define REGISTRY_SERIALIZER(message, function) _ = SerializerRegistrator(message, function);

struct DeserializerRegistrator {
    template <class T>
    DeserializerRegistrator(MessageType t_messageType, T t_function)
    {
        MessageBase::registryDeserializer(t_messageType, std::move(t_function));
    }
};

#define REGISTRY_DESERIALIZER(message, function) _ = DeserializerRegistrator(message, function);

}
