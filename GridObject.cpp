#include "GridObject.h"

#include <QOpenGLShaderProgram>
#include <vector>


void GridObject::create(QOpenGLShaderProgram * shaderProgramm) {
    const unsigned int N = 1000;
	float width = 5000;
	std::vector<float>			gridVertexBufferData;
	mBufferSize = 2*N*2*2;
	gridVertexBufferData.resize(mBufferSize);
	float * gridVertexBufferPtr = gridVertexBufferData.data();
	float x1 = -width*0.5;
	float x2 = width*0.5;
	for (unsigned int i=0; i<N; ++i, gridVertexBufferPtr += 4) {
		float z = width/(N-1)*i-width*0.5;
		gridVertexBufferPtr[0] = x1;
		gridVertexBufferPtr[1] = z;
		gridVertexBufferPtr[2] = x2;
		gridVertexBufferPtr[3] = z;
	}
	float z1 = -width*0.5;
	float z2 = width*0.5;
	for (unsigned int i=0; i<N; ++i, gridVertexBufferPtr += 4) {
		float x = width/(N-1)*i-width*0.5;
		gridVertexBufferPtr[0] = x;
		gridVertexBufferPtr[1] = z1;
		gridVertexBufferPtr[2] = x;
		gridVertexBufferPtr[3] = z2;
	}

    mVAO.create();		
    mVAO.bind();		

    mVBO.create();
    mVBO.bind();
    mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
	int vertexMemSize = mBufferSize*sizeof(float);
	qDebug() << "GridObject - VertexBuffer size =" << vertexMemSize/1024.0 << "kByte";
    mVBO.allocate(gridVertexBufferData.data(), vertexMemSize);

	shaderProgramm->enableAttributeArray(0);
	shaderProgramm->setAttributeBuffer(0, GL_FLOAT, 0, 2, 0);

    mVAO.release();
    mVBO.release();
}


void GridObject::destroy() {
    mVAO.destroy();
    mVBO.destroy();
}


void GridObject::render() {
    mVAO.bind();
	glDrawArrays(GL_LINES, 0, mBufferSize);
    mVAO.release();
}
