#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 camPos;
uniform samplerCube skybox;

void main()
{             
    float Eta = 1.0 / 1.4;
    vec3 norm = normalize(Position - camPos);

    vec3 reflectDir = reflect(norm, normalize(Normal));
    vec3 refractDir = refract(norm, normalize(Normal), Eta);
    vec4 reflectCol = texture(skybox, reflectDir);
    vec4 refractCol = texture(skybox, refractDir);

    FragColor = mix(refractCol, reflectCol, 0.05f);
}  