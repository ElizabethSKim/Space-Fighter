#include "starfighter.h"
#include <components/sprite.h>
#include <components/ship.h>

using namespace sf;

void StarFighter::initialize()
{
 // auto sprite = engine->spawn<sf::Sprite>(":/assets/kitten");
 //  child_nodes.append(sprite);
  // sprite->location = QVector3D(300,300,0);
    auto ship = engine->spawn<sf::Ship>();
    child_nodes.append(ship);
    ship->location = QVector3D(200,200,0);
}

void StarFighter::tick(float ticktime)
{
   SceneObject::tick(ticktime);
}

void StarFighter::render(float ticktime)
{
    SceneObject::render(ticktime);
}
