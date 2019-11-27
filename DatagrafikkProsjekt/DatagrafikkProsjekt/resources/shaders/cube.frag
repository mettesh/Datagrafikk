#version 330 core

// Får inn koordinater fra cubeVerticies
in vec2 cubeTextureCoordinates;
//in vec3 Normal;
in vec3 FragPos;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;



// Henter inn cube-texturen
uniform sampler2D cubeTexture;
uniform sampler2D cubeNormalMap;

// Får inn matriser fra main.cpp
uniform vec3 lightPos;
uniform vec3 viewPos;


//uniform vec3 lightColor;


// PRØV LIGHTING-MAPS!!



// Endelig farge på lys sendes ut
out vec4 FragColor;

void main()
{
    vec3 normal = texture(cubeNormalMap, cubeTextureCoordinates).rgb;
    
    normal = normalize(normal * 2.0 - 1.0);
    
    // diffuse-farge
    vec3 color = texture(cubeTexture, cubeTextureCoordinates).rgb;
    
    // ambient
    vec3 ambient = 1.0 * color;
    
    // diffuse
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;
    
    // Specular
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 relectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    
    vec3 specular = vec3(0.2) * spec;
    
    //vec3 result = (ambient + diffuse + specular)
    FragColor = vec4(ambient + diffuse + specular, 1.0);
    
}



