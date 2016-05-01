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

class Engine;
class SceneObject;

typedef QSharedPointer<SceneObject> object_ptr;

template <typename T> QSharedPointer<T> object_cast(object_ptr o)
{
    return o.dynamicCast<T>();
}

class SceneObject : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT

public:

    SceneObject();
    ~SceneObject();

    void test2();
    // OVERRIDABLE FUNCTIONS
    // When you are adding new objects to the game, these are the functions
    // that you need to implement

    // This is called once when the object is added to the scene graph. It is called on the next
    // tick.
    virtual void initialize();

    // This function is called by engine::spawn with whatever parameters are passed
    // to spawn. You should use this for expensive stuff like compiling shaders
    // or loading textures. Look at sf::Sprite as an example
    void configure( /* whatever parameters you want */ );

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
    virtual QRectF get_intrinsic_aabbox();

    // UTILITY FUNCTIONS
    // These are available for use in the object's implementation
public:
    // Convert this generic SceneObject to a specific object type

    template <typename T> bool foo()
    {
        qDebug() << "I don't do anything...";
        return true;
    }

    // Returns true if this object is a subclass of the given class
    bool is (const char* klass)
    {
        return this->inherits(klass);
    }

    // Returns true if the object's axis-aligned bounding box intersects the viewport
    bool on_screen();

    // You will not need to use this
    void set_engine(Engine *engine);
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
    Engine *engine;

    // Used to ensure initialized() is only called once
    bool is_initialized;

    // Used by the default tick() to signal enter_screen() and left_screen()
    bool on_screen_last_frame;

public:
    // The object's physical properties
    QVector3D location;
    QVector3D velocity;
    QVector3D acceleration;
    double rotation;
    double rotvelocity;
    double rotacceleration;
    QVector3D scale;

    // If true, the object and it's children should not be rendered
    bool invisible;

    // If true, the object is collidable and will have collided() signalled
    // on it (assuming that none of it's parents in the scene graph are also
    // collidable, as the collided signal is sent to the first
    // collidable object in a heirarchy
    bool collidable;

    //These are the SceneObjects below this one in the scene graph
    QList<object_ptr> child_nodes;

    // This is set by tick() after it calculates the bbox, so is faster
    // to use
    QRectF aabbox;

    // This is set by tick() and contains the object's full matrix
    QMatrix4x4 matrix;

    // This will not need to be used, it is simply for performance debugging
    static int total_live_scene_objects;

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
