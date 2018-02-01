#pragma once

#include "VertexArray.h"
#include <vector>

class Mesh
{
      protected:
        VertexArray vertArray;
        std::vector<float> vertices;     // every 3 float represent (x, y, z) of a vertex

    public:
        Mesh() {};

        virtual void loadFromObjFile(const char* objFilepath) = 0;
        const float * getData() const { return vertices.data(); }
        size_t verticesCount() const { return vertices.size(); }
};