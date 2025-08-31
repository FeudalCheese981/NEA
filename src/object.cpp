#include "object.hpp"

Object::Object(): objectMesh(vertices, indices)
{
    Object::vertices = vertices;
    Object::indices = indices;
}

void Object::Place(Shader& shader)
{
    objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
}

void Object::Update(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    Object::vertices = vertices;
    Object::indices = indices;
    objectMesh.Update(vertices, indices);
}

void Object::Draw(Shader& shader, Camera& camera, float thickness)
{   
    glLineWidth(thickness);
    glPointSize(thickness);
    objectMesh.Draw(shader, camera, drawType);
    glLineWidth(1.0f);
    glPointSize(1.0f);
}

void Object::Delete()
{
    objectMesh.Delete();
}
