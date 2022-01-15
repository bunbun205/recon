#include "OpenGLWindow.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

#include <QtGui/QOpenGLContext>
#include <QtGui/QOpenGLPaintDevice>
#include <QtGui/QPainter>

OpenGLWindow::OpenGLWindow(QWindow *parent) :
	QWindow(parent),
	mContext(nullptr),
	mDebugLogger(nullptr)
{
	setSurfaceType(QWindow::OpenGLSurface);
}


void OpenGLWindow::renderLater() {
	requestUpdate();
}


void OpenGLWindow::renderNow() {
	if (!isExposed())
		return;

	if (mContext == nullptr)
		initOpenGL();

	mContext->makeCurrent(this);

	paintGL();

	mContext->swapBuffers(this);
}


bool OpenGLWindow::event(QEvent *event) {
	switch (event->type()) {
	case QEvent::UpdateRequest:
		renderNow();
		return true;
	default:
		return QWindow::event(event);
	}
}


void OpenGLWindow::exposeEvent(QExposeEvent * /*event*/) {

	renderNow(); 
}


void OpenGLWindow::resizeEvent(QResizeEvent * event) {
	QWindow::resizeEvent(event);

	if (mContext == nullptr)
		initOpenGL();

	resizeGL(width(), height());
}


void OpenGLWindow::onMessageLogged(const QOpenGLDebugMessage &msg) {
	QString prefix;

	switch (msg.severity()) {
		case QOpenGLDebugMessage::NotificationSeverity:
			prefix += "++++";
		break;
		case QOpenGLDebugMessage::HighSeverity:
			prefix += "+++";
		break;
		case QOpenGLDebugMessage::MediumSeverity:
			prefix += "++";
		break;
		case QOpenGLDebugMessage::LowSeverity:
			prefix += "+";
		break;
	}

	prefix += " [";

#define CASE(c) case QOpenGLDebugMessage::c: prefix += #c; break
	switch (msg.source())
	{
	  CASE(APISource);
	  CASE(WindowSystemSource);
	  CASE(ShaderCompilerSource);
	  CASE(ThirdPartySource);
	  CASE(ApplicationSource);
	  CASE(OtherSource);
	  CASE(InvalidSource);
	}
 #undef CASE

	prefix += ":";

	// Format based on type
 #define CASE(c) case QOpenGLDebugMessage::c: prefix += #c; break
	switch (msg.type())
	{
		CASE(ErrorType);
		CASE(DeprecatedBehaviorType);
		CASE(UndefinedBehaviorType);
		CASE(PortabilityType);
		CASE(PerformanceType);
		CASE(OtherType);
		CASE(MarkerType);
		CASE(GroupPushType);
		CASE(GroupPopType);
	}
#undef CASE

	prefix += "] ";
	qDebug().noquote().nospace() << prefix << msg.message() << "\n";

}


void OpenGLWindow::initOpenGL() {
	Q_ASSERT(mContext == nullptr);

	mContext = new QOpenGLContext(this);
	mContext->setFormat(requestedFormat());
	mContext->create();

	mContext->makeCurrent(this);
	Q_ASSERT(mContext->isValid());

	initializeOpenGLFunctions();

#ifdef GL_DEBUG
	if (mContext->hasExtension(QByteArrayLiteral("GL_KHR_debug")))
		qDebug() << "GL_KHR_debug extension available";
	else
		qWarning() << "GL_KHR_debug extension *not* available";
	mDebugLogger = new QOpenGLDebugLogger(this);
	if (mDebugLogger->initialize()) {
		qDebug() << "Debug Logger initialized\n";
		connect(mDebugLogger, SIGNAL(messageLogged(QOpenGLDebugMessage)), this, SLOT(onMessageLogged(QOpenGLDebugMessage)));
		mDebugLogger->startLogging();
	}
#endif // GL_DEBUG

	initializeGL(); // call user code
}
