#include "scenegraph.h"

#include "components/starfighter.h"
#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>

#include <QtCore/qmath.h>


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    SceneGraph engine;
    engine.resize(1024,768);
    auto gamelogic = engine.spawn<sf::StarFighter>();
    engine.root_obj = gamelogic;

    //engine.show();
    engine.showFullScreen();

    engine.start();

    return app.exec();


}
