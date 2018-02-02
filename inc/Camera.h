#pragma once

#include <glm/glm.hpp>

class Camera
{
    public:
        Camera() {}
        Camera(glm::mat4 view, glm::mat4 perspective);

        void setViewMatrix(glm::vec3 postion, glm::vec3 center, glm::vec3 up);
        void setProjectionMatrix(float fovy, float aspect, float zNear, float zFar);
        void translate(glm::vec3 translation);

        glm::mat4 getViewProjectionMatrix() const;

    private:
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        
};