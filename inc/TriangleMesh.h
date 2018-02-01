#pragma once

#include "Mesh.h"

class TriangleMesh : public Mesh
{
    public:
        TriangleMesh(const char* objFilePath);

        virtual void loadFromObjFile(const char* objFilePath);
};