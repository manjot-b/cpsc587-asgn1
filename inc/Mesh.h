#pragma once

#include "VertexArray.h"
#include <vector>
#include <glm/glm.hpp>

class Mesh
{
      protected:
        VertexArray vertArray_;
        std::vector<float> vertices_;     // every 3 float represent (x, y, z) of a vertex
        std::vector<glm::vec3> verticesGrouped_;
        GLenum primitiveType_;

        void createVerticesGrouped();
    public:
        Mesh(GLenum primitiveType) 
        : primitiveType_(primitiveType)  {}
        
        Mesh(GLenum primitiveType, const float* data, unsigned int size)
        : primitiveType_(primitiveType)
        {
            for (unsigned int i = 0; i < size; i++)
            {
                vertices_.push_back(data[i]);
            }
        }

        virtual void loadFromObjFile(const char* objFilepath) = 0;
        const float * getData() const { return vertices_.data(); }
        const std::vector<glm::vec3>& getVertices() const { return verticesGrouped_; }
        size_t verticesCount() const { return vertices_.size() / 3; }   // 3 coords per vertex
        GLenum getPrimitiveType() const { return primitiveType_; }
};