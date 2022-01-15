#include "KeyboardMouseHandler.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QWheelEvent>


KeyboardMouseHandler::KeyboardMouseHandler() :
	mLeftButtonDown(StateNotPressed),
	mMiddleButtonDown(StateNotPressed),
	mRightButtonDown(StateNotPressed),
	mWheelDelta(0)
{
}


KeyboardMouseHandler::~KeyboardMouseHandler() {
}



void KeyboardMouseHandler::keyPressEvent(QKeyEvent *event) {
	if (event->isAutoRepeat()) {
		event->ignore();
	}
	else {
		pressKey(static_cast<Qt::Key>((event->key())));
	}
}


void KeyboardMouseHandler::keyReleaseEvent(QKeyEvent *event) {
	if (event->isAutoRepeat())	{
		event->ignore();
	}
	else {
		releaseKey(static_cast<Qt::Key>((event->key())));
	}
}


void KeyboardMouseHandler::mousePressEvent(QMouseEvent *event) {
	pressButton(static_cast<Qt::MouseButton>(event->button()), event->globalPos());
}


void KeyboardMouseHandler::mouseReleaseEvent(QMouseEvent *event) {
	releaseButton(static_cast<Qt::MouseButton>(event->button()));
}


void KeyboardMouseHandler::wheelEvent(QWheelEvent *event) {
	QPoint numPixels = event->pixelDelta();
	QPoint numDegrees = event->angleDelta() / 8;

	if (!numPixels.isNull()) {
		mWheelDelta += numPixels.y();
	} else if (!numDegrees.isNull()) {
		QPoint numSteps = numDegrees / 15;
		mWheelDelta += numSteps.y();
	}

	event->accept();
}


void KeyboardMouseHandler::addRecognizedKey(Qt::Key k) {
	if (std::find(mKeys.begin(), mKeys.end(), k) != mKeys.end())
		return;
	mKeys.push_back(k);
	mKeystates.push_back(StateNotPressed);
}


void KeyboardMouseHandler::clearRecognizedKeys() {
	mKeys.clear();
	mKeystates.clear();
}


void KeyboardMouseHandler::clearWasPressedKeyStates() {
	mLeftButtonDown = (mLeftButtonDown == StateWasPressed) ? StateNotPressed  : mLeftButtonDown;
	mMiddleButtonDown = (mMiddleButtonDown == StateWasPressed) ? StateNotPressed  : mMiddleButtonDown;
	mRightButtonDown = (mRightButtonDown == StateWasPressed) ? StateNotPressed  : mRightButtonDown;

	for (unsigned int i=0; i<mKeystates.size(); ++i)
		mKeystates[i] = static_cast<KeyStates>(mKeystates[i] & 1);
}



bool KeyboardMouseHandler::pressKey(Qt::Key k) {
	for (unsigned int i=0; i<mKeys.size(); ++i) {
		if (mKeys[i] == k) {
			mKeystates[i] = StateHeld;
			return true;
		}
	}
	return false;
}


bool KeyboardMouseHandler::releaseKey(Qt::Key k) {
	for (unsigned int i=0; i<mKeys.size(); ++i) {
		if (mKeys[i] == k) {
			mKeystates[i] = StateWasPressed;
			return true;
		}
	}
	return false;
}


bool KeyboardMouseHandler::pressButton(Qt::MouseButton btn, QPoint currentPos) {
	switch (btn) {
		case Qt::LeftButton		: mLeftButtonDown = StateHeld; break;
		case Qt::MiddleButton	: mMiddleButtonDown = StateHeld; break;
		case Qt::RightButton	: mRightButtonDown = StateHeld; break;
		default: return false;
	}
	mMouseDownPos = currentPos;
	return true;
}


bool KeyboardMouseHandler::releaseButton(Qt::MouseButton btn) {
	switch (btn) {
		case Qt::LeftButton		: mLeftButtonDown = StateWasPressed; break;
		case Qt::MiddleButton	: mMiddleButtonDown = StateWasPressed; break;
		case Qt::RightButton	: mRightButtonDown = StateWasPressed; break;
		default: return false;
	}
	return true;
}


QPoint KeyboardMouseHandler::resetMouseDelta(const QPoint currentPos) {
	QPoint dist = currentPos - mMouseDownPos;
	mMouseDownPos = currentPos;
	return dist;
}


int KeyboardMouseHandler::wheelDelta() const {
	return mWheelDelta;
}


int KeyboardMouseHandler::resetWheelDelta() {
	int wd = mWheelDelta;
	mWheelDelta = 0;
	return wd;
}


bool KeyboardMouseHandler::keyDown(Qt::Key k) const {
	for (unsigned int i=0; i<mKeys.size(); ++i) {
		if (mKeys[i] == k)
			return mKeystates[i] == StateHeld;
	}
	return false;
}


bool KeyboardMouseHandler::buttonDown(Qt::MouseButton btn) const {
	switch (btn) {
		case Qt::LeftButton		: return mLeftButtonDown == StateHeld;
		case Qt::MiddleButton	: return mMiddleButtonDown == StateHeld;
		case Qt::RightButton	: return mRightButtonDown == StateHeld;
		default: return false;
	}
}
