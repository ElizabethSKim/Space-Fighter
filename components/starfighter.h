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
    };
}
#endif // STARFIGHTER_H
