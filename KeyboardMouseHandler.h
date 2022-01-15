#ifndef KeyboardMouseHandlerH
#define KeyboardMouseHandlerH

#include <QPoint>
#include <vector>

class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

class KeyboardMouseHandler {
public:
	KeyboardMouseHandler();
	virtual ~KeyboardMouseHandler();

	void addRecognizedKey(Qt::Key k);
	void clearRecognizedKeys();

	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

	bool pressKey(Qt::Key k);
	bool releaseKey(Qt::Key k);
	bool pressButton(Qt::MouseButton btn, QPoint currentPos);
	bool releaseButton(Qt::MouseButton btn);

	bool keyDown(Qt::Key k) const;
	bool buttonDown(Qt::MouseButton btn) const;

	QPoint mouseDownPos() const { return mMouseDownPos; }

	QPoint resetMouseDelta(const QPoint currentPos);

	int wheelDelta() const;

	int resetWheelDelta();

	void clearWasPressedKeyStates();

private:
	enum KeyStates {
		StateNotPressed,
		StateHeld,
		StateWasPressed
	};

	std::vector<Qt::Key>	mKeys;
	std::vector<KeyStates>	mKeystates;

	KeyStates				mLeftButtonDown;
	KeyStates				mMiddleButtonDown;
	KeyStates				mRightButtonDown;

	QPoint					mMouseDownPos;

	int						mWheelDelta;
};

#endif // KeyboardMouseHandlerH
