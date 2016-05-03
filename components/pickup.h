#ifndef PICKUP_H
#define PICKUP_H
#include "sfcomponent.h"
#include <components/sprite.h>

namespace sf
{
    class Pickup : public SceneObject
    {
        Q_OBJECT
    public:
        Pickup();
        void configure();
        void tick(float ticktime);
    private:
        static PreloadAsset preload;
    };
}
#endif // PICKUP_H



