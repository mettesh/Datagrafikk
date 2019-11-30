#version 330

// Incoming variables.
in vec2 TextureCoordinates;
in vec3 N;
in vec3 worldVertex;

uniform vec3 lightPos;
uniform vec3 viewPos;

// Outgoing final color.
out vec4 outputColor;

// Vectors
vec3 L;
vec3 NN;
vec3 V;
vec3 R;

// Colors
vec4 color;
vec4 ambient;
vec4 diffuse;
vec4 specular;

// Texture sampler
uniform sampler2D sphereTexture;

void main()
{

    
    
    
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * texture( sphereTexture, TextureCoordinates ).rgb;

    // diffuse
    vec3 norm = normalize(N);
    vec3 lightDir = normalize(lightPos - worldVertex);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * texture( sphereTexture, TextureCoordinates ).rgb;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - worldVertex);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //vec3 specular = specularStrength * spec * lightColor;
        
    
    
    vec3 specular = specularStrength * spec * texture( sphereTexture, TextureCoordinates ).rgb;
    
    vec3 result = (ambient + diffuse + specular);
    
    outputColor = vec4(result, 1.0);

}
