#ifndef CAMERA_H
#define CAMERA_H

#include "Transform3D.h"

class Camera : public Transform3D {
public:

	QVector3D forward() const {
		const QVector3D LocalForward(0.0f, 0.0f, -1.0f);
		return mRotation.rotatedVector(LocalForward);
	}

	QVector3D up() const {
		const QVector3D LocalUp(0.0f, 1.0f, 0.0f);
		return mRotation.rotatedVector(LocalUp);
	}

	QVector3D right() const {
		const QVector3D LocalRight(1.0f, 0.0f, 0.0f);
		return mRotation.rotatedVector(LocalRight);
	}

	const QMatrix4x4 & toMatrix() const {
		if (mDirty) {
			mDirty = false;
			mWorld.setToIdentity();
			mWorld.rotate(mRotation.conjugated());
			mWorld.translate(-mTranslation);
		}
		return mWorld;
	}

};

#endif // CAMERA_H
