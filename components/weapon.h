#ifndef WEAPON_H
#define WEAPON_H

#include "sfcomponent.h"
#include <components/sprite.h>
#include <QElapsedTimer>

namespace sf
{
    class Weapon : public SceneObject
    {
        Q_OBJECT
    public:
        Weapon();
        void configure();
        void tick(float ticktime);
        int weaponType;
        QElapsedTimer frameTime;
        bool isFiring;
    private:
        object_ptr lazer[6];
        static PreloadAsset preload;

    };
}
#endif // WEAPON_H

