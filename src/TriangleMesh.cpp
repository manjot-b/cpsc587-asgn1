#include "TriangleMesh.h"

TriangleMesh::TriangleMesh(const char* objFilePath)
: Mesh(GL_TRIANGLES)
{
    loadFromObjFile(objFilePath);
}

void TriangleMesh::loadFromObjFile(const char* objFilePath)
{

}
