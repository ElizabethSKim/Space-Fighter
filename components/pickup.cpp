#include "pickup.h"
#include <components/sprite.h>
#include <components/starfighter.h>
#include <components/score.h>
#include "sceneobject.h"
using sf::Pickup;

Pickup::Pickup()
{

}

PreloadAsset Pickup::preload([](Engine* engine){
    engine->spawn<sf::Sprite>(":assets/asteroid/star");

});

void Pickup::configure()
{
    auto star = engine->spawn<sf::Sprite>(":assets/asteroid/star");
    collidable = true;
    child_nodes.append(star);


    connect(this, &SceneObject::collided, this, [this](object_ptr me, object_ptr other, bool dominant)
    {
        //Collide with ship
        if (other->is("sf::Ship"))
        {
            //TODO remove this/me whatevs
//            auto ptr_score  = object_cast<sf::StarFighter>(engine->root_obj)->score;
            object_cast<sf::Score>(object_cast<sf::StarFighter>(engine->root_obj)->score)->score += 1;
            auto ptr = object_cast<sf::StarFighter>(engine->root_obj);
            ptr->remove_child_later(me);
            me.clear();

        }
    });

}

void Pickup::tick(float ticktime)
{
    SceneObject::tick(ticktime);
}
