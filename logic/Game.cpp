#include "Game.h"

#include <QMetaObject>
#include <QThread>
#include <QtConcurrent>

using namespace controller::messages;
using namespace controller;
using namespace logic;

Game::Game(const GameControllerShp& t_controller, QObject* t_parent)
    : QObject(t_parent)
    , m_controller {t_controller}
    , m_worker {std::make_unique<LogicWorker>()}
{
    qRegisterMetaType<QSharedPointer<controller::messages::MessageBase>>("QSharedPointer<controller::messages::MessageBase>");
    qRegisterMetaType<controller::messages::MessageBaseShp>("controller::messages::MessageBaseShp");
    qRegisterMetaType<QSharedPointer<QList<QString>>>("QSharedPointer<QList<QString>>");

    m_worker->moveToThread(&m_workerThread);

    connect(&m_workerThread, &QThread::started, m_worker.get(), &LogicWorker::run);

    connect(m_controller.data(), &GameController::playerConnected, m_worker.get(),
            &LogicWorker::processPlayerConnect);
    connect(m_controller.data(), &GameController::playerDissconnected, m_worker.get(),
            &LogicWorker::processPlayerDisconnect);
    connect(m_controller.data(), &GameController::hasCommand, m_worker.get(),
            &LogicWorker::processCommand);
    connect(m_worker.get(), &LogicWorker::notifyPlayers, m_controller.data(),
            &GameController::notifyPlayers);

    m_workerThread.start();
}
