#version 330 core

// Definerer metoden da jeg ønsker å ha main-funksjonen først.
vec3 getFragColor(vec3 lightPos, vec3 lightColor, vec3 ViewPos, float ambientStrength, float specularStrength, float specularShininess);

// Får inn koordinater fra cubeTwoVertices
in vec2 cubeTextureCoordinates;
in vec3 normalCoordinates;
in vec3 FragPos;

// Henter inn cube-texturen
uniform sampler2D cubeTexture;

// Får inn matriser fra main.cpp
uniform vec3 lightOnePos;
uniform vec3 viewOnePos;
uniform vec3 lightOneColor;

uniform vec3 lightTwoPos;
uniform vec3 viewTwoPos;
uniform vec3 lightTwoColor;

// Endelig resultat som sendes ut
out vec4 FragColorResult;

void main()
{
    vec3 fragColorLightOne = getFragColor(lightOnePos, lightOneColor, viewOnePos, 0.1, 0.5, 64.0);
    vec3 fragColorLightTwo = getFragColor(lightTwoPos, lightTwoColor, viewTwoPos, 0.1, 0.5, 256.0);
    
    float lightOneDistance = length(lightOnePos - FragPos);
    float lightTwoDistance = length(lightTwoPos - FragPos);
    
    vec3 colorOne = ( ( lightOneDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightOne);
    vec3 colorTwo = ( ( lightTwoDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightTwo);
    
    // Hvorfor en svart side??
    vec3 result = colorOne + colorTwo;
    
    FragColorResult = vec4(result, 1.0);
}

// Kan settes sammen i en metode: Sende lightPos, viewPos, fragPos og LightColor
vec3 getFragColor(vec3 lightPos, vec3 lightColor, vec3 viewPos, float ambientStrength, float specularStrength, float specularShininess)
{
    
    // Normalverdier
    vec3 normal = normalize(normalCoordinates);
    
    // diffuse-farge
    vec3 objectColor = texture(cubeTexture, cubeTextureCoordinates).rgb;
    
    // ambient
    //float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * objectColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * objectColor;

    // Specular
    //float specularStrength = 1.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    vec3 specular = vec3(specularStrength) * spec;
    
    vec3 fragColor = (ambient + diffuse + specular) * lightColor;
    return fragColor;

}






