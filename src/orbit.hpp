#ifndef ORBIT_HPP
#define ORBIT_HPP

#include "object.hpp"

class Orbit: public Object
{
    public:
        int lineSegments;

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

        Orbit
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
        );

        void GenerateVertices() override;
        void GenerateIndices() override;
};

#endif