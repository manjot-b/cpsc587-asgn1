#pragma once

#include "IMesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glm/glm.hpp>

class Entity
{
    public:
        Entity();
        Entity(VertexArray vertexArray, Shader shader, glm::mat4 modelMatrix);

        void setModelMatrix(glm::mat4 modelMatrix);
        void setShader(Shader shader);
        void setVertexArray(VertexArray vertexArray);
        
        void getShader() const;
        void getModelMatrix() const;

        void draw() const;


    private:
        glm::mat4 modelMatrix;
        Shader shader;
        VertexArray vertexArray;
};