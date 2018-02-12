#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <memory>
#include "Scene.h"

class Engine
{
    public:
        Engine(int argc, const char* argv[]);
        int run();

    private:
        struct DestroyglfwWin
        {
            void operator()(GLFWwindow* ptr)
            {
                glfwDestroyWindow(ptr);
            }
        };

        typedef std::unique_ptr<GLFWwindow, DestroyglfwWin> GLFWwindowPtr;
        GLFWwindowPtr window_;
        bool windowInitialized_;
        
        Scene scene_;
        std::shared_ptr<Entity> track_;
        std::shared_ptr<Entity> cart_;
        std::shared_ptr<Entity> ground_;

        bool initWindow();
        void initScene();
        void processInput();
        uint update(uint time);
        void render();        
};