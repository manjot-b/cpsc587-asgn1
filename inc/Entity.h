#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include <glm/glm.hpp>
#include <memory>

struct BoundingBox      
{
    float x = 0;
    float y = 0;
    float z = 0; 
    float width = 1.0f;
    float height = 1.0f;
    float depth = 1.0f;
};

class Entity
{
    private:
        glm::mat4 modelMatrix_;
        std::unique_ptr<Mesh> mesh_;
        std::unique_ptr<Shader> shader_;
        std::unique_ptr<VertexArray> vertexArray_;

        bool drawPoints_;

        BoundingBox boundingBox_; 
        void calcBoundingBox();

    public:
        Entity();
        Entity(Mesh* Mesh, Shader* shader, glm::mat4 modelMatrix = glm::mat4(1.0f));

        void setModelMatrix(glm::mat4 modelMatrix);
        void setShader(Shader* shader);
        void setVertexArray(VertexArray vertexArray);
        
        Shader& getShader();
        const Mesh& getMesh() const;
        const VertexArray& getVertexArray() const;
        void getModelMatrix() const;
        const BoundingBox& getBoundingBox() const;

        void draw() const;
        void pointsOn();
        void pointsOff();
};