#include "asteroid.h"
#include <components/sprite.h>
#include "health.h"
#include "sceneobject.h"
#include <components/starfighter.h>
using sf::Asteroid;

Asteroid::Asteroid()
{
    isExploding = false;
    isHit = false;
    collidable = true;
    canDelete = false;
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

});

void Asteroid::configure()
{

    asteroid = engine->spawn<sf::Sprite>(":assets/asteroid/asteroid1");


    explosion[0] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion1");
    explosion[1] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion2");
    explosion[2] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion3");
    explosion[3] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion4");
    explosion[4] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion5");
    explosion[5] = engine->spawn<sf::Sprite>(":assets/asteroid/explosion6");

    for (int i = 0; i < 6; i++)
    {
        explosion[i]->invisible = true;
        explosion[i]->location = QVector3D(0,0,0);
        child_nodes.append(explosion[i]);
    }
    child_nodes.append(asteroid);
    asteroid->scale = QVector3D(1,1,1);
    // if (fmod((double)engine->gametime.elapsed()/1000.0,5.0) <= 0.01)
    //X Range: [0,1900]
    //Y Range: [0,1200]

    //asteroid->location = QVector3D((fmod((double)rand(),10.0))/10.0 * 1900, 1200,0);
    //asteroid->location = QVector3D(500, 200, 0);
    //asteroid->velocity = QVector3D(0,0, 0);//-100,0);

    connect(this, &SceneObject::collided, this, [this](object_ptr me, object_ptr other, bool dominant)
    {
        //Collide with ship
        if (other->is("sf::Ship"))
        {
            auto health = object_cast<sf::StarFighter>(engine->root_obj)->health;
            if (health && !isHit)
            {
                isHit = true;
                invincibleTime.start();
            }
        }

        //Collide with weapon
        if (other->is("sf::Weapon"))
        {
            if (!isExploding)
            {
                asteroid->invisible = true;
                isExploding = true;
                explosionTime.start();
                deleteMe = me;
            }
        }

        if (dominant && other->is("sf::Asteroid"))
        {
            /*
            auto vel1dir = this->velocity;
            vel1dir.normalize();
            auto vel2dir = other->velocity;
            vel2dir.normalize();
            auto midaxis = (vel1dir + veld2dir)/2;
            */
            auto v = me->velocity;
            me->velocity = other->velocity;
            other->velocity = v;

           // me->velocity = -velocity;
//            other->velocity = -velocity;
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

    if (isHit)
    {
        //TODO
        if (invincibleTime.elapsed() == 3000)
        {
            auto health = object_cast<sf::StarFighter>(engine->root_obj)->health;
            if (object_cast<sf::Health>(health)->healthPoints > 0)
            {
                object_cast<sf::Health>(health)->healthPoints -= 1;
                object_cast<sf::Health>(health)->setHearts();
            }
        } else
        {
            isHit = false;
        }
    }

    if (isExploding)
    {
        if (explosionTime.elapsed() < 100)
        {
            explosion[0]->invisible = false;
        }
        else if (explosionTime.elapsed() < 200)
        {
            explosion[1]->invisible = false;
            explosion[0]->invisible = true;

        }
        else if (explosionTime.elapsed() < 300)
        {
            explosion[2]->invisible = false;
            explosion[1]->invisible = true;
        }
        else if (explosionTime.elapsed() < 400)
        {
            explosion[3]->invisible = false;
            explosion[2]->invisible = true;
        }

        else if (explosionTime.elapsed() < 500)
        {
            explosion[4]->invisible = false;
            explosion[3]->invisible = true;
        }
        else if (explosionTime.elapsed() < 600)
        {
            explosion[5]->invisible = false;
            explosion[4]->invisible = true;
        }
        else if (explosionTime.elapsed() < 700)
        {
            explosion[5]->invisible = true;
            isExploding = false;
            canDelete = true;

        }
    }

    if (deleteMe && canDelete)
    {
        auto ptr = object_cast<sf::StarFighter>(engine->root_obj);
        ptr->remove_child_later(deleteMe);
        object_cast<sf::StarFighter>(engine->root_obj)->renderedAsteroids -= 1;
        object_cast<sf::StarFighter>(engine->root_obj)->spawnPickup(deleteMe->location);
        deleteMe.clear();
    }
}
