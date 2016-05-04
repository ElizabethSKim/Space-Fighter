#ifndef SCORE_H
#define SCORE_H
#include "sfcomponent.h"
#include <components/sprite.h>

namespace sf
{
    class Score : public SceneObject
    {
        Q_OBJECT
        public:
            Score();
            void configure();
            void tick(float ticktime);
            object_ptr numbers[10];
            int score;
            QVector<int> print_each_digit(int i);
            QVector3D spawnLoc;
        private:
            static PreloadAsset preload;
    };
}
#endif // SCORE_H
