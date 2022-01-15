#ifndef CAMERAMESH_H
#define CAMERAMESH_H

#include <QOpenGLFunctions>

#include <QColor>
#include <QMatrix4x4>
#include <vector>

#include "Transform3D.h"
#include "Vertex.h"

class CameraMesh
{
public:
    CameraMesh(const QVector3D position = QVector3D(0.f, 0.f, 1.f));

    void setColor(QColor c) { mColors = std::vector<QColor>(1,c); }
    void setFaceColors(const std::vector<QColor> & c) { Q_ASSERT(c.size() == 6); mColors = c; }

    void transform(const QMatrix4x4 &transform);

    void copy2Buffer(Vertex * &vertexBuffer,
                    GLuint * & elementBuffer,
                    unsigned int & elementStartIndex) const;

    static const unsigned int VertexCount = 16;
    static const unsigned int IndexCOunt = 18;

private:
    std::vector<QVector3D> mVertices;
    std::vector<QColor> mColors;
};

#endif // CAMERAMESH_H
