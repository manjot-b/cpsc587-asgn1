#pragma once

#include "Mesh.h"

class TriangleMesh : public Mesh
{
    public:
        TriangleMesh(const char* objFilePath);
        TriangleMesh(const float* data, unsigned int size);

        virtual void loadFromObjFile(const char* objFilePath);
};