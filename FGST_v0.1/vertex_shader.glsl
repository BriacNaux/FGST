#version 440 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

 // output a texture to the fragment shader
out vec2 TexCoord;
out vec3 normal;

uniform mat4 MeshMatrix;
uniform mat4 view;
uniform mat4 projection;



void main()
{
    
    gl_Position = projection * view * MeshMatrix * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
    normal = aNormal;
} 