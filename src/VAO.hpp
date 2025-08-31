#ifndef VAO_HPP
#define VAO_HPP

#include <glad/glad.h>
#include "VBO.hpp"

class VAO
{
	public:
		GLuint ID;
		VAO();

		void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
		void Bind();
		void Unbind();
		void Delete();
};

#endif