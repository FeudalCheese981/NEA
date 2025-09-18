#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <cmath>
#include <string>

const float G = 6.67e-11f;

float wrapTwoPi(float angleRadians);

class Satellite
{
    public:
        const char* satelliteName;
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
        float satelliteVelocity;
        float satelliteFlightPathAngle;

        float satelliteXPos;
        float satelliteYPos;
        float satelliteZPos;
        

        Satellite
        (
            const char* name,
            float mass,
            float parentMass,
            float parentRadius
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
};

#endif