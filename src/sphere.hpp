#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "object.hpp"

const unsigned int SPHERE_COLOR_DEFAULT = 0;
const unsigned int SPHERE_COLOR_RGB = 1;

class Sphere: public Object
{
    public:
        float radius;
        int segments;
        int rings;

        glm::vec4 color;
        unsigned int colorMode;

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