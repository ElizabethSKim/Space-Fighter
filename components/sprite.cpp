#include "sprite.h"

using sf::Sprite;

void Sprite::configure(QString asset)
{
    texture = new QOpenGLTexture(QImage(asset).mirrored());
    
}

static const char *vertex_shader =
    "attribute highp vec4 vertex_position;                      \n"
    "attribute highp vec4 vertex_uv_coordinate;                 \n"
    "uniform highp mat4 matrix;                                 \n"
    "varying highp vec4 frag_uv_coordinate                      \n"
    "                                                           \n"
    "void main() {                                              \n"
    "   frag_uv_coordinate = vertex_uv_coordinate               \n"
    "   gl_Position = matrix * vertex_position;                 \n"
    "}                                                          \n";

static const char *fragment_shader =
    "varying highp vec4 frag_uv_coordinate;                     \n"
    "uniform sampler2D texture;                                 \n"
    "void main() {                                              \n"
    "   vec4 texel = texture2D(texture, frag_uv_coordinate);    \n"
    "   gl_FragColor = texel;                                   \n"
    "}                                                          \n";
