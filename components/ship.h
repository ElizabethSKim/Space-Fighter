#ifndef SHIP_H
#define SHIP_H

#include "sfcomponent.h"
#include <components/sprite.h>

namespace sf
{
    class Ship : public SceneObject
    {
        Q_OBJECT

    public:
        Ship();
        void configure();
        void tick(float ticktime);
    private:
        QVector2D steeringVector;
        double throttle;
        object_ptr flame;
    };
}
#endif // SHIP_H
