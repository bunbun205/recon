#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QMatrix4x4>
#include <QOpenGLTimeMonitor>
#include <QElapsedTimer>

#include "OpenGLWindow.h"
#include "ShaderProgram.h"
#include "KeyboardMouseHandler.h"
#include "GridObject.h"
#include "BoxObject.h"
#include "Camera.h"
//#include "CameraObject.h"

class SceneView : public OpenGLWindow {
public:
	SceneView();
	virtual ~SceneView() override;

protected:
	void initializeGL() override;
	void resizeGL(int width, int height) override;
	void paintGL() override;

	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;

private:
	void checkInput();
	void processInput();
	void updateWorld2ViewMatrix();
	bool mInputEventReceived;

	KeyboardMouseHandler mKeyboardMouseHandler;

	QMatrix4x4 mProjection;
	Transform3D mTransform;
	Camera mCamera;
	QMatrix4x4 mWorldToView;

	QList<ShaderProgram> mShaderPrograms;

	BoxObject mBoxObject;
	GridObject mGridObject;
//    CameraObject mCameraObject;

	QOpenGLTimeMonitor mGPUTimers;
	QElapsedTimer mCPUTimer;
};

#endif // SCENEVIEW_H
