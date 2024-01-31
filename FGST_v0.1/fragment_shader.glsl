#version 440 core

out vec4 FragColor;


in vec2 TexCoord; 
in vec3 normal;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{

    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.3); //texture() est une fonction qui permet de renvoyer un fragment de texture
    //FragColor = vec4(0.0f, 0.5f, 1.0f, 1.0f);
}