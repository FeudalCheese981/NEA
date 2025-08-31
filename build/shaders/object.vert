#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec3 aNormal;

out vec3 crntPos;
out vec4 color;
out vec3 normal;

uniform mat4 camMatrix;
uniform mat4 model;

void main()
{
    crntPos = vec3(model * vec4(aPos, 1.0f));

    color = aColor;
    normal = aNormal;

    gl_Position = camMatrix * vec4(crntPos, 1.0);
}