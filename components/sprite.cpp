#include "sprite.h"
#include <QVector3D>

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

void Sprite::configure(QString asset)
{
    auto image = QImage(asset);
    if (image.isNull()) {
        qFatal("asset %s failed to load", asset.toStdString().c_str());
    }
    texture = new QOpenGLTexture(image.mirrored());
    shader_program = new QOpenGLShaderProgram(this);
    shader_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader);
    shader_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader);
    shader_program->link();
    shader_vertex_position = shader_program->attributeLocation("vertex_position");
    shader_vertex_uv_coordinate = shader_program->attributeLocation("vertex_uv_coordinate");
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
}

QRectF Sprite::get_intrinsic_aabbox()
{
    return QRectF(-50,-50,100,100);
}

void Sprite::render(float ticktime)
{
    pushmatrix();
    automatrix();

    auto nloc = getmatrix() * location;
    //qDebug() << "NLOC IS" << nloc;
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
