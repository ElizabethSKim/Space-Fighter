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
#include <QGamepad>

#include "sceneobject.h"
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;

#define SCENE_WIDTH 1920
#define SCENE_HEIGHT 1080
#define SCENE_NEAR -100
#define SCENE_FAR 100

class Engine : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Engine(QWindow *parent = 0);
    ~Engine();

    template <typename T, typename ... ArgT> object_ptr spawn(ArgT... args)
    {
        T *raw = new T();
        raw->set_engine(this);
        raw->configure(args...);
        return object_ptr(raw);
    }

    void start();

    QStack<QMatrix4x4> stack;
    object_ptr root_obj;
    QGamepad *gamepad;
    QElapsedTimer gametime;

    void runLoadFunctions();

    static void onLoad(std::function<void(Engine*)> cb);

public slots:
    void renderLater();
    void renderNow();

signals:
    void leftJoyChanged(QVector2D v);
    void rightJoyChanged(QVector2D v);
    void dpadL(bool down);
    void dpadR(bool down);
    void dpadU(bool down);
    void dpadD(bool down);
    void leftTrigChanged(double v);
    void rightTrigChanged(double v);
    void butA(bool down);
    void butB(bool down);
    void butX(bool down);
    void butY(bool down);
    void leftBumper(bool down);
    void rightBumper(bool down);
    void startPressed();
    void backPressed();

protected:
    void initGamePad();
    void render();
    void collateCollidables(object_ptr o, QList<object_ptr> &list);
    void checkCollisions(QList<object_ptr> &list);
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:

    bool m_update_pending;
    bool m_animating;

    qint64 last_tick;
    QOpenGLContext *m_context;
    QOpenGLPaintDevice *m_device;
    QTimer fpstimer;
    int frames;

};

#endif
