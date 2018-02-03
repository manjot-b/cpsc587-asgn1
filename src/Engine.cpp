#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "Engine.h"
#include "CurveMesh.h"
#include "Shader.h"
#include "VertexArray.h"

using namespace std;

Engine::Engine(int argc, const char* argv[])
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
		[] (GLFWwindow *window, int width, int height) {
			glViewport(0, 0, width, height);
		}
	);
	glEnable(GL_DEPTH_TEST);
	
	return true;	// if we made it here then success
}

void Engine::initScene()
{
	// CurveMesh* mesh = new CurveMesh("rsc/roller_coaster.obj");
	
		vector<float> data =
{
	// Triangle 1
	// positions
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.0f, 0.0f,

	// Triangle 2
	0.0f, 0.0f,
	-0.5f, 0.5f,
	0.5f, 0.5f
};

	CurveMesh* mesh = new CurveMesh(data.data(), data.size());
	Shader* shader = new Shader("rsc/vertex.glsl", "rsc/fragment.glsl");
	// cout << "(in Engine.cpp) SHADER ID " << shader->getID() << endl;


	shader->link();
	shared_ptr<Entity> curve = make_shared<Entity>(mesh, shader);

	// for (int i = 0; i < mesh->verticesCount(); i++)
	// 	cout << mesh->getData()[i] << endl;

	scene.addEntity(curve);
}

int Engine::run()
{
	if ( !windowInitialized_ ) return -1;
/*
	vector<float> verticies =
{
	// Triangle 1
	// positions		// colou
	-0.5f, -0.5f, 	1.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 	0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 	0.0f, 0.0f, 1.0f,

	// Triangle 2
	0.0f, 0.0f, 	0.0f, 0.0f, 1.0f,
	-0.5f, 0.5f,	1.0f, 0.0f, 0.0f,
	0.5f, 0.5f, 	0.0f, 1.0f, 0.0f
};

	Shader shader("rsc/vertex.glsl", "rsc/fragment.glsl");
	shader.link();

	GLuint VAO, VBO;
	glGenBuffers(1, &VBO); // gen 1 buffer and store id in VBO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), verticies.data(), GL_STATIC_DRAW);

	const int COLOR_COMPONENTS = 3;
	const int VERTEX_COMPONENTS = 2;
	const int COMPONENTS_PER_VERTEX = COLOR_COMPONENTS + VERTEX_COMPONENTS;
	const int STRIDE = COMPONENTS_PER_VERTEX * sizeof(float);
	//position attributes
	glVertexAttribPointer(0, VERTEX_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
	glEnableVertexAttribArray(0);
	//color attributes
	glVertexAttribPointer(1, COLOR_COMPONENTS, GL_FLOAT, GL_FALSE, STRIDE, (void*)(VERTEX_COMPONENTS * sizeof(float)));
	glEnableVertexAttribArray(1);

	// unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);*/

		vector<float> data =
{
	// Triangle 1
	// positions
	-0.5f, -0.5f,
	0.5f, -0.5f,
	0.0f, 0.0f,

	// Triangle 2
	0.0f, 0.0f,
	-0.5f, 0.5f,
	0.5f, 0.5f
};
	unique_ptr<Shader> shader(new Shader("rsc/vertex.glsl", "rsc/fragment.glsl"));
	shader->link();
	int comps = 2;
	unique_ptr<VertexArray> va(new VertexArray(&comps, 1, data.data(), data.size()));

    while( !glfwWindowShouldClose(window_.get()) )
    {
		processInput();

        glClearColor(0.3f, 1.0f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// scene.drawScene();
		glUseProgram(shader->getID());
		glBindVertexArray(va->getID());
		glDrawArrays(GL_TRIANGLES, 0, data.size());



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