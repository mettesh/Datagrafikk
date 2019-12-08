#version 330

// Incoming variables.
in vec2 UV;
in vec3 N;
in vec3 worldVertex;

uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec4 shininessColor;
uniform float shininess;
uniform vec3 cameraPosition;

uniform vec3 lightOnePos
uniform vec3 lightOneColor;
uniform vec3 viewOnePos;



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
uniform sampler2D textureSampler;

void main()
{
    // Normalverdier
    vec3 normal = normalize(normalCoordinates);
     
    // diffuse-farge
    vec3 objectColor = texture(textureSampler, UV).rgba;
     
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularShininess = 64.0;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), specularShininess);
    vec3 specular = specularStrength * spec * lightColor;
     
    vec3 outputColor = (ambient + diffuse + specular) * objectColor;
    


}

