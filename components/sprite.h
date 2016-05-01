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
        QRectF get_intrinsic_aabbox();

        QOpenGLTexture *texture;

        static int shader_vertex_position;
        static int shader_vertex_uv_coordinate;
        static QOpenGLShaderProgram *shader_program;
        static QHash<QString, QOpenGLTexture*> loaded;

        static PreloadAsset preload;
    };
}
#endif // SPRITE_H
