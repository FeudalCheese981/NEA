#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include "VAO.hpp"
#include "EBO.hpp"
#include "camera.hpp"

class Mesh
{
	public:
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;

		VAO VAO;

		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

		void Draw(Shader& shader, Camera& camera, GLenum type);
		void Update(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
		void Delete();
};

#endif