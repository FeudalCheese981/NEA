#ifndef ORBIT_LINE_HPP
#define ORBIT_LINE_HPP

#include "object.hpp"

class OrbitLine: public Object
{
    public:
        int orbitLineSegments;

        float orbitSemiMajorAxis;
        float orbitEccentricity;
        float orbitInclination;
        float orbitArgumentOfPeriapsis;
        float orbitLongitudeOfAscendingNode;

        OrbitLine
        (
            int lineSegments,
            float semiMajorAxis, 
            float eccentricity, 
            float inclination, 
            float argumentOfPeriapsis, 
            float longitudeOfAscendingNode,
            glm::vec4 color,
            glm::vec3 pos,
            GLenum drawType
        );

        void GenerateVertices() override;
        void GenerateIndices() override;
};

#endif