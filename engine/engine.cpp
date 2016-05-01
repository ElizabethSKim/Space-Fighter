#include "engine.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>
#include <QtGamepad/QGamepad>

Engine::Engine(QWindow *parent)
    : QWindow(parent)
    , m_update_pending(false)
    , m_animating(false)
    , m_context(0)
    , m_device(0)
    , frames(0)
{
    setSurfaceType(QWindow::OpenGLSurface);
    QSurfaceFormat format;
    format.setSamples(16);
    setFormat(format);

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
            //qDebug() << "col" << i << j;
            //qqDebug() << " aabbs" << lhs->aabbox << "  " << rhs->aabbox;
            if (lhs->aabbox.intersects(rhs->aabbox))
            {
                //TODO perform "expensive" but accurate per-pixel collision test
                emit lhs->collided(rhs, true);
                emit rhs->collided(lhs, false);
            }
        }
    }
}

void Engine::render()
{
    //Init OpenGL
    if (!m_device)
        m_device = new QOpenGLPaintDevice;
    //Work out our tick time
    qint64 now = gametime.elapsed();
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
    //qDebug() << "we have " << collidables.length() << " active collidable objects";
    checkCollisions(collidables);

    //Then we render the first pass,
    glViewport(0, 0, width(), height());

    //first clear our framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Change the GL context to match our window
    m_device->setSize(size());

    //Set up an orthographic projection matrix
    stack.push(stack.top());
    //Then descend into the tree
    root_obj->render(ticktime);

    //TODO postprocess shader rendering

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
