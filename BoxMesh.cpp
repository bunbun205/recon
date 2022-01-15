#include "BoxMesh.h"

void copyPlane2Buffer(Vertex * & vertexBuffer, GLuint * & elementBuffer, unsigned int & elementStartIndex,
					  const Vertex & a, const Vertex & b, const Vertex & c, const Vertex & d);


BoxMesh::BoxMesh(float width, float height, float depth, QColor boxColor) {

    mVertices.push_back(QVector3D(-0.5f*width, -0.5f*height,  0.5f*depth)); // a = 0
    mVertices.push_back(QVector3D( 0.5f*width, -0.5f*height,  0.5f*depth)); // b = 1
    mVertices.push_back(QVector3D( 0.5f*width,  0.5f*height,  0.5f*depth)); // c = 2
    mVertices.push_back(QVector3D(-0.5f*width,  0.5f*height,  0.5f*depth)); // d = 3

    mVertices.push_back(QVector3D(-0.5f*width, -0.5f*height, -0.5f*depth)); // e = 4
    mVertices.push_back(QVector3D( 0.5f*width, -0.5f*height, -0.5f*depth)); // f = 5
    mVertices.push_back(QVector3D( 0.5f*width,  0.5f*height, -0.5f*depth)); // g = 6
    mVertices.push_back(QVector3D(-0.5f*width,  0.5f*height, -0.5f*depth)); // h = 7

	setColor(boxColor);
}


void BoxMesh::transform(const QMatrix4x4 & transform) {
    for (QVector3D & v : mVertices)
		v = transform*v;
}


void BoxMesh::copy2Buffer(Vertex *& vertexBuffer, GLuint *& elementBuffer, unsigned int & elementStartIndex) const {
	std::vector<QColor> cols;
	Q_ASSERT(!mColors.empty());
	if (mColors.size() == 1) {
		cols = std::vector<QColor>(6, mColors[0]);
	}
	else {
		Q_ASSERT(mColors.size() == 6);
		cols = mColors;
	}

	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[0], cols[0]),
            Vertex(mVertices[1], cols[0]),
            Vertex(mVertices[2], cols[0]),
            Vertex(mVertices[3], cols[0])
		);

	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[1], cols[1]),
            Vertex(mVertices[5], cols[1]),
            Vertex(mVertices[6], cols[1]),
            Vertex(mVertices[2], cols[1])
		);

	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[5], cols[2]),
            Vertex(mVertices[4], cols[2]),
            Vertex(mVertices[7], cols[2]),
            Vertex(mVertices[6], cols[2])
		);

	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[4], cols[3]),
            Vertex(mVertices[0], cols[3]),
            Vertex(mVertices[3], cols[3]),
            Vertex(mVertices[7], cols[3])
		);

	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[4], cols[4]),
            Vertex(mVertices[5], cols[4]),
            Vertex(mVertices[1], cols[4]),
            Vertex(mVertices[0], cols[4])
		);

	copyPlane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[3], cols[5]),
            Vertex(mVertices[2], cols[5]),
            Vertex(mVertices[6], cols[5]),
            Vertex(mVertices[7], cols[5])
		);
}


void copyPlane2Buffer(Vertex * & vertexBuffer, GLuint * & elementBuffer, unsigned int & elementStartIndex,
					  const Vertex & a, const Vertex & b, const Vertex & c, const Vertex & d)
{


	vertexBuffer[0] = a;
	vertexBuffer[1] = b;
	vertexBuffer[2] = c;
	vertexBuffer[3] = d;

	if (a.y < c.y) {
		vertexBuffer[0].r *= 0.5;
		vertexBuffer[1].r *= 0.5;
		vertexBuffer[0].g *= 0.5;
		vertexBuffer[1].g *= 0.5;
		vertexBuffer[0].b *= 0.5;
		vertexBuffer[1].b *= 0.5;
	}

	vertexBuffer += 4;


	elementBuffer[0] = elementStartIndex;
	elementBuffer[1] = elementStartIndex+1;
	elementBuffer[2] = elementStartIndex+3;
	elementBuffer[3] = elementStartIndex+1;
	elementBuffer[4] = elementStartIndex+2;
	elementBuffer[5] = elementStartIndex+3;

	elementBuffer += 6;
	elementStartIndex += 4;
}




