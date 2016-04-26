#ifndef __SCENE_OBJECT_H__
#define __SCENE_OBJECT_H__

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtCore/qmath.h>
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QElapsedTimer>
#include <QTimer>

#include "sceneobject.h"
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

#define SCENE_WIDTH 1920
#define SCENE_HEIGHT 1080
#define SCENE_NEAR -100
#define SCENE_FAR 100

class SceneGraph : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit SceneGraph(QWindow *parent = 0);
    ~SceneGraph();

    template <typename T> object_ptr spawn()
    {
        T *raw = new T();
        qobject_cast<SceneObject*>(raw)->set_engine(this);
        return object_ptr(raw);
    }

    void start();

    QStack<QMatrix4x4> stack;
    object_ptr root_obj;

public slots:
    void renderLater();
    void renderNow();

protected:
    void render();
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:


    bool m_update_pending;
    bool m_animating;

    QElapsedTimer gametime;
    qint64 last_tick;
    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
    QTimer fpstimer;
    int frames;

};

#endif
