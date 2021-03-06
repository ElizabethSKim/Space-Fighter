#include "starfighter.h"
#include <components/sprite.h>
#include <components/ship.h>
#include <components/health.h>
#include <components/asteroid.h>
#include <components/weapon.h>
#include <components/ui.h>
#include <components/pickup.h>
#include <components/score.h>
#include <components/dabomb.h>

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

    bomb = engine->spawn<sf::DaBomb>();
    child_nodes.append(bomb);

    connect(engine, &Engine::rightBumper, this, [this](bool b)
    {
       if (!b) return;
       if (object_cast<sf::Score>(score)->score >= 10)
       {
           if (!object_cast<sf::DaBomb>(bomb)->started)
           {
                object_cast<sf::Score>(score)->score -= 10;
                firebomb(ship->location);
           }
       }
    });

    connect(engine, &Engine::startPressed, this, [this](bool b)
    {
        if (b)
        {
            ship = engine->spawn<sf::Ship>();
            child_nodes.append(ship);
            ship->location = QVector3D(800,200,0);

            score = engine->spawn<sf::Score>();
            child_nodes.append(score);
            score->location = QVector3D(1700, 100, 0);

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
    star->scale = QVector3D(0.5,0.5,0);
}

void StarFighter::firebomb(QVector3D loc)
{
    auto realbomb = object_cast<sf::DaBomb>(bomb);
    bomb->location = loc;
    if(!realbomb->started)
    {
        realbomb->start();
    }
    foreach(auto star, child_nodes)
    {
        if (star->is("sf::Pickup"))
        {
            star->velocity = (loc - star->location) * 0.25;
        }
        if (star->is("sf::Asteroid"))
        {
            object_cast<sf::Asteroid>(star)->die(star);
        }
    }
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
            //TODO if has entered is true and then isLeft -> remove asteroid
            asteroid->location = QVector3D(xAxis, 0, 0);
            asteroid->velocity = QVector3D(qrand() % 100, qrand() % 100 ,0);
            renderedAsteroids += 1;

            auto asteroid1 = engine->spawn<sf::Asteroid>();
            child_nodes.append(asteroid1);
            asteroid1->location = QVector3D(xAxis, 1200, 0);
            asteroid1->velocity = QVector3D(qrand() % 50,-qrand() % 100 ,0);
            renderedAsteroids += 1;

//            auto asteroid2 = engine->spawn<sf::Asteroid>();
//            child_nodes.append(asteroid2);
//            int yAxis = qrand() % 100;
//            asteroid2->location = QVector3D(1500, yAxis,0);
//            asteroid2->velocity = QVector3D(-qrand() % 100,qrand() % 100 ,0);
//            renderedAsteroids += 1;
        }
    }
}

void StarFighter::render(float ticktime)
{
    SceneObject::render(ticktime);
}
