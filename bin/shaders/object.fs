#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 lightFragPos;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

#define PCF 4 // radius of texels 

float Shadow(vec4 light_fragpos)
{
    if (light_fragpos.z > 1.0)
        return 0;
    
    vec3 proj = light_fragpos.xyz * 0.5 + 0.5;
    float currentDepth = proj.z;
    
    // PCF
    float shadow = 0.0;
    float bias = 0.009;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);

    for(int x = -PCF; x <= PCF; ++x)
    {
        for(int y = -PCF; y <= PCF; ++y)
        {
            float pcfDepth = texture(shadowMap, proj.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= (2*PCF+1)*(2*PCF+1);
        
    return shadow;
}

void main()
{           
    vec3 color = texture(diffuseTexture, TexCoords).rgb;
    vec3 lightColor = vec3(0.47f, 0.4f, 0.43f);

    // Ambient
    vec3 ambient = 0.4f * color;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
        
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    vec3 specular = spec * lightColor;  

    // Shadow                     
    vec3 lighting = color * (ambient + (diffuse + specular) * (1.0 - Shadow(lightFragPos)));    
    
    FragColor = vec4(lighting, 1.0);
}