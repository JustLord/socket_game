#include <QCoreApplication>
#include <QTimer>

#include "logic/Game.h"
#include "network/Server.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    const auto server = network::ServerShp::create();
    const auto game = logic::GameShp::create(controller::GameControllerShp::create(server));

    QTimer::singleShot(0, [&] { server->startListen(); });
    return a.exec();
}
