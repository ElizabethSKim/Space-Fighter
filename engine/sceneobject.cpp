
#include "sceneobject.h"
#include "scenegraph.h"


void SceneObject::tick(float ticktime)
{
    //Linear
    QVector3D delta_velocity = acceleration*ticktime;
    QVector3D delta_location = velocity*ticktime;
    location += delta_location;
    velocity += delta_velocity;

    //Angular
    rotacceleration.normalize();
    QQuaternion delta_rotvelocity = rotacceleration*ticktime;
    QQuaternion delta_rotation = rotvelocity*ticktime;
    rotation += delta_rotation;
    rotation.normalize();
    rotvelocity += delta_rotvelocity;
    rotvelocity.normalize();

    //Bind this object to the OpenGL context, and also call the init function
    if (!is_initialized)
    {
        initializeOpenGLFunctions();
        initialize();
        is_initialized = true;
    }

    pushmatrix();
    automatrix();
    //Also tick sub nodes
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
        (*p)->tick(ticktime);
    }
    popmatrix();

    //Now that all sub nodes are ticked, lets update our bbox
    //First we need to get our own intrinsic aabbox and convert
    //it to screen coordinates
    QRect rv = get_intrinsic_aabbox();
    bool set = rv.width() != 0 && rv.height() != 0;
    if (set)
    {
        QPoint tl = rv.topLeft();
        QVector3D real_tl = getmatrix() * QVector3D(tl);
        QPoint br = rv.bottomRight();
        QVector3D real_br = getmatrix() * QVector3D(br);
        float left = floor(min(real_tl.x(), real_br.x()));
        float right = ceil(max(real_tl.x(), real_br.x()));
        float top = floor(min(real_tl.y(), real_br.y()));
        float bottom = ceil(max(real_tl.y(), real_br.y()));
        rv = QRect(QPoint(left,top), QPoint(right, bottom));
    }
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
        QRect cbb = (*p)->aabbox;
        if (cbb.height() != 0 && cbb.width() != 0)
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
void SceneObject::render(float ticktime)
{
    //Default simply sets matrix and processes sub objects, assuming they are visible
    //and on-screen. If/when you replace this you should still do this
    pushmatrix();
    automatrix();
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
        if ((*p)->visible && (*p)->on_screen()) {
            (*p)->render(ticktime);
        }
    }
    popmatrix();
}
void SceneObject::set_engine(SceneGraph *engine)
{
    this->engine = engine;
}

void SceneObject::matrix_rotate(QQuaternion &q)
{
    engine->stack.top().rotate(q);
}
void SceneObject::matrix_rotate(float radians)
{
    engine->stack.top().rotate(radians, 0, 0, -1);
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
    matrix_scale(scale);
    matrix_rotate(rotation);
    matrix_translate(location);
}
float SceneObject::min(float a, float b)
{
    return a<b?a:b;
}
float SceneObject::max(float a, float b)
{
    return a>b?a:b;
}
float SceneObject::clamp(float value, float floor, float ceil)
{
    return max(floor, min(value, ceil));
}
void SceneObject::initialize()
{
    //No default behavior, children will get initialized when we tick them
}
void SceneObject::postprocess(float ticktime)
{
    //Default simply sets matrix and processes sub objects, assuming they are visible
    //and on-screen. If/when you replace this you should still do this
    pushmatrix();
    automatrix();
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
        if ((*p)->visible && (*p)->on_screen()) {
            (*p)->postprocess(ticktime);
        }
    }
    popmatrix();
}
QRect SceneObject::get_intrinsic_aabbox()
{
    //We assume we have no intrinsic aabox
    return QRect(0,0,0,0);
}
bool SceneObject::on_screen()
{
    return aabbox.intersects(QRect(0,0,SCENE_WIDTH, SCENE_HEIGHT));
}
