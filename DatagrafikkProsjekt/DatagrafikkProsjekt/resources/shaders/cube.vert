#version 330 core

layout (location = 0) in vec3 cubePositions;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 textureCoordinates;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

// Får inn matriser satt i drawGLScene() og resizeGL()
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

// Output variables
out vec2 cubeTextureCoordinates;
//out vec3 Normal;
out vec3 FragPos;

// out - mapping
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;


void main()
{

    // Set the world vertex for calculating the light direction in the fragment shader
    //worldVertex = vec3(model * vec4(cubePositions, 1));
    FragPos = vec3(model * vec4(cubePositions, 1.0));
    
    // Kalkulerer texturekoordinater til kuben som skal sendes til frag-shader
    //cubeTextureCoordinates = vec2(textureCoordinates.x, 1.0 - textureCoordinates.y);
    cubeTextureCoordinates = textureCoordinates;
    
    
    // NormalMapping
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T = normalize(T - dot(T, N) * N);
    vec3 B = cross(N, T);
    
    mat3 TBN = transpose(mat3(T, B, N));
    TangentLightPos = TBN * lightPos;
    TangentViewPos  = TBN * viewPos;
    TangentFragPos  = TBN * FragPos;
    
    // Setter posisjonen til kuben i verden
    gl_Position = projection * view * model * vec4(cubePositions, 1.0);
    
}
