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
        ~StarFighter();
        void test();
        virtual void initialize();
        virtual void tick(float ticktime);
        virtual void render(float ticktime);
        object_ptr weapon;
        object_ptr ship;
        object_ptr health;
        object_ptr asteroids;

    };
}
#endif // STARFIGHTER_H
