#include "sprite.h"
#include <QVector3D>
#include <QHash>
#include "preloadasset.h"

using sf::Sprite;

static float coords[12] = {
    -50,-50,0,
    -50,50,0,
    50,-50,0,
    50,50,0
};
static float uvs[8] = {
    0,1,
    0,0,
    1,1,
    1,0,
};
static const char *vertex_shader =
    "attribute highp vec3 vertex_position;                      \n"
    "attribute highp vec2 vertex_uv_coordinate;                 \n"
    "uniform highp mat4 matrix;                                 \n"
    "varying highp vec2 frag_uv_coordinate;                     \n"
    "                                                           \n"
    "void main() {                                              \n"
    "   frag_uv_coordinate = vertex_uv_coordinate;              \n"
    "   vec4 real_vertex = vec4(0,0,0,1);                       \n"
    "   real_vertex.xyz = vertex_position;                      \n"
    "   gl_Position = matrix * real_vertex;                     \n"
    "}                                                          \n";

static const char *fragment_shader =
    "varying highp vec2 frag_uv_coordinate;                     \n"
    "uniform sampler2D texture;                                 \n"
    "void main() {                                              \n"
    "   vec4 texel = texture2D(texture, frag_uv_coordinate);    \n"
    "   gl_FragColor = texel;                                   \n"
    "}                                                          \n";

static const char *hitpoly_vshader =
    "attribute highp vec2 vertex_position;                      \n"
    "uniform highp mat4 matrix;                                 \n"
    "void main() {                                              \n"
    "   vec4 real_vertex = vec4(0,0,0,1);                       \n"
    "   real_vertex.xy = vertex_position;                       \n"
    "   gl_Position = matrix * real_vertex;                     \n"
    "}                                                          \n";

static const char* hitpoly_fshader =
        "uniform highp vec4 color;                                  \n"
        "void main() {                                              \n"
        "   gl_FragColor = color;                                   \n"
        "}                                                          \n";



// Allocate static storage
int Sprite::shader_vertex_position;
int Sprite::shader_vertex_uv_coordinate;
QOpenGLShaderProgram *Sprite::shader_program;
QHash<QString, QOpenGLTexture*> Sprite::loaded;

void Sprite::configure(QString asset)
{
    draw_hitpoly = DEFAULT_DRAW_HITPOLY;
    hitbox_color = QVector3D(1,0,0.5);
    if (shader_program == 0) {
        shader_program = new QOpenGLShaderProgram(NULL);
        shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader);
        shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader);
        shader_program->link();
        shader_vertex_position = shader_program->attributeLocation("vertex_position");
        shader_vertex_uv_coordinate = shader_program->attributeLocation("vertex_uv_coordinate");
    }
    if (loaded.contains(asset)) {
        texture = loaded[asset];
    } else {
        auto image = QImage(asset);
        if (image.isNull()) {
            qFatal("asset %s failed to load", asset.toStdString().c_str());
        }
        texture = new QOpenGLTexture(image.mirrored());
        texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        texture->setMagnificationFilter(QOpenGLTexture::Linear);
        texture->setWrapMode(QOpenGLTexture::ClampToEdge);
        loaded[asset] = texture;
    }
    //TODO Calculate this correctly some time
    hitpoly[0]  = 30; hitpoly[1]   = -50;
    hitpoly[2]  = 50; hitpoly[3]   = -30;
    hitpoly[4]  = 50; hitpoly[5]   = 30;
    hitpoly[6]  = 30; hitpoly[7]   = 50;
    hitpoly[8]  = -30; hitpoly[9]  = 50;
    hitpoly[10] = -50; hitpoly[11] = 30;
    hitpoly[12] = -50; hitpoly[13] = -30;
    hitpoly[14] = -30; hitpoly[15] = -50;

}
//This returns the object's normal axes, taking care to multiply in the matrix cached during tick
QList<QVector2D> Sprite::hitpolyNormals() {
    QList<QVector2D> rv;
    for (int i = 0; i < 8; i++) {
        int nxti = (i+1) % 8;
        QVector3D v1 = QVector3D(this->hitpoly[2*i], this->hitpoly[2*i+1],0);
        QVector3D v2 = QVector3D(this->hitpoly[nxti*2], this->hitpoly[nxti*2+1],0);
        QVector3D edge = (matrix*v1)-(matrix*v2);
        QVector2D normal = QVector2D(edge.y(), -edge.x());
      //  qDebug() << "normal for" <<v1 << v2 << "is " << normal;
        rv.append(normal);
    }
    return rv;
}
QPair<double, double> Sprite::hitpolyProject(QVector2D &axis)
{
    QList<QVector2D> vertices;
    for (int i = 0;i<8;i++){
        QVector3D v = QVector3D(this->hitpoly[2*i], this->hitpoly[2*i+1],0);
        v = matrix*v;
        vertices.append(QVector2D(v.x(), v.y()));
    }
    double min = QVector2D::dotProduct(axis, vertices[0]);
    double max = min;
    for (int i = 1; i<8;i++){
        double p = QVector2D::dotProduct(axis, vertices[i]);
        if (p < min) {
            min = p;
        } else if (p > max) {
            max = p;
        }
    }
    return QPair<double, double>(min, max);
}

