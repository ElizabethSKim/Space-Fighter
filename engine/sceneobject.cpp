
#include "sceneobject.h"
#include "engine.h"


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
}
SceneObject::~SceneObject()
{
    total_live_scene_objects--;
}
void SceneObject::test2() {
    qDebug() << "SdfsFD";
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
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
        (*p)->tick(ticktime);
    }
    popmatrix();

    //Now that all sub nodes are ticked, lets update our bbox
    //First we need to get our own intrinsic aabbox and convert
    //it to screen coordinates
    QRectF rv = get_intrinsic_aabbox();
    //qDebug() << "our intrinsic bb is " << rv;
    bool set = !rv.isNull();
    if (set)
    {
        QPointF tl = rv.topLeft();
        QVector4D real_tl = matrix * QVector4D(tl.x(),tl.y(),0,1);
        QPointF br = rv.bottomRight();
        QVector4D real_br = matrix * QVector4D(br.x(),br.y(),0,1);
        float left = min(real_tl.x(), real_br.x());
        float right = max(real_tl.x(), real_br.x());
        float top = min(real_tl.y(), real_br.y());
        float bottom = max(real_tl.y(), real_br.y());
        rv = QRectF(QPointF(left,top), QPointF(right, bottom));
        //qDebug() << "after matrix mul aabb is" << rv;
    }
    for (auto p = child_nodes.begin(); p != child_nodes.end(); p++) {
        QRectF cbb = (*p)->aabbox;
        if (!cbb.isNull())
        {
            //Valid bounding box:
            if (!set) {
                rv = cbb;
                set = true;
            } else {
                //qDebug() << "uniting our bb from " << rv << " to " << rv.united(cbb) << " with cbb" << cbb;
                rv = rv.united(cbb);
            }
        }
    }
    //qDebug() << "after merging etc aabb is " << rv;
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
        if (!(*p)->invisible && (*p)->on_screen()) {
            (*p)->postprocess(ticktime);
        }
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
