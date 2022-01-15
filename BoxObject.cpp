#include "BoxObject.h"

#include <QVector3D>
#include <QOpenGLShaderProgram>

#include "recon.h"

BoxObject::BoxObject() :
    mVBO(QOpenGLBuffer::VertexBuffer),
    mEBO(QOpenGLBuffer::IndexBuffer)
{
    Transform3D trans;

    auto pointCloud = PointCloud("MultiView/views.txt", 4876.8, 2032, 1520);

    const size_t pointCount = pointCloud.size();

    for(unsigned int i = 0; i < pointCount; ++i) {

        BoxMesh b(0.005, 0.005, 0.005);
        trans.setTranslation(pointCloud[i][0], pointCloud[i][1], pointCloud[i][2]);
        b.transform(trans.toMatrix());
        mBoxes.push_back(b);
    }

    unsigned int NBoxes = mBoxes.size();

    mVertexBufferData.resize(NBoxes*BoxMesh::VertexCount);
    mElementBufferData.resize(NBoxes*BoxMesh::IndexCount);

    Vertex * vertexBuffer = mVertexBufferData.data();
	unsigned int vertexCount = 0;
    GLuint * elementBuffer = mElementBufferData.data();
    for (const BoxMesh & b : mBoxes)
		b.copy2Buffer(vertexBuffer, elementBuffer, vertexCount);
}


void BoxObject::create(QOpenGLShaderProgram * shaderProgramm) {

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

    shaderProgramm->enableAttributeArray(0);
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
    shaderProgramm->enableAttributeArray(1);
	shaderProgramm->setAttributeBuffer(1, GL_FLOAT, offsetof(Vertex, r), 3, sizeof(Vertex));

    mVAO.release();
    mVBO.release();
    mEBO.release();
}


void BoxObject::destroy() {
    mVAO.destroy();
    mVBO.destroy();
    mEBO.destroy();
}


void BoxObject::render() {
    mVAO.bind();

    glDrawElements(GL_TRIANGLES, mElementBufferData.size(), GL_UNSIGNED_INT, nullptr);
    mVAO.release();
}
