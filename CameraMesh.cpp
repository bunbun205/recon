#include "CameraMesh.h"

void plane2Buffer(Vertex * & vertexBuffer, GLuint * & elementBuffer, unsigned int & elementStartIndex,
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

CameraMesh::CameraMesh(const QVector3D position){

    mVertices.push_back(position);
    mVertices.push_back(position + QVector3D( 0.2,  0.15, -0.5));
    mVertices.push_back(position + QVector3D(-0.2,  0.15, -0.5));
    mVertices.push_back(position + QVector3D(-0.2, -0.15, -0.5));
    mVertices.push_back(position + QVector3D( 0.2, -0.15, -0.5));

    setColor(Qt::yellow);
}

void CameraMesh::transform(const QMatrix4x4 &transform){

    for(QVector3D &v : mVertices)
        v = transform * v;
}

void CameraMesh::copy2Buffer(Vertex *&vertexBuffer, GLuint *&elementBuffer, unsigned int &elementStartIndex) const{

    std::vector<QColor> cols;
    Q_ASSERT(!mColors.empty());
    // three ways to store vertex colors
    if (mColors.size() == 1) {
        cols = std::vector<QColor>(5, mColors[0]);
    }
    else {
        Q_ASSERT(mColors.size() == 5);
        cols = mColors;
    }

    plane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[0], cols[0]),
            Vertex(mVertices[1], cols[0]),
            Vertex(mVertices[2], cols[0]),
            Vertex(mVertices[0], cols[0])
        );

    plane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[0], cols[1]),
            Vertex(mVertices[2], cols[1]),
            Vertex(mVertices[3], cols[1]),
            Vertex(mVertices[0], cols[1])
        );

    plane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[0], cols[2]),
            Vertex(mVertices[3], cols[2]),
            Vertex(mVertices[4], cols[2]),
            Vertex(mVertices[0], cols[2])
        );

    plane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[0], cols[3]),
            Vertex(mVertices[4], cols[3]),
            Vertex(mVertices[2], cols[3]),
            Vertex(mVertices[0], cols[3])
        );

    plane2Buffer(vertexBuffer, elementBuffer, elementStartIndex,
            Vertex(mVertices[1], cols[4]),
            Vertex(mVertices[2], cols[4]),
            Vertex(mVertices[3], cols[4]),
            Vertex(mVertices[7], cols[4])
        );
}


