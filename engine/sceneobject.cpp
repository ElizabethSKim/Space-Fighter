
#include "sceneobject.h"
#include "engine.h"

#include "components/sprite.h"

int SceneObject::total_live_scene_objects;

SceneObject::SceneObject()
{
    scale = QVector3D(1,1,1);
    collidable = false;
    rotation = 0;
    is_initialized = false;
    aabbox = QRectF();
    on_screen_last_frame = false;
    total_live_scene_objects++;
    deferred_deletes = QList<object_ptr>();
}
SceneObject::~SceneObject()
{
    total_live_scene_objects--;
}
void SceneObject::test2() {
    qDebug() << "SdfsFD";
}

QList<QSharedPointer<sf::Sprite>> SceneObject::get_collidable_sprites()
{
    QList<QSharedPointer<sf::Sprite>> rv;
    foreach(auto ch, child_nodes)
    {
        if (!ch->invisible)
        {
            if (ch->is("sf::Sprite"))
                rv.append(object_cast<sf::Sprite>(ch));
            rv.append(ch->get_collidable_sprites());
        }
    }
    return rv;
}

void SceneObject::tick(float ticktime)
{
    //Linear
    QVector3D delta_velocity = acceleration*ticktime;
    QVector3D delta_location = velocity*ticktime;
    location += delta_location;
    velocity += delta_velocity;

    //Angular
   /* rotacceleration.normalize();
    QQuaternion delta_rotvelocity = rotacceleration*ticktime;
    QQuaternion delta_rotation = rotvelocity*ticktime;
    rotation += delta_rotation;
    rotation.normalize();
    rotvelocity += delta_rotvelocity;
    rotvelocity.normalize();*/

    //Bind this object to the OpenGL context, and also call the init function
    if (!is_initialized)
    {
        initializeOpenGLFunctions();
        initialize();
        is_initialized = true;
    }

    pushmatrix();
    automatrix();
    //Cache our matrix
    matrix = getmatrix();

    //Also tick sub nodes
    foreach(auto p, child_nodes)
    {
        p->tick(ticktime);
    }

    //And delete all that asked to be deleted
    foreach (auto p, deferred_deletes)
    {
        child_nodes.removeOne(p);
    }
    deferred_deletes.clear();
    popmatrix();

    //Now that all sub nodes are ticked, lets update our bbox
    //First we need to get our own intrinsic aabbox and convert
    //it to screen coordinates
    QRectF rv = get_intrinsic_aabbox();
    bool set = !rv.isNull();
    if (set)
    {
        QList<QVector3D> verts;
        verts.append(matrix*QVector3D(rv.topLeft()));
        verts.append(matrix*QVector3D(rv.topRight()));
        verts.append(matrix*QVector3D(rv.bottomLeft()));
        verts.append(matrix*QVector3D(rv.bottomRight()));
        double left = verts[0].x();
        double right = verts[0].x();
        double top = verts[0].y();
        double bottom = verts[0].y();
        foreach(auto v, verts)
        {
            if (v.x() < left) left = v.x();
            if (v.x() > right) right = v.x();
            if (v.y() > top) top = v.y();
            if (v.y() < bottom) bottom = v.y();

        }
        rv = QRectF(QPointF(left,top), QPointF(right, bottom));
    }
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
        if ((*p)->invisible)
            continue;
        QRectF cbb = (*p)->aabbox;
        if (!cbb.isNull())
        {
            //Valid bounding box:
            if (!set) {
                rv = cbb;
                set = true;
            } else {
                rv = rv.united(cbb);
            }
        }
    }
    aabbox = rv;

    //Now lets check if we have entered or left the screen
    if (!on_screen_last_frame && on_screen())
    {
        emit enter_screen();
    }
    if (on_screen_last_frame && !on_screen())
    {
        emit left_screen();
    }
    on_screen_last_frame = on_screen();
}
void SceneObject::remove_child_later(object_ptr o)
{
    deferred_deletes.append(o);
}

void SceneObject::render(float ticktime)
{
    //Default simply sets matrix and processes sub objects, assuming they are visible
    //and on-screen. If/when you replace this you should still do this
    pushmatrix();
    automatrix();
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
        if (!(*p)->invisible && (*p)->on_screen()) {
            (*p)->render(ticktime);
        }
    }
    popmatrix();
}
void SceneObject::configure()
{

}

void SceneObject::set_engine(Engine *engine)
{
    this->engine = engine;
}

void SceneObject::matrix_rotate(QQuaternion &q)
{
    engine->stack.top().rotate(q);
}
void SceneObject::matrix_rotate(float degrees)
{
    engine->stack.top().rotate(degrees, 0, 0, 1);
}
void SceneObject::matrix_translate(QVector3D &v)
{
    engine->stack.top().translate(v);
}
void SceneObject::matrix_translate(float x, float y)
{
    engine->stack.top().translate(x,y,0);
}
void SceneObject::matrix_scale(QVector3D &v)
{
    engine->stack.top().scale(v);
}
void SceneObject::pushmatrix()
{
    engine->stack.push(engine->stack.top());
}
void SceneObject::popmatrix()
{
    engine->stack.pop();
}
QMatrix4x4 SceneObject::getmatrix()
{
    return engine->stack.top();
}
void SceneObject::automatrix()
{
    matrix_translate(location);
    matrix_rotate(rotation);
    matrix_scale(scale);
}
float SceneObject::min(float a, float b)
{
    return a<b?a:b;
}
float SceneObject::max(float a, float b)
{
    return (a>b)?a:b;
}
float SceneObject::clamp(float value, float floor, float ceil)
{
    return max(floor, min(value, ceil));
}
void SceneObject::initialize()
{
}
void SceneObject::postprocess(float ticktime)
{
    //Default simply sets matrix and processes sub objects, assuming they are visible
    //and on-screen. If/when you replace this you should still do this
    pushmatrix();
    automatrix();
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
      //  if (!(*p)->invisible && (*p)->on_screen()) {
            (*p)->postprocess(ticktime);
        //}
    }
    popmatrix();
}
QRectF SceneObject::get_intrinsic_aabbox()
{
    //We assume we have no intrinsic aabox
    return QRectF();
}
bool SceneObject::on_screen()
{
    if (aabbox.isNull())
        return false;
    return aabbox.intersects(QRectF(-1,-1,2,2));
}
