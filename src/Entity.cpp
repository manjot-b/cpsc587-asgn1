#include <glad/glad.h>
#include "Entity.h"
#include <iostream>

using namespace std;

Entity::Entity(Mesh* mesh, Shader* shader, glm::mat4 modelMatrix) :
    mesh_(mesh), shader_(shader)
{
    int componentsPerVertex = 2;
    vertexArray_ = make_unique<VertexArray>(&componentsPerVertex, 1, mesh_->getData(), mesh_->verticesCount());
}


void Entity::draw() const
{
    // assume that this entities shader is already in use
    glBindVertexArray(vertexArray_->getID());
    // cout << "VA ID" << mesh_->verticesCount();
	glDrawArrays(mesh_->getPrimitiveType(), 0, mesh_->verticesCount());	// 3 vertices per triangle
	glBindVertexArray(0);

}

const Shader& Entity::getShader() const { return *shader_; }