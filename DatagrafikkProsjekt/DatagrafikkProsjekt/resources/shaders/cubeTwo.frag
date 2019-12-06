#version 330 core

// Får inn koordinater fra cubeTwoVertices
in vec2 cubeTextureCoordinates;
in vec3 normalCoordinates;
in vec3 FragPos;

// Henter inn cube-texturen
uniform sampler2D cubeTexture;

// Får inn matriser fra main.cpp
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

// Endelig resultat som sendes ut
out vec4 FragColor;

void main()
{
    // Normalverdier
    vec3 normal = normalize(normalCoordinates);
    
    // diffuse-farge
    vec3 color = texture(cubeTexture, cubeTextureCoordinates).rgb;
    
    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * color;

    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // Specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32);
    
    vec3 specular = vec3(specularStrength) * spec;
    
    
    vec3 result = (ambient + diffuse + specular) * lightColor;
    FragColor = vec4(result, 1.0);

    
}






