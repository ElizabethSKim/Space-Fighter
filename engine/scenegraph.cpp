#include "scenegraph.h"

#include <QtCore/QCoreApplication>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

SceneGraph::SceneGraph(QWindow *parent)
    : QWindow(parent)
    , m_update_pending(false)
    , m_animating(false)
    , m_context(0)
    , m_device(0)
{
    setSurfaceType(QWindow::OpenGLSurface);
    QSurfaceFormat format;
    format.setSamples(16);
    setFormat(format);

    fpstimer.setInterval(1000);
    connect(&fpstimer, &QTimer::timeout, [this]{
        qDebug("FPS: %d",this->frames);
        this->frames = 0;
    });

    fpstimer.start();
}

SceneGraph::~SceneGraph()
{
    delete m_device;
}

void SceneGraph::render()
{
    //Init OpenGL
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    //Work out our tick time
    qint64 now = gametime.elapsed();
    float ticktime = (float)((now - last_tick) / 1000.0);
    last_tick = now;

    //First we tick the scene
    root_obj->tick(ticktime);
    //Then we render the first pass,

    //first clear our framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //Change the GL context to match our window
    m_device->setSize(size());

    //Set up an orthographic projection matrix
    stack.clear();
    QMatrix4x4 matrix;
    matrix.ortho(0, SCENE_WIDTH, SCENE_HEIGHT, 0, SCENE_NEAR, SCENE_FAR);
    stack.push(matrix);

    //Then descend into the tree
    root_obj->render(ticktime);

    //TODO postprocess shader rendering

    frames++;

}

void SceneGraph::renderLater()
{
    if (!m_update_pending) {
        m_update_pending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool SceneGraph::event(QEvent *event)
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

void SceneGraph::exposeEvent(QExposeEvent *event)
{
    Q_UNUSED(event);

    if (isExposed())
        renderNow();
}

void SceneGraph::renderNow()
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

void SceneGraph::start()
{
    m_animating = true;
    renderLater();
}
