#include "asteroid.h"
#include <components/sprite.h>
#include "health.h"
#include "sceneobject.h"
#include <components/starfighter.h>
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
    //connect(this, &SceneObject::collided, this, [this](object_ptr other, bool dominant)
    connect(engine, &Engine::butY, this, [this](bool b)
    {
        if (b)
        //if (other->is("ship"))
        {
            auto health = object_cast<sf::StarFighter>(engine->root_obj)->health;
            if (health)
            {
                qDebug() <<"Collided asteroid + ship";
                object_cast<sf::Health>(health)->healthPoints -= 1;
                object_cast<sf::Health>(health)->setHearts();
                //engine->root_obj->as<sf::StarFighter>()->health->as<sf::Health>()->setHearts();
            }
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
