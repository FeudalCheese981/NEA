#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "mesh.hpp"

class Object
{
    public:
        std::vector<Vertex> vertices = {};
        std::vector<GLuint> indices = {};

        Mesh objectMesh;

        glm::mat4 objectModel = glm::mat4(1.0f);
        glm::vec3 objectPos;

        GLenum drawType;

        Object();

        void Place(Shader& Shader);
        void Update(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
        void Draw(Shader& shader, Camera& camera, float thickness = 1.0f);
        void Delete();

        virtual void GenerateVertices() = 0;
        virtual void GenerateIndices() = 0;
};

#endif