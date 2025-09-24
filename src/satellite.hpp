#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <cmath>
#include <string>
#include <memory>

#include "orbitLine.hpp"
#include "sphere.hpp"

const float G = 6.67e-11f;

float wrapTwoPi(float angleRadians);

class Satellite
{
    public:
        bool isAlive = true;

        std::string satelliteName;
        float satelliteMass;
        float satelliteParentMass;
        float satelliteParentRadius;
        float mu; // standard gravitational parameter

        // six orbital parameters
        float satelliteEccentricity;
        float satelliteSemiMajorAxis;
        float satelliteInclination;
        float satelliteLongitudeOfAscendingNode;
        float satelliteArgumentOfPeriapsis;
        float satelliteTimeOfPeriapsisPassage;

        // other orbital information
        float satelliteApoapsisDistance;
        float satellitePeriapsisDistance;
        float satelliteApoapsisAltitude;
        float satellitePeriapsisAltitude;
        float satelliteOrbitalPeriod;
        float satelliteMeanMotion;
        float satelliteMeanAnomaly;
        float satelliteEccentricAnomaly;

        // satellite position, velocity and fpa
        float satelliteTrueAnomaly;
        float satelliteDistance;
        float satelliteAltitude;
        float satelliteVelocity;
        float satelliteFlightPathAngle;

        float satelliteXPos;
        float satelliteYPos;
        float satelliteZPos;
        
        Sphere satelliteObject;
        OrbitLine satelliteOrbit;

        Satellite
        (
            const char* name,
            float mass,
            float parentMass,
            float parentRadius,
            float radius,
            glm::vec4 lineColor
        );

        void CalculateOrbitalParameters
        (
            float longitude,
            float latitude,
            float azimuth,
            float altitude,
            float velocity,
            float distance,
            float FPA,
            float time
        );
        
        void UpdatePosition(float time);
        void Draw(Shader& lineShader, Shader& objectShader, Camera& Camera);
};

#endif