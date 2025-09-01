#include "light.hpp"

Light::Light
(
    float radius,
    int segments,
    int rings,
    glm::vec4 color,
    glm::vec3 pos
):
Sphere(radius, segments, rings, color, pos, SPHERE_COLOR_DEFAULT) {}

void Light::Place(Shader& shader)
{
    objectModel = glm::mat4(1.0f);
    objectModel = glm::translate(objectModel, objectPos);
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), color.x, color.y, color.z, color.w);
}

void Light::SendShaderLightInfo(Shader& shader)
{
    shader.Activate();
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), color.x, color.y, color.z, color.w);
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), objectPos.x, objectPos.y, objectPos.z);
}