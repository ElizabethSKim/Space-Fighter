TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += engine/main.cpp \
    engine/scenegraph.cpp \
    engine/scenegraphwindow.cpp \
    engine/sceneobject.cpp \
    components/sprite.cpp \
    components/starfighter.cpp \
    components/kitties.cpp

INCLUDEPATH += engine

RESOURCES += assets.qrc

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    engine/scenegraph.h \
    engine/sceneobject.h \
    components/sprite.h \
    components/starfighter.h \
    components/kitties.h

DISTFILES +=
