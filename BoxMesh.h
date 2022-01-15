#ifndef BOXMESH_H
#define BOXMESH_H

#include <QtGui/QOpenGLFunctions>

#include <QColor>
#include <QMatrix4x4>
#include <vector>

#include "Transform3D.h"
#include "Vertex.h"

class BoxMesh {
public:
    BoxMesh(float width = 0.5, float height = 0.5, float depth = 0.5, QColor boxColor = Qt::cyan);

	void setColor(QColor c) { mColors = std::vector<QColor>(1,c); }

	void setFaceColors(const std::vector<QColor> & c) { Q_ASSERT(c.size() == 6); mColors = c; }

	void transform(const QMatrix4x4 &transform);

	void copy2Buffer(Vertex * & vertexBuffer,
					GLuint * & elementBuffer,
					unsigned int & elementStartIndex) const;

	static const unsigned int VertexCount = 6*4; 
	static const unsigned int IndexCount = 6*2*3;

private:
    	std::vector<QVector3D>	mVertices;
	std::vector<QColor>	mColors;
};

#endif // BOXMESH_H
