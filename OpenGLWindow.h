#ifndef OpenGLWindow_H
#define OpenGLWindow_H

#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>

#include <QOpenGLDebugLogger>

QT_BEGIN_NAMESPACE
class QOpenGLContext;
class QOpenGLContext;
QT_END_NAMESPACE

class OpenGLWindow : public QWindow, protected QOpenGLFunctions {
	Q_OBJECT
public:
	explicit OpenGLWindow(QWindow *parent = nullptr);

public slots:
	void renderLater();

	void renderNow();

protected:
	bool event(QEvent *event) override;
	void exposeEvent(QExposeEvent *event) override;
	void resizeEvent(QResizeEvent *) override;

	virtual void initializeGL() = 0;

	virtual void resizeGL(int width, int height) { Q_UNUSED(width) Q_UNUSED(height) }

	virtual void paintGL() = 0;


	QOpenGLContext		*mContext;

private slots:

	void onMessageLogged(const QOpenGLDebugMessage &msg);


private:
	void initOpenGL();

	QOpenGLDebugLogger	*mDebugLogger;
};

#endif // OpenGLWindow_H
