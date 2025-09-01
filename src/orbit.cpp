#define _USE_MATH_DEFINES
#include <cmath>

#include "orbit.hpp"

Orbit::Orbit
(
    int lineSegments,
    float semiMajorAxis,
    float eccentricity,
    float inclination,
    float argumentOfPeriapsis,
    float longitudeOfAscendingNode,
    float epochOfPeriapsis,
    glm::vec4 lineColor,
    glm::vec3 pos
):
Object(pos, GL_LINE_STRIP)
{
    Orbit::lineSegments = lineSegments;
    Orbit::semiMajorAxis = semiMajorAxis;
    Orbit::eccentricity = eccentricity;
    Orbit::inclination = inclination;
    Orbit::argumentOfPeriapsis = argumentOfPeriapsis;
    Orbit::longitudeOfAscendingNode = longitudeOfAscendingNode;
    Orbit::epochOfPeriapsis = epochOfPeriapsis;
    Orbit::lineColor = lineColor;
    apoapsis = semiMajorAxis * (1 - eccentricity);
    periapsis = semiMajorAxis * (1 * eccentricity);
    GenerateVertices();
    GenerateIndices();
    Update(vertices, indices);
}

void Orbit::GenerateVertices()
{
    for (int i = 0; i <= lineSegments; ++i)
    {
        float phi = 2.0f * M_PI * (float)i / lineSegments;
        float r = (semiMajorAxis * (1 - (eccentricity * eccentricity))) / (1 + eccentricity * cos(phi));
        float x = r * cos(phi);
        float y = r * sin(phi);
        float z = 0.0f;
        glm::vec3 pos = glm::vec3(x, y, z);
        pos = glm::rotate(pos, argumentOfPeriapsis, glm::vec3(0.0f, 0.0f, 1.0f));
        pos = glm::rotate(pos, inclination, glm::vec3(1.0f, 0.0f, 0.0f));
        pos = glm::rotate(pos, longitudeOfAscendingNode, glm::vec3(0.0f, 0.0f, 1.0f));
        vertices.push_back(Vertex{ pos, lineColor, glm::vec3(0.0f, 0.0f, 0.0f) });
    }
}

void Orbit::GenerateIndices()
{
    for (int i = 0; i <= lineSegments; ++i)
    {
        indices.push_back(i);
    }
}