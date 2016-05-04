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
        void firebomb(QVector3D loc);
        object_ptr weapon;
        object_ptr ship;
        object_ptr health;
        object_ptr score;
        object_ptr bomb;
        bool spawnAsteroids;
        int maxAsteroids;
        int renderedAsteroids;
        void spawnPickup(QVector3D loc);
    };
}
#endif // STARFIGHTER_H
