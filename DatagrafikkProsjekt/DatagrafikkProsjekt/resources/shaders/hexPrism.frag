#version 330 core

// Definerer metoden da jeg ønsker å ha main-funksjonen først.
vec3 getFragColor(vec3 lightPos, vec3 lightColor, float ambientStrength, float specularStrength, float specularShininess);

// Får inn koordinater fra cubeTwoVertices
in vec2 cubeTextureCoordinates;
in vec3 normalCoordinates;
in vec3 FragPos;

// Henter inn cube-texturen
uniform sampler2D cubeTexture;

// Får inn lys-matriser
uniform vec3 lightOnePos;
uniform vec3 lightOneColor;
uniform vec3 lightTwoPos;
uniform vec3 lightTwoColor;
uniform vec3 viewPos;

// Endelig resultat som sendes ut
out vec4 FragColorResult;

void main() {
    // Får endelig fragmentfarge med begge lysene
    vec3 fragColorLightOne = getFragColor(lightOnePos, lightOneColor, 0.2, 0.3, 256.0);
    vec3 fragColorLightTwo = getFragColor(lightTwoPos, lightTwoColor, 0.2, 0.3, 64.0);
    
    // Finner distansen fra begge lys og fragment
    float lightOneDistance = length(lightOnePos - FragPos);
    float lightTwoDistance = length(lightTwoPos - FragPos);
    
    // Finner endelig farge basert på begge lysene
    vec3 colorOne = ( ( lightOneDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightOne);
    vec3 colorTwo = ( ( lightTwoDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightTwo);
    
    // Legger de to fragmentFargene sammen og sender det ut
    vec3 result = colorOne + colorTwo;
    FragColorResult = vec4(result, 1.0);
}

vec3 getFragColor(vec3 lightPos, vec3 lightColor, float ambientStrength, float specularStrength, float specularShininess) {
    // Normalverdier
    vec3 normal = normalize(normalCoordinates);
    
    // Får TextureFarge til fragmentet (Diffuse)
    vec3 objectColor = texture(cubeTexture, cubeTextureCoordinates).rgb;
    
    // Ambient
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), specularShininess);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 fragColor = (ambient + diffuse + specular) * objectColor;
    return fragColor;

}






