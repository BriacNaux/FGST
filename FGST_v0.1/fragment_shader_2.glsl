#version 440 core

out vec4 FragColor;


in vec2 TexCoord; 

in vec3 FragPos;

in vec3 normal;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;

uniform vec3 viewPos;

void main()
{
    

    
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;


    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.7;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
    vec3 specular = specularStrength * spec * lightColor;  
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}