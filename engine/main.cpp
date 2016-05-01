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
    auto gamelogic = engine.spawn<sf::StarFighter>();
    engine.root_obj = gamelogic;
   // engine.runLoadFunctions();
    engine.show();
    //engine.showFullScreen();

    engine.start();

    return app.exec();


}
