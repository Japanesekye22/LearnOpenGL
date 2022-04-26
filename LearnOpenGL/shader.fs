#version 460 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture; // sampler2D type to pass texture object.

void main()
{
    // Calls texture function. texture sampler, texture coordinates.
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0); // can multiply with color
}