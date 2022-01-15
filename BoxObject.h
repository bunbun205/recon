#ifndef BOXOBJECT_H
#define BOXOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

#include "BoxMesh.h"

class BoxObject {
public:
	BoxObject();

	void create(QOpenGLShaderProgram * shaderProgramm);
	void destroy();

	void render();

    std::vector<BoxMesh>		mBoxes;

    std::vector<Vertex>			mVertexBufferData;
    std::vector<GLuint>			mElementBufferData;

    QOpenGLVertexArrayObject	mVAO;

    QOpenGLBuffer				mVBO;
    QOpenGLBuffer				mEBO;
};

#endif // BOXOBJECT_H
