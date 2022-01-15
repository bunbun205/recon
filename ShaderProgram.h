#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QString>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QOpenGLShaderProgram;
QT_END_NAMESPACE

class ShaderProgram {
public:
	ShaderProgram();
	ShaderProgram(const QString & vertexShaderFilePath, const QString & fragmentShaderFilePath);

	void create();
	void destroy();

	QOpenGLShaderProgram * shaderProgram() { return mProgram; }

	QString		mVertexShaderFilePath;
	QString		mFragmentShaderFilePath;


	QStringList	mUniformNames;

	QList<int>	mUniformIDs;

private:
	QOpenGLShaderProgram	*mProgram;
};

#endif // SHADERPROGRAM_H
