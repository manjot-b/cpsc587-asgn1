#include <glad/glad.h>
#include "Entity.h"

Entity::Entity(Mesh* mesh, Shader shader, glm::mat4 modelMatrix) :
    mesh_(mesh)
{
    int componentsPerVertex = 3;
    vertexArray_ = VertexArray(&componentsPerVertex, 1, mesh_->getData(), mesh_->verticesCount());
}


void Entity::draw() const
{
    // assume that this entities shader is already in use
    glBindVertexArray(vertexArray_.getID());
	glDrawArrays(mesh_->getPrimitiveType(), 0, mesh_->verticesCount());	// 3 vertices per triangle
	glBindVertexArray(0);

}

const Shader& Entity::getShader() const { return shader_; }