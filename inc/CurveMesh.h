#pragma once
#include "Mesh.h"

class CurveMesh : public Mesh
{
    public:
        CurveMesh(const char *objFilePath);

        virtual void loadFromObjFile(const char *objFilePath);
};