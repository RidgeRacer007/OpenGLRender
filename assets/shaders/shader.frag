#version 330 core

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objColor;
uniform vec3 lightsrcColor;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightsrcColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diffuse = max(dot(norm, lightDir), 0.0);
    vec3 diffusion = diffuse * lightsrcColor;  

    float specularStrength = 0.5;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir),0.0), 2);
    vec3 specular = specularStrength * spec * lightsrcColor;

    vec3 result = (ambient + diffusion + specular) * objColor;

    FragColor = vec4(result, 1.0);
}