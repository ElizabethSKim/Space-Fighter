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

    numbers[0] = engine->spawn<sf::Sprite>(":assets/numbers/0");
    numbers[1] = engine->spawn<sf::Sprite>(":assets/numbers/1");
    numbers[2] = engine->spawn<sf::Sprite>(":assets/numbers/2");
    numbers[3] = engine->spawn<sf::Sprite>(":assets/numbers/3");
    numbers[4] = engine->spawn<sf::Sprite>(":assets/numbers/4");
    numbers[5] = engine->spawn<sf::Sprite>(":assets/numbers/5");
    numbers[6] = engine->spawn<sf::Sprite>(":assets/numbers/6");
    numbers[7] = engine->spawn<sf::Sprite>(":assets/numbers/7");
    numbers[8] = engine->spawn<sf::Sprite>(":assets/numbers/8");
    numbers[9] = engine->spawn<sf::Sprite>(":assets/numbers/9");

    for (int i = 0; i < 10; i ++)
    {
        child_nodes.append(numbers[i]);
        numbers[i]->invisible = true;
    }

}

QVector<int> Score::print_each_digit(int i)
{
    QVector <int> v;
    while(i != 0 ){
        v.push_back(i%10);
        i = i/10;
    }
    return v;
}

void Score::tick(float ticktime)
{
    SceneObject::tick(ticktime);

    for (int i = 0; i < 10; i ++)
    {
        numbers[i]->invisible = true;
    }

    int n = score+10;
    QVector<int> v = print_each_digit(n);
    int loc = 0;
    for(int i=v.size()-1; i>=0; i--){
        numbers[v[i]]->location = QVector3D(spawnLoc.x()+loc, spawnLoc.y(), spawnLoc.z());
        numbers[v[i]]->invisible = false;
        loc += 50;
        qDebug() << v.size()-1;
    }
}
