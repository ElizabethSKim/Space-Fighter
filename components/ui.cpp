#include "ui.h"
#include <components/sprite.h>
#include <components/starfighter.h>
#include <components/health.h>

using sf::UI;

UI::UI()
{

}

void UI::configure()
{

    //TODO preload stuff
    start = engine->spawn<sf::Sprite>(":assets/ui/start");
    start->invisible = false;
    child_nodes.append(start);

    howtoplay = engine->spawn<sf::Sprite>(":assets/ui/howtoplay");
    howtoplay->invisible = true;
    child_nodes.append(howtoplay);

    background = engine->spawn<sf::Sprite>(":assets/ui/background");
    background->invisible = true;
    child_nodes.append(background);

    gameover = engine->spawn<sf::Sprite>(":assets/ui/gameover");
    gameover->invisible = true;
    child_nodes.append(gameover);

    connect(engine, &Engine::startPressed, this, [this]()
    {
        start->invisible = true;
        background->invisible = false;
    });

    connect(engine, &Engine::butB, this, [this](bool b)
    {
        if (b)
        {
            start->invisible = true;
            howtoplay->invisible = false;
        }
    });

}

void UI::tick(float ticktime)
{
    SceneObject::tick(ticktime);
    auto sf = object_cast<sf::StarFighter>(engine->root_obj);
    auto h = object_cast<sf::Health>(sf->health);
  //  if ((h->healthPoints) <= 0)
    {
//        gameover->invisible = false;
//        object_cast<sf::StarFighter>(engine->root_obj)->spawnAsteroids = false;

    }
}



