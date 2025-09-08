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

void Light::Draw(Shader& shader, Camera& camera, float thickness)
{
    shader.Activate();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(objectModel));
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), sphereColor.x, sphereColor.y, sphereColor.z, sphereColor.w);
    camera.Matrix(shader, "camMatrix");

    objectMesh.Draw(objectDrawType);
}

void Light::SendLightInfoToShader(Shader& shader)
{
    shader.Activate();
    glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), sphereColor.x, sphereColor.y, sphereColor.z, sphereColor.w);
    glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), objectPos.x, objectPos.y, objectPos.z);
}