#pragma once
#include "Mesh.h"
#include <glm/glm.hpp>

class CurveMesh : public Mesh
{
    public:
        CurveMesh(const float* data, unsigned int size);
        CurveMesh(const char *objFilePath);

        virtual void loadFromObjFile(const char *objFilePath);
        void smooth(unsigned int iterations);

    private:
        std::vector<glm::vec3> uValues_;

        void arcLengthParameterization();
};