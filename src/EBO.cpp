#include "EBO.hpp"

EBO::EBO(std::vector<GLuint>& indices)
{
	// Generate the buffer
	glGenBuffers(1, &ID);
	// Bind the buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// Pass in the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}
