#include "TestDialog.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

#include "SceneView.h"

TestDialog::TestDialog() :
#ifdef Q_OS_WIN
	QDialog(nullptr, Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint)
#else
	QDialog(nullptr, Qt::Window)
#endif
{

	QSurfaceFormat format;
	format.setRenderableType(QSurfaceFormat::OpenGL);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setVersion(3,3);
	format.setSamples(4);
	format.setDepthBufferSize(8);
#ifdef GL_DEBUG_
	format.setOption(QSurfaceFormat::DebugContext);
#endif // GL_DEBUG

	mSceneview = new SceneView;
	mSceneview->setFormat(format);

	QWidget *container = QWidget::createWindowContainer(mSceneview);
	container->setFocusPolicy(Qt::TabFocus);
    container->setMinimumSize(QSize(1280,720));

	QVBoxLayout * vlay = new QVBoxLayout;
	vlay->setMargin(0);
	vlay->setSpacing(0);
	vlay->addWidget(container);

	QHBoxLayout * hlay = new QHBoxLayout;
	hlay->setMargin(0);

	QLabel * navigationInfo = new QLabel(this);
	navigationInfo->setWordWrap(true);
	navigationInfo->setText("Hold right mouse button for free mouse look and to navigate "
							"with keys WASDQE. Hold shift to slow down. Use scroll-wheel to move quickly forward and backward.");
	hlay->addWidget(navigationInfo);

	QPushButton * closeBtn = new QPushButton(tr("Close"), this);
	connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

	hlay->addWidget(closeBtn);
	hlay->setStretch(0,1);

	vlay->addLayout(hlay);

	setLayout(vlay);

	resize(700,450);

	container->setFocus();
}
