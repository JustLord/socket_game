#pragma once

#include "MessageBase.h"

namespace controller::messages {

struct Rename final : public MessageBase {
    Rename() = default;
    Rename(QString t_name);
    QString name;
    MessageType messageType() const override { return MessageType::Rename; }
};

using RenameShp = QSharedPointer<Rename>;

template <>
[[nodiscard]] inline RenameShp fromJsonObject(const QJsonObject& t_jsonObject)
{
    return RenameShp::create(t_jsonObject["data"].toString());
}

}
