#include "Scene.h"
#include <algorithm>

Scene::Scene()
{

}

Scene::Scene(Camera camera) :
camera_(camera)
{

}

void Scene::addEntity(EntityPtr entity)
{
    EntityList entityList = entityMap_[entity->getShader().getID()];
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
            // draw all the entities with the same shader
            entityList[0]->getShader().use();

            for (const auto& entity : entityList)
            {
                entity->draw();
            }

            entityList[0]->getShader().unuse();
        }
    }
}