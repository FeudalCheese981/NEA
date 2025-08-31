#version 460 core

// input layouts
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;

// outputs
out vec4 color;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
	color = aColor;

	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
};