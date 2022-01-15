QT       += core gui widgets

TARGET = recon
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS GL_DEBUG

contains( OPTIONS, sanitize_checks ) {

	CONFIG(debug, debug|release) {
		CONFIG += sanitizer
		CONFIG += sanitize_address
		CONFIG += sanitize_undefined
	}

	linux-g++ | linux-g++-64 | macx {
		QMAKE_CXXFLAGS_DEBUG   *= -fsanitize=address -fno-omit-frame-pointer
	}
}

CONFIG += c++11

CONFIG += link_pkgconfig
PKGCONFIG += opencv4

win32 {
	LIBS += -lopengl32
}

SOURCES += \
		BoxMesh.cpp \
		BoxObject.cpp \
		CameraMesh.cpp \
		CameraObject.cpp \
		GridObject.cpp \
		KeyboardMouseHandler.cpp \
		OpenGLException.cpp \
		OpenGLWindow.cpp \
		SceneView.cpp \
		ShaderProgram.cpp \
		TestDialog.cpp \
		Transform3D.cpp \
		main.cpp

HEADERS += \
	BoxMesh.h \
	BoxObject.h \
	Camera.h \
	CameraMesh.h \
	CameraObject.h \
	DebugApplication.h \
	GridObject.h \
	KeyboardMouseHandler.h \
	OpenGLException.h \
	OpenGLWindow.h \
	SceneView.h \
	ShaderProgram.h \
	TestDialog.h \
	Transform3D.h \
	Vertex.h \
	recon.h

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
