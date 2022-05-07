#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1; // sampler2D type to pass texture object.
uniform sampler2D texture2;

void main()
{
    // Mixes two textures with last value.
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f);
}