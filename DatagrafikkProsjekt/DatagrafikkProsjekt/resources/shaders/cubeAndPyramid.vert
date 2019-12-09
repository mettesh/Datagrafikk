#version 330 core

layout (location = 0) in vec3 cubePosition;
layout (location = 1) in vec3 normalCoordinates;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec3 tangentCoordinates;

// Får inn matriser satt i drawGLScene() og resizeGL()
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Får inn to lys
uniform vec3 lightOnePos;
uniform vec3 lightTwoPos;
uniform vec3 viewPos;

// Sendes til fragmentShader
out vec2 cubeTextureCoordinates;
out vec3 TangentLightOnePos;
out vec3 TangentLightTwoPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;


void main()
{

    // Setter posisjonen til kben for å kunne kalkulere lusets retning
    vec3 FragPos = vec3(model * vec4(cubePosition, 1.0));
    
    // Texture-Koordinater skal kun sendes videre til fragmentShaderen
    cubeTextureCoordinates = textureCoordinates;
    
    // Finner tangent
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 tangent = normalize(normalMatrix * tangentCoordinates);
    vec3 normal = normalize(normalMatrix * normalCoordinates);
    tangent = normalize(tangent - dot(tangent, normal) * normal);
    
    // Regner ut Bitangent ved å ta kryssproduktet av Normal og Tangent
    vec3 biTangent = cross(normal, tangent);
    
    mat3 TBN = transpose(mat3(tangent, biTangent, normal));
    TangentLightOnePos = TBN * lightOnePos;
    TangentLightTwoPos = TBN * lightTwoPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
    
    // Setter posisjonen til kuben i verden
    gl_Position = projection * view * model * vec4(cubePosition, 1.0);
    
}
