#include "asteroid.h"
#include <components/sprite.h>
#include "health.h"
#include "sceneobject.h"

using sf::Asteroid;

Asteroid::Asteroid()
{

}
void Asteroid::configure()
{
    collidable = true;
    maxAsteroids = 6;
    renderedAsteroids = 0;
    engine->gametime.start();

    asteroid = engine->spawn<sf::Sprite>(":assets/asteroid/asteroid1");
    child_nodes.append(asteroid);
    asteroid->scale = QVector3D(1,1,1);
    asteroid->invisible = false;

    //Decrement healthPoints if ship collided with asteroid
    connect(this, &SceneObject::collided, this, [this](object_ptr other, bool dominant)
    {
        if (other->is("ship"))
        {
            qDebug() <<"Collided asteroid + ship";
            //other->as<sf::Ship>()->
            //<sf::Health>()->healthPoints -= 1;
            //<sf::Health>()->setHearts();
        }
    });
        /*
    connect(engine, &SceneObject::left_screen, this, [this]()
    {
        child_nodes.removeLast();
    });
    */
}

void Asteroid::tick(float ticktime)
{
    SceneObject::tick(ticktime);
/*
    if (fmod((double)engine->gametime.elapsed()/1000.0,5.0) <= 0.01)
    {
        if (renderedAsteroids < maxAsteroids)
        {
            //X Range: [0,1900]
            //Y Range: [0,1200]
            asteroid->invisible = false;
            asteroid->location = QVector3D((fmod((double)rand(),10.0))/10.0 * 1900, 1200,0);
            asteroid->velocity = QVector3D(100*ticktime,-100,0);
            renderedAsteroids += 1;
        }
     }
  */

}
