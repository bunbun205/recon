#ifndef VERTEX_H
#define VERTEX_H

#include <QVector3D>
#include <QColor>
#include <QVector2D>

struct Vertex {
	Vertex() {}
	Vertex(const QVector3D & coords, const QColor & col) :
		x(float(coords.x())),
		y(float(coords.y())),
		z(float(coords.z())),
		r(float(col.redF())),
		g(float(col.greenF())),
		b(float(col.blueF()))
	{
	}

	float x,y,z;
	float r,g,b;
};

struct textureVertex {

    QVector3D postion;
    QVector2D texCoord;
};

#endif // VERTEX_H
