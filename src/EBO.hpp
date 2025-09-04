#ifndef EBO_HPP
#define EBO_HPP

#include <glad/glad.h>
#include <vector>

// Element Buffer Object
class EBO
{
	public:
		// ID reference that opengl methods will use
		GLuint ID;
		// Constructor takes in indices
		EBO(std::vector<GLuint>& indices);

		// Bind the EBO to the OpenGL context
		void Bind();
		// Unbind the EBO from the OpenGL context
		void Unbind();
		// Delete the EBO
		void Delete();
};

#endif
