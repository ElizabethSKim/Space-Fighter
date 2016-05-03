#include "starfighter.h"
#include <components/sprite.h>
#include <components/ship.h>
#include <components/health.h>
#include <components/asteroid.h>
#include <components/weapon.h>
#include <components/ui.h>
#include <components/pickup.h>

using namespace sf;

//FIXME Pressing start spawns 2 ships :O

void StarFighter::initialize()
{    
    //starts with start page invisible = false
    auto ui = engine->spawn<sf::UI>();
    child_nodes.prepend(ui);
    ui->location = QVector3D(960,540,0);
    ui->scale = QVector3D(19.3,10.7,1);

    spawnAsteroids = false;
    renderedAsteroids = 0;
    maxAsteroids = 10;
    score = 0;

    connect(engine, &Engine::startPressed, this, [this](bool b)
    {
        if (b)
        {
            ship = engine->spawn<sf::Ship>();
            child_nodes.append(ship);
            ship->location = QVector3D(800,200,0);

            weapon = engine->spawn<sf::Weapon>();
            child_nodes.append(weapon);

            //Spawn health
            health = engine->spawn<sf::Health>();
            child_nodes.append(health);
            health->location = QVector3D(50, 50, 0);

            spawnAsteroids = true;
        }
    });
}

void StarFighter::spawnPickup(QVector3D loc)
{
    //TODO
    auto star = engine->spawn<sf::Pickup>();
    child_nodes.append(star);
    star->location = loc;
}

void StarFighter::tick(float ticktime)
{

    SceneObject::tick(ticktime);

    if (weapon)
    {
        weapon->location = ship->location;
        weapon->rotation = ship->rotation + 270;
    }

    if (spawnAsteroids)
    {
            if (qrand() % 180 == 0)
            {
                auto asteroid = engine->spawn<sf::Asteroid>();
                child_nodes.append(asteroid);
                int xAxis = qrand() % 1500;
                //TODO Have 4 cases
                //TODO if has entered is true and then isLeft -> remove asteroid
                asteroid->location = QVector3D(xAxis, 0, 0);
                asteroid->velocity = QVector3D(qrand() % 100, qrand() % 100 ,0);
                renderedAsteroids += 1;
            }


//            if (engine->gametime.elapsed() % 128 == 0)
//            {
//                auto asteroid = engine->spawn<sf::Asteroid>();
//                child_nodes.append(asteroid);
//                int xAxis = qrand() % 1900;
//                asteroid->location = QVector3D(xAxis, 1200, 0);
//                asteroid->velocity = QVector3D(qrand() % 100,-qrand() % 100 ,0);
//                renderedAsteroids += 1;
//            }
//            if (engine->gametime.elapsed() % 42 = 0)
//            {
//                auto asteroid = engine->spawn<sf::Asteroid>();
//                child_nodes.append(asteroid);
//                int yAxis = qrand() % 1200;
//                asteroid->location = QVector3D(1900, yAxis,0);
//                asteroid->velocity = QVector3D(-qrand() % 100,qrand() % 100 ,0);
//                renderedAsteroids += 1;
//            }
//        }
    }


    //TODO
//   if (object_cast<sf::Health>(health)->healthPoints < 0)
//   {
//        //call gameover function
//   }

}

void StarFighter::render(float ticktime)
{
    SceneObject::render(ticktime);
}
