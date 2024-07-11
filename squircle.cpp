#include "squircle.h"
#include <QOpenGLFramebufferObject>

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>

#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLContext>
#include <QDebug>

class SquircleRenderer : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions {
    public:
        SquircleRenderer() : m_t(0.0){
            initializeOpenGLFunctions();            
            if (!m_program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex,
                                                    "attribute highp vec4 vertices;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    gl_Position = vertices;"
                                                    "    coords = vertices.xy;"
                                                    "}")) {
                qFatal("Cannot add vertex shader");
            }
            if (!m_program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment,
                                                    "uniform lowp float t;"
                                                    "varying highp vec2 coords;"
                                                    "void main() {"
                                                    "    lowp float i = 1. - (pow(abs(coords.x), 4.) + pow(abs(coords.y), 4.));"
                                                    "    i = smoothstep(t - 0.8, t + 0.8, i);"
                                                    "    i = floor(i * 20.) / 20.;"
                                                    "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
                                                    "}")) {
                qFatal("Cannot add fragment shader");
            }
            if (!m_program.link()) {
                qFatal("Cannot link shader program");
            }
            if (!m_program.isLinked()) {
                qFatal("Shader program not linked");
            }
            qDebug() << "Shader program linked";
            m_vertexLocation = m_program.attributeLocation("vertices");
            m_tLocation = m_program.uniformLocation("t");
            if (m_vertexLocation == -1 || m_tLocation == -1) {
                qFatal("m_vertexLocation = %d, m_tLocation = %d", m_vertexLocation, m_tLocation);
            }
            static const float values[] = {
                -1, -1,
                1, -1,
                -1, 1,
                1, 1,
            };
            glGenBuffers(1, &m_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(values), values, GL_STATIC_DRAW);
        }
        ~SquircleRenderer() {
            glDeleteBuffers(1, &m_vbo);
        }

        void render() {
            glClearColor(0, 1, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            
            m_program.bind();
            glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
            glVertexAttribPointer(m_vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(m_vertexLocation);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glUniform1f(m_tLocation, m_t);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
            glDisableVertexAttribArray(m_vertexLocation);

            m_program.release();
        }

        void synchronize(QQuickFramebufferObject *item) override {
            Squircle *squircle = static_cast<Squircle *>(item);
            m_t = squircle->getT();
        }

        QOpenGLFramebufferObject *createFramebufferObject(const QSize &size) {
            QOpenGLFramebufferObjectFormat format;
            format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
            format.setSamples(4);
            return new QOpenGLFramebufferObject(size, format);
        }

        void setT(qreal t) { m_t = t; }

    private:
        QOpenGLShaderProgram m_program;
        int m_vertexLocation;
        int m_tLocation;
        GLuint m_vbo;

        qreal m_t;
};

QQuickFramebufferObject::Renderer *Squircle::createRenderer() const {
    return new SquircleRenderer();
}
