#pragma once

#include "Mesh.h"
#include <vector>
#include <glm/glm.hpp>

class TriangleMesh : public Mesh
{
    public:
        TriangleMesh(const char* objFilePath);
        TriangleMesh(const float* data, unsigned int size);

        virtual void loadFromObjFile(const char* objFilePath);
    private:
        std::vector<float> normals_;
};