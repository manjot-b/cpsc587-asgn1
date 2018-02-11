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
#include "TriangleMesh.h"
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
	// TRACK
	shared_ptr<Mesh> mesh = make_shared<CurveMesh>("rsc/roller_coaster.obj");
	static_cast<CurveMesh*>(mesh.get())->smooth(10);

	shared_ptr<Shader> shader = make_shared<Shader>("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader->link();

	shared_ptr<Entity> curve = make_shared<Entity>(mesh, shader);
	curve->pointsOn();

	glm::mat4 model(1.0f);
	BoundingBox box = curve->getBoundingBox();
	float scale = 1 / max(box.width, max(box.height, box.depth));
	float xTrans = -box.x - (box.width / 2);
	float yTrans = -box.y;// - (box.height / 2);
	float zTrans = -box.z + (box.depth / 2);
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::translate(model, glm::vec3(xTrans, yTrans, zTrans)); 
	curve->setModelMatrix(model);

	scene.addEntity(curve);

	// GROUND
	float p[] = {
				box.x, box.y, box.z,
				box.x, box.y, box.z - box.depth,
				box.x + box.width, box.y, box.z,

				box.x + box.width, box.y, box.z,
				box.x + box.width, box.y, box.z - box.depth,
				box.x, box.y, box.z - box.depth
				};
	mesh = make_shared<TriangleMesh>(p, sizeof(p) / sizeof(float));
	shared_ptr<Entity> ground = make_shared<Entity>(mesh, shader);
	
	box = ground->getBoundingBox();
	scale = 1 / max(box.width, max(box.height, box.depth));
	xTrans = -box.x - (box.width / 2);
	yTrans = -box.y;// - (box.height / 2);
	zTrans = -box.z + (box.depth / 2);
	model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(scale, scale, scale));
	model = glm::translate(model, glm::vec3(xTrans, yTrans, zTrans));
	ground->setModelMatrix(model);

	scene.addEntity(ground);

	glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 0.4f, 0.8f),	// camera position
			glm::vec3(0, 0.2f, 0),				// where camera is lookin
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
		update();
		render();
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

void Engine::update()
{

}

void Engine::render()
{
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.drawScene();

	glfwSwapBuffers(window_.get());
	glfwPollEvents();
}