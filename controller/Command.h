#pragma once

#include <QJsonObject>
#include <QSharedPointer>

namespace controller::command {

template <class T>
[[nodiscard]] QSharedPointer<T> fromJsonObject(const QJsonObject& t_jsonObject);

class Command {
public:
    enum class CommandType {
        Move,
        Fire,
        Rename,
    };

    [[nodiscard]] virtual CommandType commandType() const = 0;
};

using CommandShp = QSharedPointer<Command>;

class CommandMove final : public Command {
public:
    CommandMove(double t_x, double t_y)
        : x {t_x}
        , y {t_y}
    {
    }

    [[nodiscard]] CommandType commandType() const override
    {
        return CommandType::Move;
    }

public:
    double x;
    double y;
};

using CommandMoveShp = QSharedPointer<CommandMove>;

template <>
[[nodiscard]] inline CommandMoveShp fromJsonObject(const QJsonObject& t_jsonObject)
{
    return CommandMoveShp::create(t_jsonObject["x"].toDouble(), t_jsonObject["y"].toDouble());
}

class CommandRename final : public Command {
public:
    CommandRename(const QString& t_name)
        : name {t_name}
    {
    }

    [[nodiscard]] CommandType commandType() const override
    {
        return CommandType::Rename;
    }

public:
    QString name;
};

using CommandRenameShp = QSharedPointer<CommandRename>;

class CommandFire final : public Command {
public:
    CommandFire(double t_x, double t_y, double t_radians)
        : x {t_x}
        , y {t_y}
        , radians {t_radians}
    {
    }

    [[nodiscard]] CommandType commandType() const override
    {
        return CommandType::Fire;
    }

public:
    double x;
    double y;
    double radians;
};

using CommandFireShp = QSharedPointer<CommandFire>;

template <>
[[nodiscard]] inline CommandFireShp fromJsonObject(const QJsonObject& t_jsonObject)
{
    const auto jsonObject = t_jsonObject["data"].toObject();
    return CommandFireShp::create(jsonObject["x"].toDouble(), jsonObject["y"].toDouble(), jsonObject["route"].toDouble());
}

template <>
[[nodiscard]] inline QSharedPointer<Command> fromJsonObject(const QJsonObject& t_jsonObject)
{
    if (const auto type = t_jsonObject["type"].toString(); type == "shot") {
        return qSharedPointerCast<Command>(fromJsonObject<CommandFire>(t_jsonObject["data"].toObject()));
    } else if (type == "move") {
        return qSharedPointerCast<Command>(fromJsonObject<CommandMove>(t_jsonObject["data"].toObject()));
    } else if (type == "rename") {
        return qSharedPointerCast<Command>(CommandRenameShp::create(t_jsonObject["data"].toString()));
    } else {
        return nullptr;
    }
}
}
