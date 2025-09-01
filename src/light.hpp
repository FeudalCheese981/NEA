#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "sphere.hpp"

class Light: public Sphere
{
    public:
        Light
        (
            float radius,
            int segments,
            int rings,
            glm::vec4 color,
            glm::vec3 pos
        );
        
        void Draw(Shader& shader, Camera& camera, float thickness = 1.0f);
        void SendLightInfoToShader(Shader& shader);
};

#endif