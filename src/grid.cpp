#include "grid.hpp"

Grid::Grid
(
    float size,
    int divisions,
    glm::vec4 color,
	glm::vec3 pos
)
{
    Grid::size = size;
    Grid::divisions = divisions;
    Grid::color = color;
	objectPos = pos;
    drawType = GL_LINES;
    GenerateVertices();
    GenerateIndices();
    Update(vertices, indices);
}

void Grid::GenerateVertices()
{
    float step = (2.0f * size) / divisions;
    float maxRadius = glm::length(glm::vec2(size, size));

	for (int i = 0; i <= divisions; i++)
	{
		float x = -size + i * step;
		for (int j = 0; j <= divisions; j++)
		{
			float y = -size + j * step;

            float dist = glm::length(glm::vec2(x, y));
            float alpha = glm::pow(glm::clamp(1.0f - (dist / size), 0.0f, 1.0f), 2.0f);
			
			vertices.push_back(Vertex{ glm::vec3(x, y, 0.0f), glm::vec4(color.r, color.g, color.b, alpha), glm::vec3(0.0f, 0.0f, 0.0f) });
		}
	}
}

void Grid::GenerateIndices()
{
    for (int i = 0; i <= divisions; i++)
    {
        for (int j = 0; j < divisions; j++)
        {
            int i0 = i * (divisions + 1) + j;
			int i1 = i0 + 1;

			indices.push_back(i0);
			indices.push_back(i1);
        }
    }

	for (int j = 0; j <= divisions; j++)
	{
		for (int i = 0; i < divisions; i++)
		{
			int i0 = i * (divisions + 1) + j;
			int i1 = i0 + (divisions + 1);

			indices.push_back(i0);
			indices.push_back(i1);
		}
	}
}