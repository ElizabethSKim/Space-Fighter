#include "health.h"
#include <components/sprite.h>

using sf::Health;

Health::Health()
{
    healthPoints = 3;
}

void Health::configure()
{

        setHearts();

}

void Health::setHearts()
{
    child_nodes.clear();
    for (int i = 0; i < healthPoints; i ++)
    {
        qDebug() << "got here";
        heart = engine->spawn<sf::Sprite>(":assets/ui/heart");
        child_nodes.append(heart);
        heart->scale = QVector3D(0.5,0.5,1);
        heart->location = QVector3D(i*75, 10, 0);

    }
}

void Health::tick(float ticktime)
{
    SceneObject::tick(ticktime);

    //TODO
    if (healthPoints < 1)
    {
        //trigger game sound
        //trigger game over
    }



}
