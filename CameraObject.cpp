#include "CameraObject.h"

#include <QVector3D>
#include <QOpenGLShaderProgram>

CameraObject::CameraObject()
    :mVBO(QOpenGLBuffer::VertexBuffer),
     mEBO(QOpenGLBuffer::IndexBuffer){

    CameraMesh c;
    Transform3D trans;
    trans.setTranslation(0, 1, 0);
    c.transform(trans.toMatrix());
    cameras.push_back(c);

    Vertex * vertexBuffer = mVertexBufferData.data();
    unsigned int vertexCount = 0;
    GLuint * elementBuffer = mElementBufferData.data();
    c.copy2Buffer(vertexBuffer, elementBuffer, vertexCount);
}

void CameraObject::create(QOpenGLShaderProgram *shaderProgramm) {

    mVAO.create();
    mVAO.bind();

    mVBO.create();
    mVBO.bind();
    mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int vertexMemSize = mVertexBufferData.size()*sizeof(Vertex);
    qDebug() << "BoxObject - VertexBuffer size =" << vertexMemSize/1024.0 << "kByte";
    mVBO.allocate(mVertexBufferData.data(), vertexMemSize);

    mEBO.create();
    mEBO.bind();
    mEBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
    int elementMemSize = mElementBufferData.size()*sizeof(GLuint);
    qDebug() << "BoxObject - ElementBuffer size =" << elementMemSize/1024.0 << "kByte";
    mEBO.allocate(mElementBufferData.data(), elementMemSize);

    shaderProgramm->enableAttributeArray(0); // array with index/id 0
    shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));

    shaderProgramm->enableAttributeArray(1); // array with index/id 1
    shaderProgramm->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, r), 3, sizeof(Vertex));

    mVAO.release();
    mVBO.release();
    mEBO.release();
}

void CameraObject::destroy(){

    mVAO.destroy();
    mVBO.destroy();
    mEBO.destroy();
}

void CameraObject::render() {
    mVAO.bind();
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
    glDrawElements(GL_TRIANGLES, mElementBufferData.size(), GL_UNSIGNED_INT, nullptr);
    mVAO.release();
}
















