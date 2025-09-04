#ifndef VBO_HPP
#define VBO_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

// Vertex struct containing: positions, colors and normals
struct Vertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 normal;
};

// Vertex Buffer Object
class VBO
{
	public:
		// ID reference OpenGL methods use
		GLuint ID;
		// Constructor sends in vertices
		VBO(std::vector<Vertex>& vertices);

		// Bind VBO to OpenGL context
		void Bind();
		// Unbind VBO from OpenGL context
		void Unbind();
		// Delete VBO
		void Delete();
};

#endif