#version 460 core

in vec3 crntPos;
in vec4 color;
in vec3 normal;

out vec4 FragColor;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{   
    vec3 norm = normalize(normal);
    vec3 lightDirection = normalize(lightPos - crntPos);

    float diffuse = max(dot(norm, lightDirection), 0.0f);
    float specularLight = 0.05f;
    vec3 viewDirection = normalize(camPos - crntPos);
    vec3 reflectionDirection = reflect(-lightDirection, norm);
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
    float specular = specAmount * specularLight;

    FragColor = vec4( vec3(color) * (diffuse + specular), 1.0f);
}