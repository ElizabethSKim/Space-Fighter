#include "engine.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGamepad/QGamepad>

#include <components/sprite.h>

Engine::Engine(QWindow *parent)
    : QWindow(parent)
    , m_update_pending(false)
    , m_animating(false)
    , m_context(0)
    , m_device(0)
    , last_tick(0)
    , frames(0)

{
    setSurfaceType(QWindow::OpenGLSurface);
    QSurfaceFormat format;
    format.setSamples(16);
    setFormat(format);
    gametime.start();
    fpstimer.setInterval(1000);
    connect(&fpstimer, &QTimer::timeout, [this]{
        qDebug("FPS: %d , live objects %d",this->frames, SceneObject::total_live_scene_objects);
        this->frames = 0;
    });

    fpstimer.start();

    //Get controls
    auto gamepads = QGamepadManager::instance()->connectedGamepads();
    if (gamepads.empty())
    {
        qDebug("There are no connected gamepads");
    }
    else
    {
        gamepad = new QGamepad(*gamepads.begin(), this);
        initGamePad();
    }
}

Engine::~Engine()
{
    delete m_device;
}

static QList<std::function<void(Engine*)>> &slist() {
    static QList<std::function<void(Engine*)>> lst;
    return lst;
}

void Engine::onLoad(std::function<void(Engine*)> cb) {
    slist().append(cb);
}

void Engine::runLoadFunctions() {
    for (int i = 0; i < slist().length();i++) {
        slist()[i](this);
        qDebug() << "loaded" << i << "out of" << slist().length();
    }
}

void Engine::initGamePad()
{
    connect(gamepad, &QGamepad::axisLeftXChanged, this, [this](double value){
        double x = value;
        double y = gamepad->axisLeftY();
        emit leftJoyChanged(QVector2D(x,y));
    });
    connect(gamepad, &QGamepad::axisLeftYChanged, this, [this](double value){
        double x = gamepad->axisLeftX();
        double y = value;
        emit leftJoyChanged(QVector2D(x,y));
    });
    connect(gamepad, &QGamepad::axisRightXChanged, this, [this](double value){
        double x = value;
        double y = gamepad->axisRightY();
        emit rightJoyChanged(QVector2D(x,y));
    });
    connect(gamepad, &QGamepad::axisRightYChanged, this, [this](double value){
        double x = gamepad->axisRightX();
        double y = value;
        emit rightJoyChanged(QVector2D(x,y));
    });
    connect(gamepad, &QGamepad::buttonAChanged, this, &Engine::butA);
    connect(gamepad, &QGamepad::buttonBChanged, this, &Engine::butB);
    connect(gamepad, &QGamepad::buttonXChanged, this, &Engine::butX);
    connect(gamepad, &QGamepad::buttonYChanged, this, &Engine::butY);
    connect(gamepad, &QGamepad::buttonL1Changed, this, &Engine::leftBumper);
    connect(gamepad, &QGamepad::buttonR1Changed, this, &Engine::rightBumper);
    connect(gamepad, &QGamepad::buttonLeftChanged, this, &Engine::dpadL);
    connect(gamepad, &QGamepad::buttonRightChanged, this, &Engine::dpadR);
    connect(gamepad, &QGamepad::buttonUpChanged, this, &Engine::dpadU);
    connect(gamepad, &QGamepad::buttonDownChanged, this, &Engine::dpadD);
    connect(gamepad, &QGamepad::buttonL2Changed, this, &Engine::leftTrigChanged);
    connect(gamepad, &QGamepad::buttonR2Changed, this, &Engine::rightTrigChanged);
    connect(gamepad, &QGamepad::buttonSelectChanged, this, &Engine::backPressed);
    connect(gamepad, &QGamepad::buttonStartChanged, this, &Engine::startPressed);
}

void Engine::collateCollidables(object_ptr o, QList<object_ptr> &list)
{
    //Create a list of all collidable objects
    if (o->collidable && !o->invisible)
        list.append(o);

    else
        for(auto child : o->child_nodes)
            collateCollidables(child, list);
}

void Engine::checkCollisions(QList<object_ptr> &list)
{
    for (int i = 0; i < list.length()-1; i++)
    {
        for (int j = i+1; j < list.length(); j++)
        {
            object_ptr lhs = list[i];
            object_ptr rhs = list[j];
            if (lhs->aabbox.intersects(rhs->aabbox))
            {
                auto lhs_sprites = lhs->get_collidable_sprites();
                auto rhs_sprites = rhs->get_collidable_sprites();
                foreach(auto lhs_sprite, lhs_sprites)
                {
                    foreach(auto rhs_sprite, rhs_sprites)
                    {
                        if(lhs_sprite->hitpoly_intersects(rhs_sprite))
                        {
                            emit lhs->collided(lhs, rhs, true);
                            emit rhs->collided(rhs, lhs, false);
                        }
                    }
                }
            }
        }
    }
}

