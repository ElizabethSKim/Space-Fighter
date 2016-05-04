#include "engine.h"

#include "components/starfighter.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    Engine engine;
    engine.resize(SCENE_WIDTH,SCENE_HEIGHT);

    //object_cast<sf::StarFighter>(gamelogic)->test();
    engine.show();

    //engine.showFullScreen();
    engine.start();

    return app.exec();


}
