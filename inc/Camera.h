#pragma once

#include <glm/glm.hpp>

class Camera
{
    public:
        Camera();
        Camera(glm::mat4 view, glm::mat4 perspective);

        void lookAt(glm::vec3 postion, glm::vec3 center, glm::vec3 up);
        
};