#ifndef UI_H
#define UI_H

#include "sfcomponent.h"
#include <components/sprite.h>

namespace sf
{
    class UI : public SceneObject
    {
        Q_OBJECT
        public:
            UI();
            void configure();
            void tick(float ticktime);
        private:
            object_ptr start;
            object_ptr howtoplay;
            object_ptr background;
            object_ptr gameover;

    };
}
#endif // UI_H
