#define _USE_MATH_DEFINES
#include <cmath>

#include "satellite.hpp"

Satellite::Satellite(const char* name, float mass)
{
    satelliteName = name;
    satelliteMass = mass;
}