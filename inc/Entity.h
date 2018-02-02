#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glm/glm.hpp>
#include <memory>

class Entity
{
    public:
        Entity();
        Entity(Mesh* Mesh, Shader shader, glm::mat4 modelMatrix);

        void setModelMatrix(glm::mat4 modelMatrix);
        void setShader(Shader shader);
        void setVertexArray(VertexArray vertexArray);
        
        const Shader& getShader() const;
        void getModelMatrix() const;

        void draw() const;


    private:
        glm::mat4 modelMatrix_;
        Shader shader_;
        VertexArray vertexArray_;
        std::unique_ptr<Mesh> mesh_;
};