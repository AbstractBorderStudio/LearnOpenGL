#version 330 core
in vec3 vCol;
in vec2 texPos;
out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
   FragColor = texture(ourTexture, texPos) * vec4(vCol, 1.0);
}