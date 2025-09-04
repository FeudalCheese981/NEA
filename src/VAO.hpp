#ifndef VAO_HPP
#define VAO_HPP

#include <glad/glad.h>
#include "VBO.hpp"

// Vertex Array Object
class VAO
{
	public:
		// ID reference OpenGL methods will use
		GLuint ID;
		// Constructor
		VAO();

		// Link VBO attribute to VAO
		void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		// Bind VAO to OpenGL context
		void Bind();
		// Unbind VAO from OpenGl context
		void Unbind();
		// Delete VAO
		void Delete();
};

#endif