#version 330 core

vec3 normal;
vec3 color;

float DOne;
float DTwo;


// Sette tilhørende variabler i objekter  . .  Lys1{. . . }
// Får inn koordinater fra cubeVertices
in vec2 cubeTextureCoordinates;
in vec3 TangentLightPos;
in vec3 TangentLightTwoPos;
in vec3 TangentViewPos;
in vec3 TangentViewTwoPos;
in vec3 TangentFragPos;

// Henter inn texture og normal-texture
uniform sampler2D cubeTexture;
uniform sampler2D cubeNormalMap;

uniform vec3 lightColor;
uniform vec3 lightColorTwo;

// Endelig resultat som sendes ut
out vec4 FragColor;


// Kan settes sammen i en metode: Sende lightPos, viewPos, fragPos og LightColor
vec3 getLightOne() {
    
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

    DOne = length(TangentLightPos - TangentFragPos);
    
    vec3 lightOne = (ambient + diffuse + specular) * lightColor;
    
    return lightOne;
    
}

vec3 getLightTwo() {
    
    // ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * color;
    
    // diffuse
    vec3 lightDir = normalize(TangentLightTwoPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // Specular
    float specularStrength = 1.0;
    vec3 viewDir = normalize(TangentViewTwoPos - TangentFragPos);
    vec3 relectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(specularStrength) * spec;
    
    DTwo = length(TangentLightTwoPos - TangentFragPos);
    
    vec3 lightTwo = (ambient + diffuse + specular) * lightColorTwo;
    
    return lightTwo;
}


void main()
{
    // Normalverdier
    normal = texture(cubeNormalMap, cubeTextureCoordinates).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    
    // diffuse-farge
    color = texture(cubeTexture, cubeTextureCoordinates).rgb;
      
    vec3 lightOne = getLightOne();
    vec3 lightTwo = getLightTwo();
    
    vec3 colorOne = ( ( DOne / (DOne + DTwo ) ) * lightOne);
    vec3 colorTwo = ( ( DTwo / (DOne + DTwo ) ) * lightTwo);
    
    vec3 result = lightOne + lightTwo;
    
    FragColor = vec4(result, 1.0);
}





