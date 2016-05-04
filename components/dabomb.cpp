#include "dabomb.h"

using namespace sf;

static float coords[8] = {
    -1,-1,
    -1,1,
    1,-1,
    1,1,
};

static const char *fshader =
    "#version 120                                               \n"
    "varying vec2 UV;                                           \n"
    "uniform vec3 loc;                                          \n"
    "uniform sampler2D tex;                                     \n"
    "uniform float time;                                        \n"
    "varying vec2 sc; \n"
    "void main(){                                                           \n"
    "    float radius = (3000-time)/3000.0;                                 \n"
    "    float fading = (3000-time)/3000.0;                                 \n"
    "    float dist = sqrt(pow(sc.x - loc.x, 2) + pow(sc.y - loc.y, 2));          \n"
    "    float dstrength = 0.3*1/dist*sin(15*dist*(1-fading))*fading;"
    "    vec2 distortion = vec2(loc.x - sc.x, loc.y - sc.y)*dstrength;       \n"
    "    vec2 nuv = UV + distortion;"
    "    gl_FragColor = texture2D( tex, nuv); \n"
    "}                                                          \n";

static const char *vshader =
    "#version 120                                               \n"
    "attribute highp vec3 vertex_position;                      \n"
    "varying highp vec2 UV;   "
    "varying vec2 sc;                                  \n"
    "void to_uv(in vec3 pos, out vec2 uv)                       \n"
    "{uv=(pos.xy + 1).xy/2;}                                    \n"
    "void main() {                                              \n"
    "   to_uv(vertex_position, UV);                             \n"
    "   gl_Position.xyz = vertex_position;                      \n"
    "   sc = vertex_position.xy; \n"
    "   gl_Position.w = 1;                                      \n"
    "}                                                          \n";

DaBomb::DaBomb()
{
    location = QVector3D(SCENE_WIDTH/2,SCENE_HEIGHT/2,0);
}

void DaBomb::configure()
{

}
void DaBomb::start()
{
    started = true;
    frametime.start();
}

void DaBomb::tick(float ticktime)
{
    SceneObject::tick(ticktime);
}
void DaBomb::postprocess(float ticktime)
{
    SceneObject::postprocess(ticktime);

    if (!started) return;

    auto matrix = QMatrix4x4();
    matrix.ortho(0, SCENE_WIDTH, SCENE_HEIGHT, 0, SCENE_NEAR, SCENE_FAR);

    QVector3D screencoords = matrix * location;
   // QVector3D screencoords (0,0,0);

    auto hitprog = new QOpenGLShaderProgram(NULL);
    hitprog->addShaderFromSourceCode(QOpenGLShader::Vertex, vshader);
    hitprog->addShaderFromSourceCode(QOpenGLShader::Fragment, fshader);
    hitprog->link();
    int hitprog_vp = hitprog->attributeLocation("vertex_position");
    hitprog->bind();
    hitprog->setUniformValue("loc",screencoords);
    hitprog->setUniformValue("time", (float)(frametime.elapsed()));
    glVertexAttribPointer(hitprog_vp, 2, GL_FLOAT, GL_FALSE,0,coords);
    glEnableVertexAttribArray(hitprog_vp);
    glBlendFunc(GL_ONE, GL_ZERO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glDisableVertexAttribArray(hitprog_vp);
    hitprog->release();
    if (frametime.elapsed() > 3000)
    {
        started = false;
    }

}
