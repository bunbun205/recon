#include "ShaderProgram.h"

#include <QOpenGLShaderProgram>
#include <QDebug>

#include "OpenGLException.h"

ShaderProgram::ShaderProgram(const QString & vertexShaderFilePath, const QString & fragmentShaderFilePath) :
	mVertexShaderFilePath(vertexShaderFilePath),
	mFragmentShaderFilePath(fragmentShaderFilePath),
	mProgram(nullptr)
{
}


void ShaderProgram::create() {
	FUNCID(ShaderProgram::create);
	Q_ASSERT(mProgram == nullptr);

	mProgram = new QOpenGLShaderProgram();

	if (!mProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, mVertexShaderFilePath))
		throw OpenGLException(QString("Error compiling vertex shader %1:\n%2").arg(mVertexShaderFilePath).arg(mProgram->log()), FUNC_ID);

	if (!mProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, mFragmentShaderFilePath))
		throw OpenGLException(QString("Error compiling fragment shader %1:\n%2").arg(mFragmentShaderFilePath).arg(mProgram->log()), FUNC_ID);

	if (!mProgram->link())
		throw OpenGLException(QString("Shader linker error:\n%2").arg(mProgram->log()), FUNC_ID);

	mUniformIDs.clear();
	for (const QString & uniformName : mUniformNames)
		mUniformIDs.append( mProgram->uniformLocation(uniformName));
}


void ShaderProgram::destroy() {
	delete mProgram;
	mProgram = nullptr;
}
