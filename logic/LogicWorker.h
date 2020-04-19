#pragma once

#include <QObject>

class LogicWorker : public QObject
{
    Q_OBJECT
public:
    explicit LogicWorker(QObject *parent = nullptr);

signals:

};

