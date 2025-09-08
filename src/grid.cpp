#include "grid.hpp"

Grid::Grid
(
    float size,
    int divisions,
    glm::vec4 color,
	glm::vec3 pos
):
Object(color, pos, GL_LINES)
{
    gridSize= size;
    gridDivisions = divisions;
    GenerateVertices();
    GenerateIndices();
    Update();
}

void Grid::GenerateVertices()
{
    float step = (2.0f * gridSize) / gridDivisions;
    float maxRadius = glm::length(glm::vec2(gridSize, gridSize));

	for (int i = 0; i <= gridDivisions; i++)
	{
		float x = -gridSize + i * step;
		for (int j = 0; j <= gridDivisions; j++)
		{
			float y = -gridSize + j * step;

            float dist = glm::length(glm::vec2(x, y));
            float alpha = glm::pow(glm::clamp(1.0f - (dist / gridSize), 0.0f, 1.0f), 2.0f);
			
			objectVertices.push_back(Vertex{ glm::vec3(x, y, 0.0f), glm::vec4(objectColor.r, objectColor.g, objectColor.b, alpha), glm::vec3(0.0f, 0.0f, 0.0f) });
		}
	}
}

void Grid::GenerateIndices()
{
    for (int i = 0; i <= gridDivisions; i++)
    {
        for (int j = 0; j < gridDivisions; j++)
        {
            int i0 = i * (gridDivisions + 1) + j;
			int i1 = i0 + 1;

			objectIndices.push_back(i0);
			objectIndices.push_back(i1);
        }
    }

	for (int j = 0; j <= gridDivisions; j++)
	{
		for (int i = 0; i < gridDivisions; i++)
		{
			int i0 = i * (gridDivisions + 1) + j;
			int i1 = i0 + (gridDivisions + 1);

			objectIndices.push_back(i0);
			objectIndices.push_back(i1);
		}
	}
}