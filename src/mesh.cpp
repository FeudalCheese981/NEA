#include "mesh.hpp"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	Update(vertices, indices);
}

void Mesh::Update(std::vector<Vertex>& vertices, std::vector<GLuint>& indices)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;

	VAO.Bind();
	VBO VBO(vertices);
	EBO EBO(indices);
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);
	VAO.LinkAttrib(VBO, 1, 4, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(GLfloat)));
	VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(7 * sizeof(GLfloat)));

	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();

	VBO.Delete();
	EBO.Delete();
}

void Mesh::Draw(Shader& shader, Camera& camera, GLenum type)
{
	VAO.Bind();
	glDrawElements(type, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::Delete()
{
	VAO.Delete();
}