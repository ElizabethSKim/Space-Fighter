#ifndef ASTEROID_H
#define ASTEROID_H

#include "sfcomponent.h"
#include <components/sprite.h>
#include <QElapsedTimer>
namespace sf
{
    class Asteroid : public SceneObject
    {
        Q_OBJECT
    public:
        Asteroid();
        void configure();
        void tick(float ticktime);
        void die(object_ptr me);
    private:
        QElapsedTimer invincibleTime;
        QElapsedTimer explosionTime;
        bool isHit;
        bool isExploding;
        object_ptr explosion[6];
        object_ptr asteroid;
        static PreloadAsset preload;
        object_ptr deleteMe;
        bool canDelete;


    };
}
#endif // ASTEROID_H
