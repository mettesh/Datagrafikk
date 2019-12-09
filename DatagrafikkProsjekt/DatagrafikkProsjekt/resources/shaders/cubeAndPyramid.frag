#version 330 core

// Definerer metoden da jeg ønsker å ha main-funksjonen først.
vec3 getFragColor(vec3 lightPos, vec3 lightColor, float ambientStrength, float specularStrength, float specularShininess);
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

// Får inn koordinater fra cubeVertices
in vec2 cubeTextureCoordinates;
in vec3 TangentLightOnePos;
in vec3 TangentLightTwoPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

// Får inn diffuse, normal og depth-texture
uniform sampler2D cubeTexture;
uniform sampler2D cubeNormalMap;
uniform sampler2D depthMap;

// Får inn dybden på parallexmappingen
uniform float heightScale;

// Får inn farge på lysene
uniform vec3 lightOneColor;
uniform vec3 lightTwoColor;

// Endelig resultat som sendes ut
out vec4 FragColorResult;

void main() {
    // Får endelig fragmentfarge med begge lysene
    vec3 fragColorLightOne = getFragColor(TangentLightOnePos, lightOneColor, 0.2, 0.3, 256.0);
    vec3 fragColorLightTwo = getFragColor(TangentLightTwoPos, lightTwoColor, 0.2, 0.3, 64.0);
    
    // Finner distansen fra begge lys og fragment
    float lightOneDistance = length(TangentLightOnePos - TangentFragPos);
    float lightTwoDistance = length(TangentLightTwoPos - TangentFragPos);
    
    // Finner endelig farge basert på begge lysene
    vec3 colorOne = ( ( lightOneDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightOne);
    vec3 colorTwo = ( ( lightTwoDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightTwo);
    
    // Legger de to fragmentFargene sammen og sender det ut
    vec3 result = colorOne + colorTwo;
    FragColorResult = vec4(result, 1.0);
}

vec3 getFragColor(vec3 lightPos, vec3 lightColor, float ambientStrength, float specularStrength, float specularShininess) {
    
    // Finner view-retning
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    
    // Får Depth-textureFarge til fragmentet
    vec2 parallaxTextureCoords = cubeTextureCoordinates;
    parallaxTextureCoords = ParallaxMapping(cubeTextureCoordinates,  viewDir);
    if(parallaxTextureCoords.x > 1.0 || parallaxTextureCoords.y > 1.0 || parallaxTextureCoords.x < 0.0 || parallaxTextureCoords.y < 0.0)
        discard;
    
    // Får Normal-textureFarge til fragmentet
    vec3 normal = texture(cubeNormalMap, parallaxTextureCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    
    // Får TextureFarge til fragmentet (Diffuse)
    vec3 objectColor = texture(cubeTexture, parallaxTextureCoords).rgb;
    
    /* Ambient */
    // Ganger styrken med fargen sendt inn
    vec3 ambient = ambientStrength * lightColor;
    
    /* Diffuse */
    // Regner ut vinkelen lyset treffer fragmentet. Gjør dette ved å bruke dot.
    vec3 lightDir = normalize(lightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    // Ganger denne med fargen på lyset
    vec3 diffuse = diff * lightColor;
    
    /* Specular */
    // Regner ut refleksjonen
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), specularShininess);
    // Ganger dette med valgt styrke og farge
    vec3 specular = specularStrength * spec * lightColor;
    
    // Legger sammen de 3 lystypene og ganger med fragmentFargen
    vec3 fragColor = (ambient + diffuse + specular) * objectColor;
    
    return fragColor;
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // Kalkulerer størrelsen til hvert lag
    float layerDepth = 1.0 / numLayers;
    // Dypbe til laget
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * heightScale;
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        // get depth of next layer
        currentLayerDepth += layerDepth;
    }
    
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}








