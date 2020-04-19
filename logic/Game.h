#pragma once

#include <QThread>

#include "../controller/Controller.h"
#include "../controller/messages/Move.h"
#include "../controller/messages/Rename.h"
#include "../controller/messages/Shot.h"
#include "../controller/messages/Status.h"
#include "Bullet.h"
#include "LogicWorker.h"
#include "Player.h"

namespace logic {

class Game : public QObject {
    Q_OBJECT
public:
    explicit Game(const controller::GameControllerShp& t_controller, QObject* t_parent = nullptr);

private:
    controller::GameControllerShp m_controller;
    std::unique_ptr<LogicWorker> m_worker;
    QThread m_workerThread;
};

using GameShp = QSharedPointer<Game>;

}
