#include "asteroid.h"
#include <components/sprite.h>
#include "health.h"
#include "sceneobject.h"
#include <components/starfighter.h>
using sf::Asteroid;

Asteroid::Asteroid()
{

}

PreloadAsset Asteroid::preload([](Engine* engine){
    engine->spawn<sf::Sprite>(":assets/asteroid/asteroid1");
    engine->spawn<sf::Sprite>(":assets/asteroid/asteroid2");
    engine->spawn<sf::Sprite>(":assets/asteroid/asteroid3");
    engine->spawn<sf::Sprite>(":assets/asteroid/asteroid4");
    engine->spawn<sf::Sprite>(":assets/asteroid/explosion1");
    engine->spawn<sf::Sprite>(":assets/asteroid/explosion2");
    engine->spawn<sf::Sprite>(":assets/asteroid/explosion3");
    engine->spawn<sf::Sprite>(":assets/asteroid/explosion4");
    engine->spawn<sf::Sprite>(":assets/asteroid/explosion5");
    engine->spawn<sf::Sprite>(":assets/asteroid/explosion6");
    engine->spawn<sf::Sprite>(":assets/asteroid/star");
});

void Asteroid::configure()
{
    collidable = true;
    maxAsteroids = 6;
    renderedAsteroids = 0;

    setAsteroids();

    explosion[0] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion1");
    explosion[1] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion2");
    explosion[2] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion3");
    explosion[3] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion4");
    explosion[4] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion5");
    explosion[5] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion6");


    //Decrement healthPoints if ship collided with asteroid
    //connect(this, &SceneObject::collided, this, [this](object_ptr other, bool dominant)
    connect(engine, &Engine::butY, this, [this](bool b)
    {
      //  if (other->is("ship"))
        if (b)
        {
            auto health = object_cast<sf::StarFighter>(engine->root_obj)->health;
            if (health)
            {
                qDebug() <<"Collided asteroid + ship";
                object_cast<sf::Health>(health)->healthPoints -= 1;
                object_cast<sf::Health>(health)->setHearts();
            }
        }
//        if (other->is("lazer3") || other->is("lazer4") || other->is("lazer5") || other->is("lazer6"))
//        {

//        }

    });

     /*
    connect(engine, &SceneObject::left_screen, this, [this]()
    {
        child_nodes.removeLast();
    });
    */
}

void Asteroid::setAsteroids()
{
    if (renderedAsteroids < maxAsteroids)
    {
        for (int i = 0; i < maxAsteroids; i ++)
        {
            auto asteroid = engine->spawn<sf::Sprite>(":assets/asteroid/asteroid1");
            child_nodes.append(asteroid);
            asteroid->scale = QVector3D(1,1,1);
            // if (fmod((double)engine->gametime.elapsed()/1000.0,5.0) <= 0.01)
            //X Range: [0,1900]
            //Y Range: [0,1200]

            //asteroid->location = QVector3D((fmod((double)rand(),10.0))/10.0 * 1900, 1200,0);
            asteroid->location = QVector3D(500, 200, 0);
            asteroid->velocity = QVector3D(20,0, 0);//-100,0);
            renderedAsteroids += 1;
        }
    }
}

void Asteroid::tick(float ticktime)
{
    SceneObject::tick(ticktime);
    qDebug() << "ticktimE: " << ticktime;


}
