#pragma once

#include "VertexArray.h"
#include <vector>

class Mesh
{
      protected:
        VertexArray vertArray_;
        std::vector<float> vertices_;     // every 3 float represent (x, y, z) of a vertex
        GLenum primitiveType_;

    public:
        Mesh() {};
        Mesh(const float* data, unsigned int size)
        {
            for (unsigned int i = 0; i < size; i++)
            {
                vertices_.push_back(data[i]);
            }    
        }

        virtual void loadFromObjFile(const char* objFilepath) = 0;
        const float * getData() const { return vertices_.data(); }
        size_t verticesCount() const { return vertices_.size(); }
        GLenum getPrimitiveType() const { return primitiveType_; }
};