#define _USE_MATH_DEFINES
#include <cmath>

#include "satellite.hpp"

#include <iostream>

float wrapTwoPi(float angleRadians)
{
    float newAngle = std::fmod(angleRadians, 2.0f * M_PI);
    if (newAngle < 0)
        newAngle += 2.0f * M_PI;
    
    return newAngle;
}

Satellite::Satellite
(
    const char* name,
    float mass,
    float parentMass,
    float parentRadius
)
{
    satelliteName = name;
    satelliteMass = mass;
    satelliteParentMass = parentMass;
    satelliteParentRadius = parentRadius;
    mu = G * (satelliteParentMass + satelliteMass);
}

void Satellite::CalculateOrbitalParameters
(
    float longitude,
    float latitude,
    float azimuth,
    float altitude,
    float velocity,
    float distance,
    float FPA,
    float time
)
{
    // Current Distance
    satelliteDistance = altitude + satelliteParentRadius;
    satelliteVelocity = velocity;
    satelliteFlightPathAngle = FPA;

    // Eccentricity
    satelliteEccentricity = sqrt
    (
        ((satelliteDistance * (satelliteVelocity * satelliteVelocity)) / mu - 1) * ((satelliteDistance * (satelliteVelocity * satelliteVelocity)) / mu - 1) *
        cos(satelliteFlightPathAngle) * cos(satelliteFlightPathAngle) +
        sin(satelliteFlightPathAngle) * sin(satelliteFlightPathAngle)
    );
    
    // Semi-major Axis
    satelliteSemiMajorAxis = 1.0f / ((2.0f / satelliteDistance) - ((satelliteVelocity * satelliteVelocity) / mu));
    
    // apoapsis and periapsis
    satelliteApoapsisDistance = satelliteSemiMajorAxis * (1 + satelliteEccentricity);
    satelliteApoapsisAltitude = satelliteApoapsisDistance - satelliteParentRadius;

    satellitePeriapsisDistance = satelliteSemiMajorAxis * (1 - satelliteEccentricity);
    satellitePeriapsisAltitude = satellitePeriapsisDistance - satelliteParentRadius;

    // True Anomaly
    satelliteTrueAnomaly = atan2
    (
        (((satelliteDistance * (satelliteVelocity * satelliteVelocity)) / mu) * cos(satelliteFlightPathAngle) * sin(satelliteFlightPathAngle)), 
        (((satelliteDistance * (satelliteVelocity * satelliteVelocity)) / mu) * cos(satelliteFlightPathAngle) * cos(satelliteFlightPathAngle) - 1)
    );
    satelliteTrueAnomaly = wrapTwoPi(satelliteTrueAnomaly);

    // Eccentric Anomaly
    satelliteEccentricAnomaly = atan2
    (
        sqrt(1 - (satelliteEccentricity * satelliteEccentricity)) * sin(satelliteTrueAnomaly),
        satelliteEccentricity + cos(satelliteTrueAnomaly)
    );
    satelliteEccentricAnomaly = wrapTwoPi(satelliteEccentricAnomaly);

    // Mean Anomaly
    satelliteMeanAnomaly = satelliteEccentricAnomaly - satelliteEccentricity * sin(satelliteEccentricAnomaly);

    // Orbital Period
    satelliteOrbitalPeriod = 2.0f * M_PI * sqrt((satelliteSemiMajorAxis * satelliteSemiMajorAxis * satelliteSemiMajorAxis) / mu);

    // Mean Motion
    satelliteMeanMotion = 2.0f * M_PI / satelliteOrbitalPeriod;

    // Time of Periapsis Passage
    satelliteTimeOfPeriapsisPassage = time - satelliteMeanAnomaly / satelliteMeanMotion;

    satelliteInclination = acos(cos(latitude) * sin(azimuth));
    
    float l = atan2(tan(latitude), cos(azimuth));
    satelliteArgumentOfPeriapsis = l - satelliteTrueAnomaly;

    float deltaLongitude = atan2(sin(latitude) * sin(azimuth), cos(azimuth));
    satelliteLongitudeOfAscendingNode = longitude - deltaLongitude;
}

void Satellite::UpdatePosition(float time)
{
    const int iterations = 50;
    // update Mean Anomaly

    satelliteMeanAnomaly = satelliteMeanMotion * (time - satelliteTimeOfPeriapsisPassage);
    satelliteMeanAnomaly = wrapTwoPi(satelliteMeanAnomaly);
    
    // make first guess of E
    float M = satelliteMeanAnomaly;
    float e = satelliteEccentricity;
    float E = M + satelliteEccentricity * sin(M);

    // iterate using Halley's method
    for (int i = 0; i < iterations; i++)
    {
        float f_E = E - e * sin(E) - M; // f(E)
        float f1_E = 1 - e * cos(E); // f'(E)
        float f2_E = e * sin(E); // f"(E)

        E = E - (f_E * f1_E) / ((f1_E * f1_E) - (0.5f * f_E * f2_E));
    }

    // update Eccentric Anomaly
    satelliteEccentricAnomaly = E;

    // update True Anomaly
    satelliteTrueAnomaly = 2.0f * atan(sqrt((1 + satelliteEccentricity) / (1 - satelliteEccentricity)) * tan(satelliteEccentricAnomaly / 2.0f));

    // update distance, velocity and flight path angle
    satelliteDistance = (satelliteSemiMajorAxis * (1 - (satelliteEccentricity * satelliteEccentricity))) / (1 + satelliteEccentricity * cos(satelliteTrueAnomaly));
    satelliteVelocity = sqrt(mu * ((2.0f / satelliteDistance) - (1.0f / satelliteSemiMajorAxis)));
    satelliteFlightPathAngle = atan((satelliteEccentricity * sin(satelliteTrueAnomaly)) / (1 + satelliteEccentricity * cos(satelliteTrueAnomaly)));
    
    satelliteXPos = satelliteDistance * cos(satelliteTrueAnomaly);
    satelliteYPos = satelliteDistance * sin(satelliteTrueAnomaly);
    satelliteZPos = 0.0f;

    float w = satelliteArgumentOfPeriapsis;
    float i = satelliteInclination;
    float o = satelliteLongitudeOfAscendingNode;

    float x1 =  cos(o) * cos(w) - cos(i) * sin(o) * sin(w);
    float x2 = -cos(o) * sin(w) - cos(i) * cos(w) * sin(o);
    float x3 =  sin(o) * sin(i);
    float y1 =  cos(w) * sin(o) + cos(o) * cos(i) * sin(w);
    float y2 =  cos(o) * cos(i) * cos(w) - sin(o) * sin(w);
    float y3 = -cos(o) * sin(i);
    float z1 =  sin(i) * sin(w);
    float z2 =  cos(w) * sin(i);
    float z3 =  cos(i);

    float x = x1 * satelliteXPos + x2 * satelliteYPos + x3 * satelliteZPos;
    float y = y1 * satelliteXPos + y2 * satelliteYPos + y3 * satelliteZPos;
    float z = z1 * satelliteXPos + z2 * satelliteYPos + z3 * satelliteZPos;

    satelliteXPos = x;
    satelliteYPos = y;
    satelliteZPos = z;
}