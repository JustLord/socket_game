QT -= gui
QT += network websockets concurrent

CONFIG += c++17 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    controller/messages/Damage.cpp \
    controller/messages/MessageBase.cpp \
    controller/messages/Move.cpp \
    controller/messages/Rename.cpp \
    controller/messages/Shot.cpp \
    controller/messages/Status.cpp \
    network/Server.cpp \
        main.cpp \
    logic/Game.cpp \
    controller/Controller.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    controller/messages/Damage.h \
    controller/messages/MessageBase.h \
    controller/messages/Move.h \
    controller/messages/Rename.h \
    controller/messages/Shot.h \
    controller/messages/Status.h \
    logic/Bullet.h \
    logic/Player.h \
    logic/Game.h \
    controller/Controller.h \
    network/Server.h \
    global.h
