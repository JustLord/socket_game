#include <QString>

#pragma once

namespace logic {

struct Bullet {
    Bullet() = default;
    Bullet(const QString& t_fromUser, double t_x, double t_y, double t_route, int t_damage)
        : fromUser {t_fromUser}
        , x {t_x}
        , y {t_y}
        , route {t_route}
        , damage {t_damage}
    {
    }

    QString fromUser;
    double x {};
    double y {};
    double route {};
    int damage {};
    bool isNew {true};
};

}
