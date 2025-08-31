#ifndef GRID_HPP
#define GRID_HPP

#include "object.hpp"

class Grid: public Object
{
    public:
        float size;
        int divisions;
        glm::vec4 color;

        Grid
        (
            float size,
            int divisions,
            glm::vec4 color,
            glm::vec3 pos
        );

        void GenerateVertices() override;
        void GenerateIndices() override;
};

#endif