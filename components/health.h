#ifndef HEALTH_H
#define HEALTH_H

#include "sfcomponent.h"
#include <components/sprite.h>

namespace sf
{
    class Health : public SceneObject
    {
        Q_OBJECT

    public:
        Health();
        void configure();
        void tick(float ticktime);
        double healthPoints;
        void setHearts();
        object_ptr heart;
    private:

    };
}

#endif // HEALTH_H
