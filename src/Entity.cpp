#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
#include <iostream>

using namespace std;

Entity::Entity(shared_ptr<Mesh> mesh, shared_ptr<Shader> shader, glm::mat4 modelMatrix) 
:   mesh_(mesh), shader_(shader), position_(glm::vec3(0,0,0)),
    translation_(glm::vec3(0,0,0)), scale_(1), color_(glm::vec4(1,0,0,1)), 
    drawPoints_(false)
{
    // shader_->use();
    int componentsPerAttribute = 3;
    vertexArray_ = make_unique<VertexArray>(&componentsPerAttribute, 1, mesh_->getData(), mesh_->verticesCount() * 3);
    calcBoundingBox();
}

void Entity::draw() const
{
    // assume that this entities shader is already in use
    glBindVertexArray(vertexArray_->getID());

    // debbuging. change colour of primitives
    // if (mesh_->getPrimitiveType() == GL_LINE_LOOP)
    // {
    //     shader_->setUniform1f("primitive", 0.0f);
    // }
    // else if (mesh_->getPrimitiveType() == GL_TRIANGLES)
    // {
    //     shader_->setUniform1f("primitive", 1.0f);
    // }

    shader_->setUniform4fv("uColor", color_);
    glDrawArrays(mesh_->getPrimitiveType(), 0, mesh_->verticesCount());

    // if (drawPoints_)
    // {
    //     shader_->setUniform1f("drawPoints", 1.0f);
    //     glPointSize(1);
    //     glDrawArrays(GL_POINTS, 0, mesh_->verticesCount());
    //     shader_->setUniform1f("drawPoints", 0);
    // }
    glBindVertexArray(0);
}

void Entity::calcBoundingBox()
{
    if (mesh_->verticesCount() < 1)
        return; // we need at least 1 vertex

    const float * vertices = mesh_->getData();

    float minX = vertices[0];
    float maxX = minX;
    float minY = vertices[1];
    float maxY = minY;
    float minZ = vertices[2];
    float maxZ = minZ;

    for (uint vert = 0; vert < mesh_->verticesCount() * 3; vert += 3)
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
    // cout << "VERTS " << mesh_->verticesCount() << endl;
    // cout << "MIN_X " << minX << " Max_X " << maxX << endl
    //     << "MIN_Y " << minY << " Max_Y " << maxY << endl
    //     << "MIN_Z " << minZ << " Max_Z " << maxZ << endl;
    boundingBox_.x = minX;
    boundingBox_.y = minY;
    boundingBox_.z = maxZ;
    boundingBox_.width = abs(maxX - minX);
    boundingBox_.height = abs(maxY - minY);
    boundingBox_.depth = abs(maxZ - minZ);
}

void Entity::updateModelMatrix()
{
    // modelMatrix_ = glm::mat4(1.0);
    glm::mat4 s = glm::scale(glm::mat4(1.0), glm::vec3(scale_, scale_, scale_));
    glm::mat4 t = glm::translate(glm::mat4(1.0), translation_);
    modelMatrix_ = t * s;
    // setModelMatrix(modelMatrix_);
}

Shader& Entity::getShader() { return *shader_; }
const shared_ptr<Mesh> Entity::getMesh() const { return mesh_; }
const VertexArray& Entity::getVertexArray() const { return *vertexArray_; }
const BoundingBox& Entity::getBoundingBox() const { return boundingBox_; }
const glm::mat4& Entity::getModelMatrix() const { return modelMatrix_; }
const glm::vec3& Entity::getPosition() const { return position_; }
float Entity::getScale() const { return scale_; }
const glm::vec4& Entity::getColor() const { return color_; }

void Entity::setModelMatrix(glm::mat4 modelMatrix) 
{ 
    modelMatrix_ = modelMatrix; 
    // shader_->use();
    // shader_->setUniformMatrix4fv("model", modelMatrix);
    // shader_->unuse();
}

void Entity::setPosition(glm::vec3 position) 
{
    translation_ = position; 
    position_ = position;
    updateModelMatrix();
}

void Entity::setScale(float s)
{
    scale_ = s;
    updateModelMatrix();
}

void Entity::setTranslation(glm::vec3 trans)
{
    translation_ += trans;
    updateModelMatrix();
}

void Entity::setColor(float r, float g, float b, float a)
{
    if (r > 1) r = 1;
    if (g > 1) g = 1;
    if (b > 1) b = 1;
    if (a > 1) a = 1;

    if (r < 0) r = 0;
    if (g < 0) g = 0;
    if (b < 0) b = 0;
    if (a < 0) a = 0;

    color_ = glm::vec4(r, g, b, a);
}

void Entity::pointsOn() { drawPoints_ = true; }
void Entity::pointsOff() { drawPoints_ = false; }