#include "object.hpp"

Object::Object(glm::vec4 color, glm::vec3 pos, GLenum drawType): objectMesh(objectVertices, objectIndices)
{
    objectColor = color;
    objectPos = pos;
    objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);
    objectDrawType = drawType;
}

void Object::Place()
{
    objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);
}

void Object::Update()
{
    objectMesh.Update(objectVertices, objectIndices);
}

void Object::Draw(Shader& shader, Camera& camera, float thickness)
{   
    shader.Activate();
	
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
	glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
	camera.Matrix(shader, "camMatrix");

    glLineWidth(thickness);
    glPointSize(thickness);
    objectMesh.Draw(objectDrawType);
    glLineWidth(1.0f);
    glPointSize(1.0f);
}

void Object::Delete()
{
    objectMesh.Delete();
}
