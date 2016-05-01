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

    connect(engine, &Engine::startPressed, this, [this]()
    {
        ship = engine->spawn<sf::Ship>();
        child_nodes.append(ship);
        ship->location = QVector3D(800,200,0);

        weapon = engine->spawn<sf::Weapon>();
        child_nodes.append(weapon);

        //Spawn health
        auto health = engine->spawn<sf::Health>();
        child_nodes.append(health);
        health->location = QVector3D(50, 50, 0);

        auto asteroid = engine->spawn<sf::Asteroid>();
        child_nodes.append(asteroid);
        asteroid->location = QVector3D(500, 500, 0);


    });
}

void StarFighter::tick(float ticktime)
{
   SceneObject::tick(ticktime);
   if (weapon && ship)
   {
       weapon->location = ship->location;
       weapon->rotation = ship->rotation + 270; //270;
    }
}

void StarFighter::render(float ticktime)
{
    SceneObject::render(ticktime);
}
