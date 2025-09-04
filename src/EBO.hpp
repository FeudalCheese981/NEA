#ifndef EBO_HPP
#define EBO_HPP

#include <glad/glad.h>
#include <vector>

// Element Buffer Object
class EBO
{
	public:
		// ID reference that OpenGl methods will use
		GLuint ID;
		// Constructor takes in indices
		EBO(std::vector<GLuint>& indices);

		// Bind EBO to OpenGL context
		void Bind();
		// Unbind EBO from OpenGL context
		void Unbind();
		// Delete EBO
		void Delete();
};

#endif
