#include "score.h"
#include <components/sprite.h>
#include <components/starfighter.h>
#include "sceneobject.h"
using sf::Score;

Score::Score()
{
    score = 0;
    spawnLoc = location;
}

PreloadAsset Score::preload([](Engine* engine){
    engine->spawn<sf::Sprite>(":assets/numbers/0");
    engine->spawn<sf::Sprite>(":assets/numbers/1");
    engine->spawn<sf::Sprite>(":assets/numbers/2");
    engine->spawn<sf::Sprite>(":assets/numbers/3");
    engine->spawn<sf::Sprite>(":assets/numbers/4");
    engine->spawn<sf::Sprite>(":assets/numbers/5");
    engine->spawn<sf::Sprite>(":assets/numbers/6");
    engine->spawn<sf::Sprite>(":assets/numbers/7");
    engine->spawn<sf::Sprite>(":assets/numbers/8");
    engine->spawn<sf::Sprite>(":assets/numbers/9");

});

void Score::configure()
{
    numbers[0] = QString(":assets/numbers/0");
    numbers[1] = QString(":assets/numbers/1");
    numbers[2] = QString(":assets/numbers/2");
    numbers[3] = QString(":assets/numbers/3");
    numbers[4] = QString(":assets/numbers/4");
    numbers[5] = QString(":assets/numbers/5");
    numbers[6] = QString(":assets/numbers/6");
    numbers[7] = QString(":assets/numbers/7");
    numbers[8] = QString(":assets/numbers/8");
    numbers[9] = QString(":assets/numbers/9");
}

QVector<int> Score::print_each_digit(int i)
{
    QVector <int> v;
    do{
        v.push_back(i%10);
        i = i/10;
    }while(i != 0 );
    return v;
}

void Score::tick(float ticktime)
{
    child_nodes.clear();
    int n = score;
    if (n < 0) /*lolwut*/ n = 0;
    QVector<int> v = print_each_digit(n);
    int loc = 0;
    for(int i=v.size()-1; i>=0; i--){
        auto thisdigit = engine->spawn<sf::Sprite>(numbers[v[i]]);
        child_nodes.append(thisdigit);
        thisdigit->location = QVector3D(spawnLoc.x()+loc, spawnLoc.y(), spawnLoc.z());
        loc += 50;
    }
    SceneObject::tick(ticktime);
}
