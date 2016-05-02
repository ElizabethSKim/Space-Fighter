#include "starfighter.h"
#include <components/sprite.h>
#include <components/ship.h>
#include <components/health.h>
#include <components/asteroid.h>
#include <components/weapon.h>
#include <components/ui.h>

using namespace sf;

//FIXME Pressing start spawns 2 ships :O

void StarFighter::initialize()
{    
    //starts with start page invisible = false
    auto ui = engine->spawn<sf::UI>();
    child_nodes.prepend(ui);
    ui->location = QVector3D(960,540,0);
    ui->scale = QVector3D(19.3,10.7,1);

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
        }
    });
}

void StarFighter::tick(float ticktime)
{
   SceneObject::tick(ticktime);

   if (weapon)
   {
       weapon->location = ship->location;
       weapon->rotation = ship->rotation + 270;
    }

    if (engine->gametime.elapsed() % 63 == 0)
    {
        auto asteroid = engine->spawn<sf::Asteroid>();
        child_nodes.append(asteroid);
        int xAxis = qrand() % 1900;
        asteroid->location = QVector3D(xAxis, 0, 0);
        asteroid->velocity = QVector3D(qrand() % 100, qrand() % 100 ,0);
    }

    if (engine->gametime.elapsed() % 128 == 0)
    {
        auto asteroid = engine->spawn<sf::Asteroid>();
        child_nodes.append(asteroid);
        int xAxis = qrand() % 1900;
        asteroid->location = QVector3D(xAxis, 1200, 0);
        asteroid->velocity = QVector3D(qrand() % 100,-qrand() % 100 ,0);
    }
    if (engine->gametime.elapsed() % 42 == 0)
    {
        auto asteroid = engine->spawn<sf::Asteroid>();
        child_nodes.append(asteroid);
        int yAxis = qrand() % 1200;
        asteroid->location = QVector3D(1900, yAxis,0);
        asteroid->velocity = QVector3D(-qrand() % 100,qrand() % 100 ,0);
    }
    if (engine->gametime.elapsed() % 2 == 0)
    {

    }


//   if (object_cast<sf::Health>(health)->healthPoints < 0)
//   {
//        //call gameover function
//   }

}

void StarFighter::render(float ticktime)
{
    SceneObject::render(ticktime);
}
