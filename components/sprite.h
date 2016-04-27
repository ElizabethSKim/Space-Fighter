#ifndef SPRITE_H
#define SPRITE_H

#include "sfcomponent.h"

namespace sf
{
    class Sprite : public SceneObject
    {
        Q_OBJECT

    public:
        //Must be called before the Sprite is added
        //to the scene graph
        void configure(QString asset);

        void render(float ticktime);
        QRect get_intrinsic_aabbox();

    private:
        QOpenGLTexture *texture;
        QOpenGLShaderProgram *shader_program;
        int shader_vertex_position;
        int shader_vertex_uv_coordinate;
    };
}
#endif // SPRITE_H
