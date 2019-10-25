#version 330 core
out vec4 color;

// Få inn fragPosition og Normal som man sender ut fra lighting-vertex-shaderen
in vec3 FragPos;
in vec3 Normal;

// deklarerer vec-er
uniform vec3 lightPos;
uniform vec3 viewPos;

uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{    
    // AMBIENT
    // Det generelle lyset som er i en scene- Som er i rommet. har ingen direkte origin/position
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    
    // DIFFUSE
    // Tar hensyn til retning og vinkel på obektet som får lyset på seg
    vec3 norm = normalize(Normal);
    // REgner ut hvilken retning lyset skal sendes:
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // SPECULAR
    // Det lille skinnende lyset på objekter
    float specularStrength = 20.0f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * objectColor;
    color = vec4(result, 1.0f);
}


