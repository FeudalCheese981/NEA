#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "mesh.hpp"

class Object
{
    public:
        std::vector<Vertex> objectVertices = {};
        std::vector<GLuint> objectIndices = {};

        Mesh objectMesh;

        glm::vec4 objectColor;
        glm::mat4 objectModel;
        glm::vec3 objectPos;

        GLenum objectDrawType;

        Object(glm::vec4 color, glm::vec3 pos, GLenum drawType);

        void Place();
        void Update();
        void Draw(Shader& shader, Camera& camera, float thickness = 1.0f);
        void Delete();

        virtual void GenerateVertices() = 0;
        virtual void GenerateIndices() = 0;
};

#endif