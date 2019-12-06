#version 330 core

// Får inn koordinater fra cubeVertices
in vec2 cubeTextureCoordinates;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// Henter inn texture og normal-texture
uniform sampler2D cubeTexture;
uniform sampler2D cubeNormalMap;

uniform vec3 lightColor;

// Endelig resultat som sendes ut
out vec4 FragColor;

void main()
{
    // Normalverdier
    vec3 normal = texture(cubeNormalMap, cubeTextureCoordinates).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    
    // diffuse-farge
    vec3 color = texture(cubeTexture, cubeTextureCoordinates).rgb;
    
    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * color;
    
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // Specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 relectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    
    vec3 specular = vec3(specularStrength) * spec;
    
    vec3 result = (ambient + diffuse + specular) * lightColor;
    FragColor = vec4(result, 1.0);

}



