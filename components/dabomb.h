#ifndef DABOMB_H
#define DABOMB_H

#include <components/sfcomponent.h>
#include <QElapsedTimer>

namespace sf
{
    class DaBomb : public SceneObject
    {
        Q_OBJECT
    public:
        DaBomb();
        void configure();
        void start();
        bool started;
        virtual void tick(float ticktime);
        virtual void postprocess(float ticktime);
        QElapsedTimer frametime;
    };
}
#endif // DABOMB_H
