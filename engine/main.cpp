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
    qDebug("a");
    object_cast<sf::StarFighter>(gamelogic)->test();
    //gamelogic->as<sf::StarFighter>();//->test();
    qDebug("b");
    object_cast<sf::StarFighter>(gamelogic)->test();
    //gamelogic->as<sf::StarFighter>();//->test();
    qDebug("c");
    object_cast<sf::StarFighter>(gamelogic)->test();
    //gamelogic->as<sf::StarFighter>();
    qDebug("d");
    engine.show();
 //   gamelogic->as<sf::StarFighter>()->test();
  //gamelogic->as<sf::StarFighter>()->test();
    //engine.showFullScreen();

    engine.start();

    return app.exec();


}
