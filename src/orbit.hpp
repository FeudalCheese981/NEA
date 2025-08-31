#ifndef ORBIT_HPP
#define ORBIT_HPP

#include "mesh.hpp"

glm::vec3 eulerAngleTransformation(float x, float y, float z, float w, float i, float o);

class Orbit
{
    public:
        float semiMajorAxis;
        float eccentricity;
        float inclination;
        float argumentOfPeriapsis;
        float longitudeOfAscendingNode;
        float epochOfPeriapsis;

        float apoapsis;
        float periapsis;
        float trueAnomoly;

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        glm::vec4 lineColor;

        Orbit(float semiMajorAxis, float eccentricity, float inclination, float argumentOfPeriapsis, float longitudeOfAscendingNode, float epochOfPeriapsis, glm::vec4 lineColor);

        void GenerateOrbit(int segments);
};

#endif