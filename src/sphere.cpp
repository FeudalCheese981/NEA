#define _USE_MATH_DEFINES
#include <cmath>

#include "sphere.hpp"

Sphere::Sphere
(
    float radius,
    int segments,
    int rings,
    glm::vec4 color,
	glm::vec3 pos
)
{
    Sphere::radius = radius;
    Sphere::segments = segments;
    Sphere::rings = rings;
	Sphere::color = color;
	objectPos = pos;
    drawType = GL_TRIANGLES;
    GenerateIndices();
    GenerateVertices();
    Update(vertices, indices);
}

void Sphere::GenerateVertices()
{
    for (int i = 0; i <= rings; ++i)
	{
		float phi = M_PI * (float)i / rings;

		for (int j = 0; j <= segments; ++j)
		{
			float theta = 2.0f * M_PI * (float)j / segments;

			float x = radius * sin(phi) * cos(theta);
			float y = radius * sin(phi) * sin(theta);
			float z = radius * cos(phi);

			glm::vec4 color = glm::vec4(
                (x / radius + 1.0f) * 0.5f,
                (y / radius + 1.0f) * 0.5f,
                (z / radius + 1.0f) * 0.5f,
				1.0f
            );
			
			glm::vec3 positon = glm::vec3(x, y, z);
			glm::vec3 normal = glm::normalize(positon);

			vertices.push_back(Vertex{ positon, color, normal });
		}
	}
}

void Sphere::GenerateIndices()
{
    for (int y = 0; y < rings; ++y)
	{
		for (int x = 0; x < segments; ++x)
		{
			int i0 = y * (segments + 1) + x;
			int i1 = i0 + 1;
			int i2 = i0 + (segments + 1);
			int i3 = i2 + 1;

			// Two triangles per quad
			indices.push_back(i0);
			indices.push_back(i2);
			indices.push_back(i1);

			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);
		}
	}
}