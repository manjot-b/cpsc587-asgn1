#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const char* objFilePath)
: Mesh(GL_TRIANGLES)
{
    loadFromObjFile(objFilePath);
}

TriangleMesh::TriangleMesh(const float* data, unsigned int size)
: Mesh(GL_TRIANGLES, data, size)
{

}

void TriangleMesh::loadFromObjFile(const char* objFilePath)
{

}
