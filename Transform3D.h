#ifndef TRANSFORM3D_H
#define TRANSFORM3D_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Transform3D {
public:
	Transform3D();

	void translate(const QVector3D &dt);
	void translate(float dx, float dy, float dz);
	void scale(const QVector3D &ds);
	void scale(float dx, float dy, float dz);
	void scale(float factor);
	void rotate(const QQuaternion &dr);
	void rotate(float angle, const QVector3D &axis);
	void rotate(float angle, float ax, float ay, float az);
	void grow(const QVector3D &ds);
	void grow(float dx, float dy, float dz);
	void grow(float factor);

	void setTranslation(const QVector3D &t);
	void setTranslation(float x, float y, float z);
	void setScale(const QVector3D &s);
	void setScale(float x, float y, float z);
	void setScale(float k);
	void setRotation(const QQuaternion &r);
	void setRotation(float angle, const QVector3D &axis);
	void setRotation(float angle, float ax, float ay, float az);

	const QVector3D& translation() const;
	const QVector3D& scale() const;
	const QQuaternion& rotation() const;
	const QMatrix4x4& toMatrix() const;

protected:
	QVector3D mTranslation;
	QVector3D mScale;
	QQuaternion mRotation;
	mutable QMatrix4x4 mWorld;
	mutable bool mDirty;

#ifndef QT_NO_DATASTREAM
	friend QDataStream &operator<<(QDataStream &out, const Transform3D &transform);
	friend QDataStream &operator>>(QDataStream &in, Transform3D &transform);
#endif
};

Q_DECLARE_TYPEINFO(Transform3D, Q_MOVABLE_TYPE);

inline Transform3D::Transform3D() : mScale(1.0f, 1.0f, 1.0f), mDirty(true) {}

inline void Transform3D::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void Transform3D::scale(float dx, float dy,float dz) { scale(QVector3D(dx, dy, dz)); }
inline void Transform3D::scale(float factor) { scale(QVector3D(factor, factor, factor)); }
inline void Transform3D::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Transform3D::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }
inline void Transform3D::grow(float dx, float dy, float dz) { grow(QVector3D(dx, dy, dz)); }
inline void Transform3D::grow(float factor) { grow(QVector3D(factor, factor, factor)); }

inline void Transform3D::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void Transform3D::setScale(float x, float y, float z) { setScale(QVector3D(x, y, z)); }
inline void Transform3D::setScale(float k) { setScale(QVector3D(k, k, k)); }
inline void Transform3D::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Transform3D::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

inline const QVector3D& Transform3D::translation() const { return mTranslation; }
inline const QVector3D& Transform3D::scale() const { return mScale; }
inline const QQuaternion& Transform3D::rotation() const { return mRotation; }

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const Transform3D &transform);
#endif

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const Transform3D &transform);
QDataStream &operator>>(QDataStream &in, Transform3D &transform);
#endif

#endif // TRANSFORM3D_H
