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

// Henter inn texture og normal-texture
uniform sampler2D cubeTexture;
uniform sampler2D cubeNormalMap;
uniform sampler2D depthMap;

uniform vec3 lightOneColor;
uniform vec3 lightTwoColor;

uniform float heightScale;

// Endelig resultat som sendes ut
out vec4 FragColorResult;

void main()
{
    vec3 fragColorLightOne = getFragColor(TangentLightOnePos, lightOneColor, 0.2, 0.3, 256.0);
    vec3 fragColorLightTwo = getFragColor(TangentLightTwoPos, lightTwoColor, 0.2, 0.3, 64.0);
    
    float lightOneDistance = length(TangentLightOnePos - TangentFragPos);
    float lightTwoDistance = length(TangentLightTwoPos - TangentFragPos);
    
    vec3 colorOne = ( ( lightOneDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightOne);
    vec3 colorTwo = ( ( lightTwoDistance / (lightOneDistance + lightTwoDistance ) ) * fragColorLightTwo);
    
    vec3 result = colorOne + colorTwo;
    
    FragColorResult = vec4(result, 1.0);
}

// Kan settes sammen i en metode: Sende lightPos, fragPos og LightColor
vec3 getFragColor(vec3 lightPos, vec3 lightColor, float ambientStrength, float specularStrength, float specularShininess) {
    
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec2 parallaxTextureCoords = cubeTextureCoordinates;
    
    parallaxTextureCoords = ParallaxMapping(cubeTextureCoordinates,  viewDir);
    if(parallaxTextureCoords.x > 1.0 || parallaxTextureCoords.y > 1.0 || parallaxTextureCoords.x < 0.0 || parallaxTextureCoords.y < 0.0)
        discard;
    
    // Normalverdier
    vec3 normal = texture(cubeNormalMap, parallaxTextureCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    
    // diffuse-farge
    vec3 objectColor = texture(cubeTexture, parallaxTextureCoords).rgb;
    
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
    
    vec3 relectDir = reflect(-lightDir, normal);
    //vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, relectDir), 0.0), specularShininess);
    vec3 specular = specularStrength * spec * lightColor;
    vec3 fragColor = (ambient + diffuse + specular) * objectColor;
    
    return fragColor;
}


vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    // number of depth layers
    const float minLayers = 8;
    const float maxLayers = 32;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
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








