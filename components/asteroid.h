#ifndef ASTEROID_H
#define ASTEROID_H

#include "sfcomponent.h"
#include <components/sprite.h>

namespace sf
{
    class Asteroid : public SceneObject
    {
        Q_OBJECT
    public:
        Asteroid();
        void configure();
        void tick(float ticktime);
    private:
        int maxAsteroids;
        int renderedAsteroids;
        object_ptr health;
       // object_ptr asteroid;
        object_ptr explosion[6];
        void setAsteroids();
        static PreloadAsset preload;
    };
}
#endif // ASTEROID_H
