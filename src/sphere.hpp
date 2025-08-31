#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "object.hpp"

class Sphere: public Object
{
    public:
        float radius;
        int segments;
        int rings;

        glm::vec4 color;

        Sphere
        (
            float radius,
            int segments,
            int rings,
            glm::vec4 color,
            glm::vec3 pos
        );

        void GenerateVertices() override;
        void GenerateIndices() override;
};

#endif