static const char *fshader =
    "#version 120                                               \n"
    "varying vec2 UV;                                           \n"
    "uniform sampler2D tex;                                     \n"
    "void main(){                                               \n"
    "    gl_FragColor = texture2D( tex, UV);                    \n"
    "}                                                          \n";

static const char *vshader =
    "#version 120                                               \n"
    "attribute highp vec3 vertex_position;                      \n"
    "varying highp vec2 UV;                                     \n"
    "void to_uv(in vec3 pos, out vec2 uv)                       \n"
    "{uv=(pos.xy + 1).xy/2;}                                    \n"
    "void main() {                                              \n"
    "   to_uv(vertex_position, UV);                             \n"
    "   gl_Position.xyz = vertex_position;                      \n"
    "   gl_Position.w = 1;                                      \n"
    "}                                                          \n";

static const float fullscreen_quad[] = {-1,-1,-1,1,1,1,1,-1};
void Engine::render()
{
    //Init OpenGL
    if (!m_device) {
        m_device = new QOpenGLPaintDevice;
        runLoadFunctions();
        postprog = new QOpenGLShaderProgram(NULL);
        postprog->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader);
        postprog->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader);
        postprog->link();
    }

    //Work out our tick time
    qint64 now = gametime.elapsed();
    if (last_tick == 0) {
        last_tick = now;
    }
    float ticktime = (float)((now - last_tick) / 1000.0);
    last_tick = now;

    stack.clear();
    QMatrix4x4 matrix;
    matrix.ortho(0, SCENE_WIDTH, SCENE_HEIGHT, 0, SCENE_NEAR, SCENE_FAR);
    stack.push(matrix);
    stack.push(matrix);
    //First we tick the scene
    root_obj->tick(ticktime);
    stack.pop();

    //Now we check for collision
    QList<object_ptr> collidables;
    collateCollidables(root_obj, collidables);
    checkCollisions(collidables);

    int w_width = width(); int w_height = height();

    GLuint framebuf, frametex;
    glGenFramebuffers(1, &framebuf);
    glGenTextures(1, &frametex);
    glBindTexture(GL_TEXTURE_2D, frametex);
    //I think the filters only apply when we sample from it later...
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //This is important..
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //Wonder what color depth we need?
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w_width, w_height, 0,GL_RGBA, GL_RGBA8, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frametex, 0);

    GLenum db[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, db);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        qFatal("could not do render-to-FBO");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, framebuf);

    //Then we render the first pass,
    glViewport(0, 0, w_width, w_height);

    //first clear our framebuffer
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Change the GL context to match our window
    m_device->setSize(QSize(w_width, w_height));

    //Set up an orthographic projection matrix
    stack.push(stack.top());
    //Then descend into the tree
    root_obj->render(ticktime);

    //TODO postprocess shader rendering
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0,0,w_width, w_height);

    int postprog_vp = postprog->attributeLocation("vertex_position");
    postprog->bind();
    glBindTexture(GL_TEXTURE0, frametex);
    postprog->setUniformValue("tex", 0);
    glVertexAttribPointer(postprog_vp, 2, GL_FLOAT, GL_FALSE,0,fullscreen_quad);
    glEnableVertexAttribArray(postprog_vp);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glDisableVertexAttribArray(postprog_vp);
    postprog->release();

    GLuint tex [] = {frametex};
    glDeleteTextures(1, tex);
    GLuint fbz [] = {framebuf};
    glDeleteFramebuffers(1, fbz);
    frames++;
}

void Engine::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool Engine::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::UpdateRequest:
        m_update_pending = false;
        renderNow();
        return true;
    default:
        return QWindow::event(event);
    }
}

void Engine::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void Engine::renderNow()
{
    if (!isExposed())
        return;

    bool needsInitialize = false;

    if (!m_context) {
        m_context = new QOpenGLContext(this);
        m_context->setFormat(requestedFormat());
        m_context->create();

        needsInitialize = true;
    }

    m_context->makeCurrent(this);

    if (needsInitialize) {
        initializeOpenGLFunctions();
    }

    render();

    m_context->swapBuffers(this);

    if (m_animating)
        renderLater();
}

void Engine::start()
{
    m_animating = true;
    renderLater();
}
