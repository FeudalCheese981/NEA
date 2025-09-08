#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "object.hpp"

enum ColorMode { SPHERE_COLOR_DEFAULT, SPHERE_COLOR_RGB };

class Sphere: public Object
{
    public:
        float sphereRadius;
        int sphereSegments;
        int sphereRings;

        unsigned int sphereColorMode;

        Sphere
        (
            float radius,
            int segments,
            int rings,
            glm::vec4 color,
            glm::vec3 pos,
            unsigned int colorMode
        );

        void GenerateVertices() override;
        void GenerateIndices() override;
};

#endif