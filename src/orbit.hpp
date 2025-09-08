#ifndef ORBIT_HPP
#define ORBIT_HPP

#include "object.hpp"

class Orbit: public Object
{
    public:
        int orbitLineSegments;

        float orbitSemiMajorAxis;
        float orbitEccentricity;
        float orbitInclination;
        float orbitArgumentOfPeriapsis;
        float orbitLongitudeOfAscendingNode;
        float orbitEpochOfPeriapsis;

        float orbitApoapsis;
        float orbitPeriapsis;
        float orbitTrueAnomoly;

        Orbit
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
        );

        void GenerateVertices() override;
        void GenerateIndices() override;
};

#endif