#ifndef SPRITE_H
#define SPRITE_H

#include <sceneobject.h>
#include <QString>

namespace sf
{
    class Sprite : public SceneObject
    {
        Q_OBJECT

    public:
        //Must be called before the Sprite is added
        //to the scene graph
        void configure(QString asset);
    private:
        QOpenGLTexture *texture;
    };
}
#endif // SPRITE_H

//auto spr = engine.spawn<sf::Sprite>();
//spr.configure("qrc://assets/kitten");
//spr.scale = QVector3D(0.5,0.5,1);
//spr.location = QVector3D(-10,0,0);
//ship.child_nodes.add(spr);

