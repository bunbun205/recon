#ifndef TESTDIALOG_H
#define TESTDIALOG_H

#include <QDialog>

class SceneView;

class TestDialog : public QDialog {
	Q_OBJECT
public:
	TestDialog();

private:
	SceneView * mSceneview;
};

#endif // TESTDIALOG_H
