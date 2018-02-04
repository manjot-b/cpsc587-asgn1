#include <glad/glad.h>
#include "Entity.h"
#include <iostream>

using namespace std;

Entity::Entity(Mesh *mesh, Shader *shader, glm::mat4 modelMatrix) 
: mesh_(mesh), shader_(shader), drawPoints_(false)
{
    // shader_->use();
    int componentsPerAttribute = 3;
    vertexArray_ = make_unique<VertexArray>(&componentsPerAttribute, 1, mesh_->getData(), mesh_->verticesCount() * 3);
}

void Entity::draw() const
{
    // assume that this entities shader is already in use
    glBindVertexArray(vertexArray_->getID());
    glDrawArrays(mesh_->getPrimitiveType(), 0, mesh_->verticesCount());

    if (drawPoints_)
    {
        shader_->setUniform1f("drawPoints", 1.0f);
        glPointSize(10);
        glDrawArrays(GL_POINTS, 0, mesh_->verticesCount());
        shader_->setUniform1f("drawPoints", 0);
    }
    glBindVertexArray(0);
}

void Entity::calcBoundingBox()
{
    if (mesh_->verticesCount() < 3)
        return; // we need at least 1 vertex

    const float * vertices = mesh_->getData();

    float minX = vertices[0];
    float maxX = minX;
    float minY = vertices[1];
    float maxY = minY;
    float minZ = vertices[2];
    float maxZ = minZ;

    for (uint vert = 0; vert < mesh_->verticesCount(); vert += 3)
    {
        uint x = vert;
        uint y = x+1;
        uint z = y+1;

        minX = min(minX, vertices[x]);
        maxX = max(maxX, vertices[x]);

        minY = min(minY, vertices[y]);
        maxY = max(maxY, vertices[y]);

        minZ = min(minZ, vertices[z]);
        maxZ = max(maxZ, vertices[z]);
    }

    //float length = max( abs(maxX - minX), max( abs(maxY - minY), abs(maxZ - minZ) ) );
    boundingBox_.x = minX;
    boundingBox_.y = minY;
    boundingBox_.z = maxZ;
    boundingBox_.width = abs(maxX - minX);
    boundingBox_.height = abs(maxY - minY);
    boundingBox_.depth = abs(maxZ - minZ);
}

Shader& Entity::getShader() { return *shader_; }
const Mesh& Entity::getMesh() const { return *mesh_; }
const VertexArray& Entity::getVertexArray() const { return *vertexArray_; }
const BoundingBox& Entity::getBoundingBox() const { return boundingBox_; }

void Entity::setModelMatrix(glm::mat4 modelMatrix) 
{ 
    modelMatrix_ = modelMatrix; 
    shader_->setUniformMatrix4fv("model", modelMatrix);
}

void Entity::pointsOn() { drawPoints_ = true; }
void Entity::pointsOff() { drawPoints_ = false; }