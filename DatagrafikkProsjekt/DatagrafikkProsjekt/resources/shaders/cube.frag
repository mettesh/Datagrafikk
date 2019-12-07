#version 330 core

// Definerer metoden da jeg ønsker å ha main-funksjonen først.
vec3 getFragColor(vec3 lightPos, vec3 lightColor, vec3 ViewPos, float ambientStrength, float specularStrength, float specularShininess);

// Får inn koordinater fra cubeVertices
in vec2 cubeTextureCoordinates;
in vec3 TangentLightOnePos;
in vec3 TangentLightTwoPos;
in vec3 TangentViewOnePos;
in vec3 TangentViewTwoPos;
in vec3 TangentFragPos;

// Henter inn texture og normal-texture
uniform sampler2D cubeTexture;
uniform sampler2D cubeNormalMap;

uniform vec3 lightOneColor;
uniform vec3 lightTwoColor;

// Endelig resultat som sendes ut
out vec4 FragColorResult;

void main()
{
    vec3 fragColorLightOne = getFragColor(TangentLightOnePos, lightOneColor, TangentViewOnePos, 0.1, 0.5, 64.0);
    vec3 fragColorLightTwo = getFragColor(TangentLightTwoPos, lightTwoColor, TangentViewTwoPos, 0.1, 0.5, 32.0);
    
    float lightOneDistance = length(TangentLightOnePos - TangentFragPos);
    float lightTwoDistance = length(TangentLightTwoPos - TangentFragPos);
    
    vec3 colorOne = ( ( lightOneDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightOne);
    vec3 colorTwo = ( ( lightTwoDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightTwo);
    
    // Hvorfor en svart side??
    vec3 result = colorOne + colorTwo;
    
    FragColorResult = vec4(result, 1.0);
}

// Kan settes sammen i en metode: Sende lightPos, viewPos, fragPos og LightColor
vec3 getFragColor(vec3 lightPos, vec3 lightColor, vec3 viewPos, float ambientStrength, float specularStrength, float specularShininess) {
    
    // Normalverdier
    vec3 normal = texture(cubeNormalMap, cubeTextureCoordinates).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    
    // diffuse-farge
    vec3 objectColor = texture(cubeTexture, cubeTextureCoordinates).rgb;
    
    // ambient
    // We take the light's color, multiply it with a small constant ambient factor,
    // multiply this with the object's color and use it as the fragment's color:
    vec3 ambient = ambientStrength * lightColor;
    
    // diffuse
    vec3 lightDir = normalize(lightPos - TangentFragPos);
    // We then need to measure at what angle the light ray touches the fragment.
    // The angle between the two vectors can then easily be calculated with the dot product.
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    vec3 viewDir = normalize(viewPos - TangentFragPos);
    vec3 relectDir = reflect(-lightDir, normal);
    //vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, relectDir), 0.0), specularShininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 fragColor = (ambient + diffuse + specular) * objectColor;
    
    return fragColor;
}








