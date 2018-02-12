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
        std::shared_ptr<Mesh> mesh_;
        std::shared_ptr<Shader> shader_;
        std::unique_ptr<VertexArray> vertexArray_;
        glm::vec3 position_;
        glm::vec3 translation_;
        float scale_;
        glm::vec4 color_;

        bool drawPoints_;

        BoundingBox boundingBox_; 
        void calcBoundingBox();
        void updateModelMatrix();

    public:
        Entity();
        Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Shader> shader, glm::mat4 modelMatrix = glm::mat4(1.0f));

        void setModelMatrix(glm::mat4 modelMatrix);
        void setShader(Shader* shader);
        void setVertexArray(VertexArray vertexArray);
        void setPosition(glm::vec3 poition);
        void setScale(float s);
        void setTranslation(glm::vec3 trans);
        void setColor(float r, float g, float b, float a = 1);

        Shader& getShader();
        const std::shared_ptr<Mesh> getMesh() const;
        const VertexArray& getVertexArray() const;
        const glm::mat4& getModelMatrix() const;
        const BoundingBox& getBoundingBox() const;
        const glm::vec3& getPosition() const;
        float getScale() const;
        const glm::vec4& getColor() const;

        void draw() const;
        void pointsOn();
        void pointsOff();
};