#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <map>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

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
	static_cast<CurveMesh*>(mesh.get())->smooth(7);

	shared_ptr<Shader> shader = make_shared<Shader>("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader->link();

	track_ = make_shared<Entity>(mesh, shader);
	// track_->pointsOn();
	track_->setColor(1, 0.8, 0);

	glm::mat4 model(1.0f);
	BoundingBox box = track_->getBoundingBox();
	float scale = 1 / max(box.width, max(box.height, box.depth));
	// float xTrans = -box.x - (box.width / 2);
	// float yTrans = -box.y;// - (box.height / 2);
	// float zTrans = -box.z + (box.depth / 2);
	track_->setScale(scale);
	// track_->setTranslation(glm::vec3(xTrans, yTrans, zTrans)); 

	scene_.addEntity(track_);

	// GROUND
	float groundCoords[] = {
		box.x, box.y, box.z,
		box.x, box.y, box.z - box.depth,
		box.x + box.width, box.y, box.z,

		box.x + box.width, box.y, box.z,
		box.x + box.width, box.y, box.z - box.depth,
		box.x, box.y, box.z - box.depth
		};
	mesh = make_shared<TriangleMesh>(groundCoords, sizeof(groundCoords) / sizeof(float));
	ground_ = make_shared<Entity>(mesh, shader);
	ground_->setColor(0.1, 0.2, 0.4);

	box = ground_->getBoundingBox();
	scale = 1 / max(box.width, max(box.height, box.depth));
	// xTrans = -box.x - (box.width / 2);
	// yTrans = -box.y;// - (box.height / 2);
	// zTrans = -box.z + (box.depth / 2);
	ground_->setScale(scale);
	// ground_->setTranslation(glm::vec3(xTrans, yTrans, zTrans)); 

	scene_.addEntity(ground_);

	// CART
	float cartCoords[] = {
		0, 0, -0.2f,
		0, 1, -0.2f,
		1, 0, -0.2f
	};
	mesh = make_shared<TriangleMesh>(cartCoords, sizeof(cartCoords) / sizeof(float));
	cart_ = make_shared<Entity>(mesh, shader);
	cart_->setColor(0.4, 0.1, 0.4);

	scale = track_->getScale();
	box = cart_->getBoundingBox();
	// scale = 1 / (2.1f * max(box.width, max(box.height, box.depth)));
	// xTrans = -box.x - (box.width / 2);
	// yTrans = -box.y - (box.height / 2);
	// zTrans = -box.z + (box.depth / 2);
	// model = glm::mat4(1.0f);
	cart_->setScale(scale);
	// cart_->setTranslation(glm::vec3(xTrans, yTrans, zTrans));
	cart_->setPosition( glm::vec3(0, 0, 0) );

	scene_.addEntity(cart_);


	glm::mat4 view = glm::lookAt(
			glm::vec3(0.05f, 0.3f, 0.8f),	// camera position
			glm::vec3(0.05f, 0.2f, 0),		// where camera is lookin
			glm::vec3(0, 1, 0)				// up vector
            );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    Camera camera = Camera(view, projection);
	scene_.addCamera(camera);

	TriangleMesh tr("rsc/cart.obj");
}

int Engine::run()
{
	if (!windowInitialized_)
		return -1;

	unsigned long long time = 0;
	while (!glfwWindowShouldClose(window_.get()))
	{
		processInput();
		time = update(time);
		render();
		// time++;
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

uint Engine::update(uint time)
{
	uint liftState = 0;
	uint freeFallState = 194;
	uint decelerationState = 1962;
	float speed = 0;
	float deltaT = 0.006;
	float deltaS = 0;

	const CurveMesh* trackMesh = static_cast<CurveMesh*>(track_->getMesh().get());
	const vector<glm::vec3>& points = trackMesh->getUValues();
	
	if (time >= points.size()) 
		time %= points.size();
	
	if (time >= liftState && time <= freeFallState)
	{
		speed = 0.4f;
	}
	else if (time > freeFallState && time <= decelerationState)
	{
		glm::vec4 maxHeightPos = glm::vec4(points[freeFallState], 1);
		maxHeightPos = track_->getModelMatrix() * maxHeightPos;
		float maxHeight = maxHeightPos.y;

		glm::vec4 currentCartWorldPos = glm::vec4(points[time], 1.0f);
		currentCartWorldPos = track_->getModelMatrix() * currentCartWorldPos;
		float currentHeight = currentCartWorldPos.y;

		speed = sqrt( 2 * 9.81 * (maxHeight - currentHeight));
	}
	else	// deceleration  
	{
		float decelerationLength = trackMesh->getDeltaS() * (points.size() - decelerationState);
		float remainingLength = trackMesh->getDeltaS() * (points.size() - time);

		speed = 0.8 * (remainingLength / decelerationLength);
	}
	
	deltaS = deltaT * speed;
	float remaining = deltaS / trackMesh->getDeltaS();
	float intPart;
	remaining = modf(remaining, &intPart);	// left with just fractional part
	time += intPart + 1;

	glm::vec4 newCartWorldPos = glm::vec4(points[time], 1.0f);
	// float a = (1 - (remaining / trackMesh->getDeltaS()));
	// float b = remaining / trackMesh->getDeltaS();

	// glm::vec4 newCartWorldPos = a * glm::vec4(points[time], 1.0f) + 
	// 							b * glm::vec4(points[time+1], 1.0f);

	newCartWorldPos = track_->getModelMatrix() * newCartWorldPos;
	cart_->setPosition( glm::vec3(newCartWorldPos) );


	// cout << "TIME " << time << " " << points[time].x << " " 
	// 	<< points[time].y << " " << points[time].z << endl
	// 	<< deltaS << " " << trackMesh->getDeltaS() << endl;
	return time;
}

void Engine::render()
{
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene_.drawScene();

	glfwSwapBuffers(window_.get());
	glfwPollEvents();
}