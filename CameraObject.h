#ifndef CAMERAOBJECT_H
#define CAMERAOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

#include "CameraMesh.h"

class CameraObject
{
public:
    CameraObject();

    void create(QOpenGLShaderProgram * shaderProgramm);
    void destroy();

    void render();

    std::vector<CameraMesh> cameras;

    std::vector<Vertex>			mVertexBufferData;
    std::vector<GLuint>			mElementBufferData;
    QOpenGLVertexArrayObject	mVAO;
    QOpenGLBuffer				mVBO;
    QOpenGLBuffer				mEBO;
};

#endif // CAMERAOBJECT_H
