TEMPLATE = app

QT += qml quick gamepad
CONFIG += c++11

SOURCES += engine/main.cpp \
    engine/sceneobject.cpp \
    components/sprite.cpp \
    components/starfighter.cpp \
    components/ship.cpp \
    engine/engine.cpp \
    components/health.cpp \
    components/asteroid.cpp \
    components/weapon.cpp \
    components/ui.cpp \
    components/preloadasset.cpp \
    components/pickup.cpp \
    components/score.cpp \
    components/dabomb.cpp

INCLUDEPATH += engine

RESOURCES += \
    assets.qrc

HEADERS += \
    engine/sceneobject.h \
    components/sprite.h \
    components/starfighter.h \
    components/ship.h \
    engine/engine.h \
    components/sfcomponent.h \
    components/health.h \
    components/asteroid.h \
    components/weapon.h \
    components/ui.h \
    components/preloadasset.h \
    components/pickup.h \
    components/score.h \
    components/dabomb.h

DISTFILES += \
    assets/ship.png \
    assets/kitten.png \
    assets/flame.png
