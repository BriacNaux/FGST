#version 440 core
out vec4 FragColor;


in vec2 TexCoord; 
in vec3 normal;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
    FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f); // set all 4 vector values to 1.0
}