#version 330 core

// Får inn koordinater til hvor texture skal legges fra vert-shader
in vec2 cubeTextureCoordinates;
in vec3 Normal;
in vec3 FragPos;


uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

// Henter inn cube-texturen
uniform sampler2D cubeTexture;





// Vectors
/*vec3 L;
vec3 NN;
vec3 V;
vec3 R;
 */

// Colors
//vec4 lightColor;
/*
vec4 ambient;
vec4 diffuse;
vec4 specular;

vec3 result;
 */


// Endelig farge på lys sendes ut
out vec4 FragColor;


void main()
{

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * texture( cubeTexture, cubeTextureCoordinates ).rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture( cubeTexture, cubeTextureCoordinates ).rgb;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    
    vec3 specular = specularStrength * spec * texture( cubeTexture, cubeTextureCoordinates ).rgb;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    
    FragColor = vec4(result, 1.0);
    

}



