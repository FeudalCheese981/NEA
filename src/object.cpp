#include "object.hpp"

Object::Object(glm::vec3 objectPos, GLenum drawType): objectMesh(vertices, indices)
{
    Object::vertices = {};
    Object::indices = {};
    Object::objectPos = objectPos;
    Object::objectModel = glm::mat4(1.0f);
    Object::drawType = drawType;
}

void Object::Place()
{
    objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);
}

void Object::Update(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
    Object::vertices = vertices;
    Object::indices = indices;
    objectMesh.Update(vertices, indices);
}

void Object::Draw(Shader& shader, Camera& camera, float thickness)
{   
    shader.Activate();
	
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.Matrix(shader, "camMatrix");

    glLineWidth(thickness);
    glPointSize(thickness);
    objectMesh.Draw(drawType);
    glLineWidth(1.0f);
    glPointSize(1.0f);
}

void Object::Delete()
{
    objectMesh.Delete();
}
