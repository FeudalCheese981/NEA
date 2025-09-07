#define _USE_MATH_DEFINES
#include <cmath>

#include "sphere.hpp"

Sphere::Sphere
(
    float radius,
    int segments,
    int rings,
    glm::vec4 color,
	glm::vec3 pos,
	unsigned int colorMode
):
Object(pos, GL_TRIANGLES)
{
    sphereRadius = radius;
    sphereSegments = segments;
    sphereRings = rings;
	sphereColor = color;
	sphereColorMode = colorMode;
    GenerateIndices();
    GenerateVertices();
    Update(vertices, indices);
}

void Sphere::GenerateVertices()
{
    for (int i = 0; i <= sphereRings; ++i)
	{
		float phi = M_PI * static_cast<float>(i) / static_cast<float>(sphereRings);

		for (int j = 0; j <= sphereSegments; ++j)
		{
			float theta = 2.0f * M_PI * static_cast<float>(j) / static_cast<float>(sphereSegments);
			float x = sphereRadius * sin(phi) * cos(theta);
			float y = sphereRadius * sin(phi) * sin(theta);
			float z = sphereRadius * cos(phi);

			glm::vec4 vertexColor;

			if (sphereColorMode == SPHERE_COLOR_RGB)
			{
				vertexColor = glm::vec4
				(
					(x / sphereRadius + 1.0f) * 0.5f,
					(y / sphereRadius + 1.0f) * 0.5f,
					(z / sphereRadius + 1.0f) * 0.5f,
					1.0f
				);
			}
			else vertexColor = sphereColor;

			glm::vec3 positon = glm::vec3(x, y, z);
			glm::vec3 normal = glm::normalize(positon);
			vertices.push_back(Vertex{ positon, vertexColor, normal });
		}
	}
}

void Sphere::GenerateIndices()
{
    for (int y = 0; y < sphereRings; ++y)
	{
		for (int x = 0; x < sphereSegments; ++x)
		{
			int i0 = y * (sphereSegments + 1) + x;
			int i1 = i0 + 1;
			int i2 = i0 + (sphereSegments + 1);
			int i3 = i2 + 1;

			indices.push_back(i0);
			indices.push_back(i1);
			indices.push_back(i2);

			indices.push_back(i1);
			indices.push_back(i3);
			indices.push_back(i2);
		}
	}
}