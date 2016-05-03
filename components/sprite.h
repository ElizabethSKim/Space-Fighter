#ifndef SPRITE_H
#define SPRITE_H

#include "sfcomponent.h"

//Set this to true to draw bounding box and hitpoly by default
#define DEFAULT_DRAW_HITPOLY false

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
        void tick(float ticktime);
        QList<QVector2D> hitpolyNormals();
        QPair<double, double> hitpolyProject(QVector2D &axis);
        bool hitpoly_intersects(QSharedPointer<sf::Sprite> other);

        QRectF get_intrinsic_aabbox();
        QOpenGLTexture *texture;
        bool draw_hitpoly;
        float hitpoly[16]; //An irregular octagon in object-space coordinates
        QVector4D hitbox_color;
        static int shader_vertex_position;
        static int shader_vertex_uv_coordinate;
        static QOpenGLShaderProgram *shader_program;
        static QHash<QString, QOpenGLTexture*> loaded;

        static PreloadAsset preload;
    };
}
#endif // SPRITE_H
