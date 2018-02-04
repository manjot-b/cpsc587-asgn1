#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <map>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Engine.h"
#include "CurveMesh.h"
#include "Shader.h"
#include "VertexArray.h"

using namespace std;

Engine::Engine(int argc, const char *argv[])
{
	if (!initWindow())
	{
		cerr << "Failed to  initilize GLFW" << endl;
		windowInitialized_ = false;
	}
	windowInitialized_ = true;
	initScene();
}

bool Engine::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window_ = GLFWwindowPtr(glfwCreateWindow(800, 800, "Roller Coaster", NULL, NULL));

	if (!window_)
	{
		cout << "Failed to create GLFW window. TERMINATING" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window_.get());

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return false;
	}

	glViewport(0, 0, 800, 800);
	glfwSetWindowUserPointer(window_.get(), this);

	glfwSetFramebufferSizeCallback(window_.get(),
								   [](GLFWwindow *window, int width, int height) {
									   glViewport(0, 0, width, height);
								   });
	glEnable(GL_DEPTH_TEST);

	return true; // if we made it here then success
}

void Engine::initScene()
{
	CurveMesh *mesh = new CurveMesh("rsc/roller_coaster.obj");
	// float p[] = {
	// 			-0.5f, 0, 0.2f,
	// 			0.5f, 0.5f, 0,
	// 			0.5f, 0, 0,
	// 			0, 0.5f, 0
	// 			};
	// CurveMesh* mesh = new CurveMesh(p, sizeof(p) / sizeof(float));
	Shader *shader = new Shader("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader->link();

	shared_ptr<Entity> curve = make_shared<Entity>(mesh, shader);
	curve->pointsOn();

	glm::mat4 model(1.0f);
	const BoundingBox &box = curve->getBoundingBox();
	float scale = 1 / max(box.width, max(box.height, box.depth));
	float xTrans = -box.x - (box.width / 2);
	float yTrans = -box.y - (box.height / 2);
	float zTrans = -box.z + (box.depth / 2);
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::translate(model, glm::vec3(xTrans, yTrans, zTrans)); 
	curve->setModelMatrix(model);

	scene.addEntity(curve);

	glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 2.0f, 3.0f),	// camera position
			glm::vec3(0, 0, 0),				// where camera is lookin
			glm::vec3(0, 1, 0)				// up vector
            );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    Camera camera = Camera(view, projection);
	scene.addCamera(camera);
}

int Engine::run()
{
	if (!windowInitialized_)
		return -1;

	while (!glfwWindowShouldClose(window_.get()))
	{
		processInput();

		glClearColor(0.3f, 1.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		scene.drawScene();

		glfwSwapBuffers(window_.get());
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void Engine::processInput()
{
	if (glfwGetKey(window_.get(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window_.get(), true);
	}
}