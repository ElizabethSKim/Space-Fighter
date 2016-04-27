TEMPLATE = app

QT += qml quick gamepad
CONFIG += c++11

SOURCES += engine/main.cpp \
    engine/sceneobject.cpp \
    components/sprite.cpp \
    components/starfighter.cpp \
    components/kitties.cpp \
    components/ship.cpp \
    engine/engine.cpp

INCLUDEPATH += engine

RESOURCES += assets.qrc

HEADERS += \
    engine/sceneobject.h \
    components/sprite.h \
    components/starfighter.h \
    components/kitties.h \
    components/ship.h \
    engine/engine.h \
    components/sfcomponent.h

DISTFILES +=
