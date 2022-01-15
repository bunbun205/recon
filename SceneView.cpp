#include "SceneView.h"

#include <QExposeEvent>
#include <QOpenGLShaderProgram>
#include <QDateTime>

#include "DebugApplication.h"

#define SHADER(x) mShaderPrograms[x].shaderProgram()

SceneView::SceneView() :
	mInputEventReceived(false)
{

	mKeyboardMouseHandler.addRecognizedKey(Qt::Key_W);
	mKeyboardMouseHandler.addRecognizedKey(Qt::Key_A);
	mKeyboardMouseHandler.addRecognizedKey(Qt::Key_S);
	mKeyboardMouseHandler.addRecognizedKey(Qt::Key_D);
	mKeyboardMouseHandler.addRecognizedKey(Qt::Key_Q);
	mKeyboardMouseHandler.addRecognizedKey(Qt::Key_E);
	mKeyboardMouseHandler.addRecognizedKey(Qt::Key_Shift);

    ShaderProgram blocks("shaders/withWorldAndCamera.vert","shaders/simple.frag");
	blocks.mUniformNames.append("worldToView");
	mShaderPrograms.append( blocks );

//    ShaderProgram frustums("shaders/withWorldAndCamera.vert","shaders/simple.frag");
//    frustums.mUniformNames.append("worldToView");
//    mShaderPrograms.append( frustums );

    ShaderProgram grid("shaders/grid.vert","shaders/grid.frag");
	grid.mUniformNames.append("worldToView"); // mat4
	grid.mUniformNames.append("gridColor"); // vec3
	grid.mUniformNames.append("backColor"); // vec3
	mShaderPrograms.append( grid );

	mCamera.translate(-5,10,15);
	mCamera.rotate(-30, mCamera.right());
	mCamera.rotate(-25, QVector3D(0.0f, 1.0f, 0.0f));
}


SceneView::~SceneView() {
	if (mContext) {
		mContext->makeCurrent(this);

		for (ShaderProgram & p : mShaderPrograms)
			p.destroy();

		mBoxObject.destroy();
//        	mCameraObject.destroy();
		mGridObject.destroy();

		mGPUTimers.destroy();
	}
}


void SceneView::initializeGL() {
	FUNCID(SceneView::initializeGL);
	try {
		for (ShaderProgram & p : mShaderPrograms)
			p.create();

		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		mBoxObject.create(SHADER(0));
	//      mCameraObject.create(SHADER(1));
        	mGridObject.create(SHADER(1));

		mGPUTimers.setSampleCount(5);
		mGPUTimers.create();
	}
	catch (OpenGLException & ex) {
		throw OpenGLException(ex, "OpenGL initialization failed.", FUNC_ID);
	}
}


void SceneView::resizeGL(int width, int height) {
	mProjection.setToIdentity();
	mProjection.perspective(45.0f, width / float(height), 0.01f, 1000.0f);

	updateWorld2ViewMatrix();
}


void SceneView::paintGL() {
	mCPUTimer.start();
	if (((DebugApplication *)qApp)->mAboutToTerminate)
		return;

	if (mInputEventReceived)
		processInput();

	const qreal retinaScale = devicePixelRatio(); 
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);
	qDebug() << "SceneView::paintGL(): Rendering to:" << width() << "x" << height();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	QVector3D backColor(0.1f, 0.15f, 0.3f);
	glClearColor(0.1f, 0.15f, 0.3f, 1.0f);

	QVector3D gridColor(0.5f, 0.5f, 0.7f);

	mGPUTimers.reset();

	mGPUTimers.recordSample(); 
	SHADER(0)->bind();
	SHADER(0)->setUniformValue(mShaderPrograms[0].mUniformIDs[0], mWorldToView);

	mGPUTimers.recordSample(); 
	mBoxObject.render();
	SHADER(0)->release();

    mGPUTimers.reset();

    mGPUTimers.recordSample(); 

//    SHADER(1)->bind();
//    SHADER(1)->setUniformValue(mShaderPrograms[1].mUniformIDs[0], mWorldToView);

