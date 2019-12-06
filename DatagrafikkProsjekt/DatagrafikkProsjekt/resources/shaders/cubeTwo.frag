#version 330 core

// Får inn koordinater fra cubeVerticies
in vec2 cubeTextureCoordinates;
in vec3 Normal;
in vec3 FragPos;

// Henter inn cube-texturen
uniform sampler2D cubeTexture;

// Får inn matriser fra main.cpp
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;


// PRØV LIGHTING-MAPS!!



// Endelig farge på lys sendes ut
out vec4 FragColor;

void main()
{
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * vec3(texture( cubeTexture, cubeTextureCoordinates ));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(texture( cubeTexture, cubeTextureCoordinates ));

    // specular
    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(texture( cubeTexture, cubeTextureCoordinates ));
    
    
    vec3 result = (ambient + diffuse + specular) * lightColor;
    FragColor = vec4(result, 1.0);

    
}






