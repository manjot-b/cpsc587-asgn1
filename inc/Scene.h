#pragma once

#include "Entity.h"
#include <vector>

class Scene 
{
    typedef vector<Entity> EntityList;

    void addEntity(Entity entity);
    void addEntities(EntityList entities);
    void addCamera(Camera camera);
}