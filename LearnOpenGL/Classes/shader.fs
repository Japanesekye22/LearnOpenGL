#version 460 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;


void main()
{
    // Mixes two textures with last value.
    FragColor = vec4(lightColor * objectColor, 1.0);
}