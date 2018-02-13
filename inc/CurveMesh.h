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

        const std::vector<glm::vec3>& getUValues() const;
        float getLength() const;
        float getDeltaS() const;

    private:
        std::vector<glm::vec3> uValues_;
        float length_;
        float deltaS_;

        float calcArcLength();
        void arcLengthParameterization(float length);

    protected:
        void createVerticesGrouped();
};