#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include <QStack>
#include <QVector3D>
#include <QVector2D>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QSharedPointer>
#include <QRect>
#include <QtGui/QOpenGLFunctions>

class SceneGraph;
class SceneObject;

typedef QSharedPointer<SceneObject> object_ptr;

class SceneObject : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:

    // OVERRIDABLE FUNCTIONS
    // When you are adding new objects to the game, these are the functions
    // that you need to implement

    // This is called once when the object is added to the scene graph.
    virtual void initialize();

    // This is called before every frame is rendered, and can be used for
    // adding game logic. The default implementation ticks child_nodes
    // and updates the object's location and rotation based on its linear
    // and rotational velocity and acceleration. It also updates cached_bbox
    virtual void tick(float ticktime);

    // Render the actual object. This is called once per frame. The default
    // implementation updates the matrix with the objects location and rotation
    // and then renders the child_nodes
    virtual void render(float ticktime);

    // Render any postprocessing effects. The default implementation updates the
    // matrix with the objects location and rotation and then calls postprocess
    // on the child_nodes
    virtual void postprocess(float ticktime);

    // Return the axis_aligned bounding box of just this object (not its children)
    // the default is nothing (width/height zero). The returned value should be in
    // the object's coordinate space. It is up to the caller to multiply it by
    // the matrix
    virtual QRect get_intrinsic_aabbox();

    // UTILITY FUNCTIONS
    // These are available for use in the object's implementation
public:
    // Convert this generic SceneObject to a specific object type
    template <typename T> QSharedPointer<T> as()
    {
        T *rv = qobject_cast<T*>(this);
        if (rv == 0) {
            T example;
            QObject *qo = (QObject*) &example;
            qFatal("A SceneObject of type %s was badly casted with ::as<%s>", this->metaObject()->className(), qo->metaObject()->className())
        }
        return QSharedPointer<T>(rv);
    }
    // Returns true if this object is a subclass of the given class
    bool is (QString klass)
    {
        return this->inherits(klass);
    }

    // Returns true if the object's axis-aligned bounding box intersects the viewport
    bool on_screen();

    // You will not need to use this
    void set_engine(SceneGraph *engine);
protected:
    // Push a copy of the current matrix to the stack
    void pushmatrix();

    // Pop the previous matrix off the stack
    void popmatrix();

    // Update the current matrix to add rotation
    void matrix_rotate(QQuaternion& q);

    // Same, but assume rotation about negative z (screen clockwize)
    void matrix_rotate(float radians);

    // Update the current matrix to add translation
    void matrix_translate(QVector3D &v);

    // Same, but assume dz = 0
    void matrix_translate(float x, float y);

    // Update the current matrix to add scale
    void matrix_scale(QVector3D &v);

    // Update the current matrix with the rotation, location and scale of this scene object.
    void automatrix();

    // Typical definition
    float min(float a, float b);
    float max(float a, float b);
    float clamp(float v, float min, float max);

    // This returns the complete matrix (whole stack) for use in render()
    // or postprocess(). It should not be overriden.
    QMatrix4x4 getmatrix();

    // A pointer to the global engine, set when the object is added to the engine
    SceneGraph *engine;

    // Used to ensure initialized() is only called once
    bool is_initialized;

    // Used by the default tick() to signal enter_screen() and left_screen()
    bool on_screen_last_frame;

public:
    // The object's physical properties
    QVector3D location;
    QVector3D velocity;
    QVector3D acceleration;
    QQuaternion rotation;
    QQuaternion rotvelocity;
    QQuaternion rotacceleration;
    QVector3D scale;

    // If false, the object and it's children should not be rendered
    bool visible;

    //These are the SceneObjects below this one in the scene graph
    QList<object_ptr> child_nodes;

    // This is set by tick() after it calculates the bbox, so is faster
    // to use
    QRect aabbox;

signals:
    // This is called whenever this object collides with another object.
    // The pair of objects will both have collided() emitted, but one will
    // be flagged dominant and one not, this allows you to prevent having
    // duplicate handling of collisions.
    void collided(object_ptr other, bool dominant);

    // Signalled when the object's bounding box has entered the screen.
    // See also on_screen()
    void enter_screen();

    // Signalled when the object's bounding box has left the screen.
    // See also on_screen()
    void left_screen();
};



#endif // SCENEOBJECT_H
