#include "Scene.h"
#include <algorithm>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

Scene::Scene()
{
    glm::mat4 view = glm::lookAt(
			glm::vec3(0.0f, 2.0f, 3.0f),	// camera position
			glm::vec3(0, 0, 0),				// where camera is lookin
			glm::vec3(0, 1, 0)				// up vector
            );
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
    camera_ = Camera(view, projection);
}

Scene::Scene(Camera camera) :
camera_(camera)
{

}

void Scene::addEntity(EntityPtr entity)
{
    EntityList& entityList = entityMap_[entity->getShader().getID()];       
    auto it = find(entityList.begin(), entityList.end(), entity); 
    if ( it == entityList.end() )   // add new entity
    {
        entityList.push_back(entity);
    }
}

void Scene::drawScene()
{
    for (auto mapIt = entityMap_.begin(); mapIt != entityMap_.end(); mapIt++)
    {
        const auto& entityList = mapIt->second;
        if (entityList.size() > 0)
        {
            // cout << "(in Scene.cpp) SHADER ID " << entityList[0]->getShader().getID() << endl;
            // draw all the entities with the same shader
            entityList[0]->getShader().use();

            for (const auto& entity : entityList)
            {
                // cout << "TEST" << endl;
                entity->draw();
            }

            entityList[0]->getShader().unuse();
        }
    }
}

void Scene::addCamera(Camera camera)
{
    camera_ = camera;

    // update all shaders to use view/proj matrix
    for (auto mapIt = entityMap_.begin(); mapIt != entityMap_.end(); mapIt++)
    {
        const auto& entityList = mapIt->second;
        if (!entityList.empty())
        {
            Shader& shader = entityList[0]->getShader();
            shader.setUniformMatrix4fv("view", camera_.getViewMatrix());
            shader.setUniformMatrix4fv("projection", camera_.getProjectionViewMatrix());

        }
    }
}