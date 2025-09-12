#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <cmath>
#include <string>

const float G = 6.67e-11;

class Satellite
{
    public:
        const char* satelliteName;
        float satelliteMass;
        float mu; // standard gravitational parameter

        // six orbital parameters
        float satelliteEccentricity;
        float satelliteSemiMajorAxis;
        float satelliteInclination;
        float satelliteLongitudeOfAscendingNode;
        float satelliteArgumentOfPeriapsis;
        float satelliteTimeOfPeriapsisPassage;

        // other orbital information
        float satelliteApoapsis;
        float satellitePeriapsis;
        float satelliteOrbitalPeriod;
        float satelliteMeanMotion;
        float satelliteMeanAnomaly;
        float satelliteTrueAnomaly;
        float satelliteEccentricAnomaly;

        Satellite(const char* name, float mass);

        void NewPrimaryBody(float mass);
        void CalculateOrbitalParameters
        (
            float launchLongitude,
            float launchLatitude,
            float launchAzimuth,
            float mecoAltitude,
            float mecoVelocity,
            float mecoDistance,
            float mecoFPA
        );
        
        void CalculateAnomaly();
        void updatePosition();

};

#endif