//    mGPUTimers.recordSample();
//    mCameraObject.render();
//    SHADER(1)->release();


	mGPUTimers.recordSample(); 
    SHADER(1)->bind();
    SHADER(1)->setUniformValue(mShaderPrograms[1].mUniformIDs[0], mWorldToView);
    SHADER(1)->setUniformValue(mShaderPrograms[1].mUniformIDs[1], gridColor);
    SHADER(1)->setUniformValue(mShaderPrograms[1].mUniformIDs[2], backColor);

	mGPUTimers.recordSample(); 
	mGridObject.render();
    SHADER(1)->release();

	mGPUTimers.recordSample(); 

	checkInput();

	QVector<GLuint64> intervals = mGPUTimers.waitForIntervals();
	for (GLuint64 it : intervals)
		qDebug() << "  " << it*1e-6 << "ms/frame";
	QVector<GLuint64> samples = mGPUTimers.waitForSamples();
	qDebug() << "Total render time: " << (samples.back() - samples.front())*1e-6 << "ms/frame";

	qint64 elapsedMs = mCPUTimer.elapsed();
	qDebug() << "Total paintGL time: " << elapsedMs << "ms";
}


void SceneView::keyPressEvent(QKeyEvent *event) {
	mKeyboardMouseHandler.keyPressEvent(event);
	checkInput();
}

void SceneView::keyReleaseEvent(QKeyEvent *event) {
	mKeyboardMouseHandler.keyReleaseEvent(event);
	checkInput();
}

void SceneView::mousePressEvent(QMouseEvent *event) {
	mKeyboardMouseHandler.mousePressEvent(event);
	checkInput();
}

void SceneView::mouseReleaseEvent(QMouseEvent *event) {
	mKeyboardMouseHandler.mouseReleaseEvent(event);
	checkInput();
}

void SceneView::mouseMoveEvent(QMouseEvent * /*event*/) {
	checkInput();
}

void SceneView::wheelEvent(QWheelEvent *event) {
	mKeyboardMouseHandler.wheelEvent(event);
	checkInput();
}


void SceneView::checkInput() {
	if (mKeyboardMouseHandler.buttonDown(Qt::RightButton)) {
		if (mKeyboardMouseHandler.keyDown(Qt::Key_W) ||
			mKeyboardMouseHandler.keyDown(Qt::Key_A) ||
			mKeyboardMouseHandler.keyDown(Qt::Key_S) ||
			mKeyboardMouseHandler.keyDown(Qt::Key_D) ||
			mKeyboardMouseHandler.keyDown(Qt::Key_Q) ||
			mKeyboardMouseHandler.keyDown(Qt::Key_E))
		{
			mInputEventReceived = true;
			renderLater();
			return;
		}

		if (mKeyboardMouseHandler.mouseDownPos() != QCursor::pos()) {
			mInputEventReceived = true;
			renderLater();
			return;
		}
	}
	if (mKeyboardMouseHandler.wheelDelta() != 0) {
		mInputEventReceived = true;
		renderLater();
		return;
	}
}


void SceneView::processInput() {
	Q_ASSERT(mInputEventReceived);
	mInputEventReceived = false;

	if (mKeyboardMouseHandler.buttonDown(Qt::RightButton)) {

		QVector3D translation;
		if (mKeyboardMouseHandler.keyDown(Qt::Key_W)) 		translation += mCamera.forward();
		if (mKeyboardMouseHandler.keyDown(Qt::Key_S)) 		translation -= mCamera.forward();
		if (mKeyboardMouseHandler.keyDown(Qt::Key_A)) 		translation -= mCamera.right();
		if (mKeyboardMouseHandler.keyDown(Qt::Key_D)) 		translation += mCamera.right();
		if (mKeyboardMouseHandler.keyDown(Qt::Key_Q)) 		translation -= mCamera.up();
		if (mKeyboardMouseHandler.keyDown(Qt::Key_E)) 		translation += mCamera.up();

        float transSpeed = 0.4f;
		if (mKeyboardMouseHandler.keyDown(Qt::Key_Shift))
            transSpeed = 0.05f;
		mCamera.translate(transSpeed * translation);

		QPoint mouseDelta = mKeyboardMouseHandler.resetMouseDelta(QCursor::pos());
        static const float rotatationSpeed  = 0.3f;
		const QVector3D LocalUp(0.0f, 1.0f, 0.0f);
		mCamera.rotate(-rotatationSpeed * mouseDelta.x(), LocalUp);
		mCamera.rotate(-rotatationSpeed * mouseDelta.y(), mCamera.right());

		mKeyboardMouseHandler.clearWasPressedKeyStates();
	}
	int wheelDelta = mKeyboardMouseHandler.resetWheelDelta();
	if (wheelDelta != 0) {
        float transSpeed = 0.4f;
		if (mKeyboardMouseHandler.keyDown(Qt::Key_Shift))
            transSpeed = 0.05f;
		mCamera.translate(wheelDelta * transSpeed * mCamera.forward());
	}

	updateWorld2ViewMatrix();
}



void SceneView::updateWorld2ViewMatrix() {
	mWorldToView = mProjection * mCamera.toMatrix() * mTransform.toMatrix();
}
