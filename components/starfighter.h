#ifndef STARFIGHTER_H
#define STARFIGHTER_H

#include "sfcomponent.h"

namespace sf
{
    class StarFighter : public SceneObject
    {
        Q_OBJECT

    public:
        // Override

        virtual void initialize();
        virtual void tick(float ticktime);
        virtual void render(float ticktime);
        object_ptr weapon;
        object_ptr ship;
        object_ptr health;
        bool spawnAsteroids;
        int maxAsteroids;
        int renderedAsteroids;
        int score;

        void spawnPickup(QVector3D loc);
    };
}
#endif // STARFIGHTER_H
