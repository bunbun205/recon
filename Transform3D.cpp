#include "Transform3D.h"
#include <QDebug>

void Transform3D::translate(const QVector3D &dt)
{
	mDirty = true;
	mTranslation += dt;
}

void Transform3D::scale(const QVector3D &ds)
{
	mDirty = true;
	mScale *= ds;
}

void Transform3D::rotate(const QQuaternion &dr)
{
	mDirty = true;
	mRotation = dr * mRotation;
}

void Transform3D::grow(const QVector3D &ds)
{
	mDirty = true;
	mScale += ds;
}

void Transform3D::setTranslation(const QVector3D &t)
{
	mDirty = true;
	mTranslation = t;
}

void Transform3D::setScale(const QVector3D &s)
{
	mDirty = true;
	mScale = s;
}

void Transform3D::setRotation(const QQuaternion &r)
{
	mDirty = true;
	mRotation = r;
}

const QMatrix4x4 &Transform3D::toMatrix() const {
	if (mDirty) {
		mDirty = false;
		mWorld.setToIdentity();
		mWorld.translate(mTranslation);
		mWorld.rotate(mRotation);
		mWorld.scale(mScale);
	}
	return mWorld;
}

QDebug operator<<(QDebug dbg, const Transform3D &transform)
{
	dbg << "Transform3D\n{\n";
	dbg << "Position: <" << transform.translation().x() << ", " << transform.translation().y() << ", " << transform.translation().z() << ">\n";
	dbg << "Scale: <" << transform.scale().x() << ", " << transform.scale().y() << ", " << transform.scale().z() << ">\n";
	dbg << "Rotation: <" << transform.rotation().x() << ", " << transform.rotation().y() << ", " << transform.rotation().z() << " | " << transform.rotation().scalar() << ">\n}";
	return dbg;
}

QDataStream &operator<<(QDataStream &out, const Transform3D &transform)
{
	out << transform.mTranslation;
	out << transform.mScale;
	out << transform.mRotation;
	return out;
}

QDataStream &operator>>(QDataStream &in, Transform3D &transform)
{
	in >> transform.mTranslation;
	in >> transform.mScale;
	in >> transform.mRotation;
	transform.mDirty = true;
	return in;
}
