#pragma once
#include "Mesh.h"

class CurveMesh : public Mesh
{
    public:
        CurveMesh(const float* data, unsigned int size);
        CurveMesh(const char *objFilePath);

        virtual void loadFromObjFile(const char *objFilePath);
        void smoothCurve();
};