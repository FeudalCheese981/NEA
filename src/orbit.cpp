#define _USE_MATH_DEFINES
#include <cmath>

#include "orbit.hpp"

glm::vec3 eulerAngleTransformation(float x, float y, float z, float w, float i, float o)
{
    float x1 = cos(o) * cos(w) - cos(i) * sin(o) * sin(w);
    float x2 = - cos(o) * sin(w) - cos(i) * cos(w) * sin(o);
    float x3 = sin(o) * sin(i);
    float y1 = cos(w) * sin(o) + cos(o) * cos(i) * sin(w);
    float y2 = cos(o) * cos(i) * cos(w) - sin(o) * sin(w);
    float y3 = -cos(o) *  sin(i);
    float z1 = sin(i) * sin(w);
    float z2 = cos(w) * sin(i);
    float z3 = cos(i);

    float new_x = x1 * x + x2 * y + x3 * z;
    float new_y = y1 * x + y2 * y + y3 * z;
    float new_z = z1 * x + z2 * y + z3 * z;

    return glm::vec3(new_x, new_y, new_z);
}

Orbit::Orbit(float semiMajorAxis, float eccentricity, float inclination, float argumentOfPeriapsis, float longitudeOfAscendingNode, float epochOfPeriapsis, glm::vec4 lineColor)
{
    Orbit::semiMajorAxis = semiMajorAxis;
    Orbit::eccentricity = eccentricity;
    Orbit::inclination = inclination;
    Orbit::argumentOfPeriapsis = argumentOfPeriapsis;
    Orbit::longitudeOfAscendingNode = longitudeOfAscendingNode;
    Orbit::epochOfPeriapsis = epochOfPeriapsis;
    Orbit::lineColor = lineColor;

    apoapsis = semiMajorAxis * (1 - eccentricity);
    periapsis = semiMajorAxis * (1 * eccentricity);
}

void Orbit::GenerateOrbit(int segments)
{
    for (int j = 0; j <= segments; ++j)
    {
        float phi = 2.0f * M_PI * (float)j / segments;
        float r = (semiMajorAxis * (1 - (eccentricity * eccentricity))) / (1 + eccentricity * cos(phi));

        float x = r * cos(phi);
        float y = r * sin(phi);
        float z = 0.0f;

        glm::vec3 pos = eulerAngleTransformation(x, y, z, argumentOfPeriapsis, inclination, longitudeOfAscendingNode);

        vertices.push_back(Vertex{ pos, lineColor, glm::vec3(0.0f, 0.0f, 0.0f) });
        indices.push_back(j);
    }
}