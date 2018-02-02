#pragma once

#include <glad/glad.h>
#include "Entity.h"
#include "Shader.h"
#include "Camera.h"
#include <vector>
#include <map>
#include <memory>

class Scene 
{
    typedef std::shared_ptr<Entity> EntityPtr;
    typedef std::vector< EntityPtr > EntityList;
    typedef std::map<GLuint, EntityList > EntityMap;    // key is entitie's shader id 

    public:
        Scene();
        Scene(Camera camera);

        void addEntity(EntityPtr entity);
        void addEntities(EntityList entities);
        void addCamera(Camera camera);
        void drawScene();

    private:
        EntityMap entityMap_;
        Camera camera_;
};