void Sprite::tick(float ticktime)
{
    hitbox_color = QVector3D(1,0,0.5);
    SceneObject::tick(ticktime);
}

bool Sprite::hitpoly_intersects(QSharedPointer<sf::Sprite> other)
{
    //This is implementing the Separating Axes Theorem method of collision detection
    //Largely built by referring to http://www.dyn4j.org/2010/01/sat/
    //Step 1 build up all the axes
    QList<QVector2D> axes = hitpolyNormals();
    axes += other->hitpolyNormals();

    //Step 2 project the shapes on the axis and check for intersections
    foreach(auto ax, axes){
        auto proj1 = hitpolyProject(ax);
        auto proj2 = other->hitpolyProject(ax);
     //   qDebug() << "axis" << ax << "projections: " << proj1 << proj2;
        if (proj1.second < proj2.first || proj1.first > proj2.second) {
            hitbox_color = QVector4D(1,0,0.5,0.7);
            return false;
        }
    }
    hitbox_color = QVector4D(1,0,0,0.7);
    //If there are no gaps on all the axes tested, they intersect
    return true;
}

QRectF Sprite::get_intrinsic_aabbox()
{
    return QRectF(-50,-50,100,100);
}

void Sprite::render(float ticktime)
{
    pushmatrix();
    automatrix();

    if(draw_hitpoly)
    {
        auto hitprog = new QOpenGLShaderProgram(NULL);
        hitprog->addShaderFromSourceCode(QOpenGLShader::Vertex, hitpoly_vshader);
        hitprog->addShaderFromSourceCode(QOpenGLShader::Fragment, hitpoly_fshader);
        hitprog->link();
        int hitprog_vp = hitprog->attributeLocation("vertex_position");
        hitprog->bind();
        hitprog->setUniformValue("matrix", matrix);
        hitprog->setUniformValue("color", hitbox_color);
        glVertexAttribPointer(hitprog_vp, 2, GL_FLOAT, GL_FALSE,0,hitpoly);
        glEnableVertexAttribArray(hitprog_vp);
        glBlendFunc(GL_ONE, GL_ONE);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
        hitprog->setUniformValue("matrix", QMatrix4x4());
        hitprog->setUniformValue("color", QVector4D(0,1,0,1));
        float bbox [8];
        bbox[0] = aabbox.left(); //left bottom
        bbox[1] = aabbox.bottom();
        bbox[2] = aabbox.left(); //left top
        bbox[3] = aabbox.top();
        bbox[4] = aabbox.right(); //right bottom
        bbox[5] = aabbox.bottom();
        bbox[6] = aabbox.right(); //right top
        bbox[7] = aabbox.top();
        glVertexAttribPointer(hitprog_vp, 2, GL_FLOAT, GL_FALSE,0,bbox);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisableVertexAttribArray(hitprog_vp);
        hitprog->release();
    }

    //Enable the shader for this object
    shader_program->bind();
    //Set the current matrix
    shader_program->setUniformValue("matrix", getmatrix());

    texture->bind();
    shader_program->setUniformValue("texture",0);
    //Bind the vertex position attributes
    glVertexAttribPointer(shader_vertex_position, 3, GL_FLOAT, GL_FALSE,0,coords);
    glEnableVertexAttribArray(shader_vertex_position);
    //Bind the vertex uv coordinate attributes
    glVertexAttribPointer(shader_vertex_uv_coordinate, 2, GL_FLOAT, GL_FALSE,0,uvs);
    glEnableVertexAttribArray(shader_vertex_uv_coordinate);
    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    //Clean up
    glDisableVertexAttribArray(shader_vertex_position);
    glDisableVertexAttribArray(shader_vertex_uv_coordinate);
    shader_program->release();

    popmatrix();

    //Execute default render as well
    SceneObject::render(ticktime);
}
