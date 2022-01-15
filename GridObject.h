#ifndef OPENGLGRIDOBJECT_H
#define OPENGLGRIDOBJECT_H

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE


class GridObject {
public:
	void create(QOpenGLShaderProgram * shaderProgramm);
	void destroy();

	void render();

	unsigned int				mBufferSize;

	QOpenGLVertexArrayObject	mVAO;
	QOpenGLBuffer				mVBO;

};

#endif // OPENGLGRIDOBJECT_H
