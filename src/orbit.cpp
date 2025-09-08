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
    glm::vec4 color,
    glm::vec3 pos
):
Object(color, pos, GL_LINE_STRIP)
{
    orbitLineSegments = lineSegments;
    orbitSemiMajorAxis = semiMajorAxis;
    orbitEccentricity = eccentricity;
    orbitInclination = inclination;
    orbitArgumentOfPeriapsis = argumentOfPeriapsis;
    orbitLongitudeOfAscendingNode = longitudeOfAscendingNode;
    orbitEpochOfPeriapsis = epochOfPeriapsis;
    orbitApoapsis = semiMajorAxis * (1 - eccentricity);
    orbitPeriapsis = semiMajorAxis * (1 * eccentricity);
    GenerateVertices();
    GenerateIndices();
    Update();
}

void Orbit::GenerateVertices()
{
    objectVertices = {};
    for (int i = 0; i <= orbitLineSegments; ++i)
    {
        float phi = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(orbitLineSegments);
        float r = (orbitSemiMajorAxis * (1 - (orbitEccentricity * orbitEccentricity))) / (1 + orbitEccentricity * cos(phi));
        float x = r * cos(phi);
        float y = r * sin(phi);
        float z = 0.0f;
        glm::vec3 pos = glm::vec3(x, y, z);
        pos = glm::rotate(pos, orbitArgumentOfPeriapsis, glm::vec3(0.0f, 0.0f, 1.0f));
        pos = glm::rotate(pos, orbitInclination, glm::vec3(1.0f, 0.0f, 0.0f));
        pos = glm::rotate(pos, orbitLongitudeOfAscendingNode, glm::vec3(0.0f, 0.0f, 1.0f));
        objectVertices.push_back(Vertex{ pos, objectColor, glm::vec3(0.0f, 0.0f, 0.0f) });
    }
}

void Orbit::GenerateIndices()
{
    objectIndices = {};
    for (int i = 0; i <= orbitLineSegments; ++i)
    {
        objectIndices.push_back(i);
    }
}