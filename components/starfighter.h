#ifndef STARFIGHTER_H
#define STARFIGHTER_H

#include <QObject>
#include <sceneobject.h>

namespace sf
{
    class StarFighter : public SceneObject
    {
        Q_OBJECT

    public:
        // Override
        virtual void initialize();
        virtual void tick(float ticktime);
    };
}
#endif // STARFIGHTER_H
