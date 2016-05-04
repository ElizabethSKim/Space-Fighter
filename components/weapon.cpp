#include "weapon.h"
#include <components/sprite.h>
#include <QElapsedTimer>
#include <components/starfighter.h>
#include <components/score.h>
using sf::Weapon;


Weapon::Weapon()
{
    isFiring = false;
    collidable = true;
}

PreloadAsset Weapon::preload([](Engine* engine){
    engine->spawn<sf::Sprite>(":assets/lazer/lazer1");
    engine->spawn<sf::Sprite>(":assets/lazer/lazer2");
    engine->spawn<sf::Sprite>(":assets/lazer/lazer3");
    engine->spawn<sf::Sprite>(":assets/lazer/lazer4");
    engine->spawn<sf::Sprite>(":assets/lazer/lazer5");
    engine->spawn<sf::Sprite>(":assets/lazer/lazer6");
});

void setHitPoly(object_ptr o)
{
    auto spr = object_cast<sf::Sprite>(o);
    spr->hitpoly[0]  =  30; spr->hitpoly[1]  = -20;
    spr->hitpoly[2]  =  50; spr->hitpoly[3]  = -10;
    spr->hitpoly[4]  =  50; spr->hitpoly[5]  =  10;
    spr->hitpoly[6]  =  30; spr->hitpoly[7]  =  20;
    spr->hitpoly[8]  = -30; spr->hitpoly[9]  =  20;
    spr->hitpoly[10] = -50; spr->hitpoly[11] =  10;
    spr->hitpoly[12] = -50; spr->hitpoly[13] = -10;
    spr->hitpoly[14] = -30; spr->hitpoly[15] = -20;
}

void Weapon::configure()
{

    lazer[0] = engine->spawn<sf::Sprite>(":assets/lazer/lazer1");
    lazer[0]->location = QVector3D(270, 0, 0);
    lazer[0]->scale = QVector3D(4, 1, 1);

    lazer[1] = engine->spawn<sf::Sprite>(":assets/lazer/lazer2");
    lazer[1]->location = QVector3D(270, 0, 0);
    lazer[1]->scale = QVector3D(4, 1, 1);

    lazer[2] = engine->spawn<sf::Sprite>(":assets/lazer/lazer3");
    lazer[2]->scale = QVector3D(15,1,1);
    lazer[2]->location = QVector3D(750, 0, 0);

    lazer[3] = engine->spawn<sf::Sprite>(":assets/lazer/lazer4");
    lazer[3]->scale = QVector3D(15,1,1);
    lazer[3]->location = QVector3D(750, 0, 0);

    lazer[4] = engine->spawn<sf::Sprite>(":assets/lazer/lazer5");
    lazer[4]->scale = QVector3D(15,1,1);
    lazer[4]->location = QVector3D(750,0, 0);

    lazer[5] = engine->spawn<sf::Sprite>(":assets/lazer/lazer6");
    lazer[5]->scale = QVector3D(15,1,1);
    lazer[5]->location = QVector3D(750,0, 0);

    for (int i = 0; i < 6; i++)
    {
        lazer[i]->invisible = true;
        setHitPoly(lazer[i]);
        child_nodes.append(lazer[i]);
    }
    connect(engine, &Engine::butA, this, [this](bool b)
    {
        if (b && !isFiring)
        {
            auto sf = object_cast<sf::StarFighter>(engine->root_obj);
            auto score = object_cast<sf::Score>(sf->score);
//            qDebug() << object_cast<sf::Score>(sf)->score;
            if (score)
            {
              if (score->score >= 3)
              {
                  score->score -= 3;
              }
//                {
//                    object_cast<sf::Score>(object_cast<sf::StarFighter>(engine->root_obj))->score -= 1;
                    isFiring = true;
                    frameTime.start();
//                }
            }
        }
    });
}


void Weapon::tick(float ticktime)
{
    SceneObject::tick(ticktime);
    if (isFiring)
    {
        if (frameTime.elapsed() < 100)
        {
            lazer[0]->invisible = false;
        }
        else if (frameTime.elapsed() < 200)
        {
            lazer[1]->invisible = false;
            lazer[0]->invisible = true;

        }
        else if (frameTime.elapsed() < 300)
        {
            lazer[2]->invisible = false;
            lazer[1]->invisible = true;
        }
        //wibble wobble-fixme
        else if (frameTime.elapsed() < 400)
        {
            lazer[3]->invisible = false;
            lazer[2]->invisible = true;
        }

        else if (frameTime.elapsed() < 500)
        {
            lazer[4]->invisible = false;
            lazer[3]->invisible = true;
        }
        else if (frameTime.elapsed() < 600)
        {
            lazer[3]->invisible = false;
            lazer[2]->invisible = true;
        }

        else if (frameTime.elapsed() < 700)
        {
            lazer[4]->invisible = false;
            lazer[3]->invisible = true;
        }
        else if (frameTime.elapsed() < 800)
        {
            lazer[3]->invisible = false;
            lazer[2]->invisible = true;
        }

        else if (frameTime.elapsed() < 900)
        {
            lazer[4]->invisible = false;
            lazer[3]->invisible = true;
        }
        //to here-fixme
        else if (frameTime.elapsed() < 1000)
        {
            lazer[5]->invisible = false;
            lazer[4]->invisible = true;
        }
        else if (frameTime.elapsed() < 1100)
        {
            lazer[5]->invisible = true;
            isFiring = false;
        }
    }

}



