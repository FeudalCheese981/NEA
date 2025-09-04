#ifndef MESH_HPP
#define MESH_HPP

#include <string>
#include "VAO.hpp"
#include "EBO.hpp"
#include "camera.hpp"

class Mesh
{
	public:
		// Vector containing vertex info
		std::vector<Vertex> vertices;
		// Vector containing indices
		std::vector<GLuint> indices;
		// VAO 
		VAO VAO;

		// Mesh constructor
		Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);

		// Draw Mesh
		void Draw(GLenum type);
		// Update Mesh with new vertices and indices
		void Update(std::vector<Vertex>& vertices, std::vector<GLuint>& indices);
		// Delete Mesh
		void Delete();
};

#endif