#version 330 core

layout (location = 0) in vec3 cubePositions;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 textureCoordinates;

// Får inn matriser satt i drawGLScene() og resizeGL()
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Output variables
out vec2 cubeTextureCoordinates;
out vec3 normalCoordinates;
out vec3 FragPos;


void main()
{
    // Setter posisjonen til kuben i verden
    gl_Position = projection * view * model * vec4(cubePositions, 1.0f);
    
    // Kalkulerer texturekoordinater til kuben som skal sendes til frag-shader
    cubeTextureCoordinates = vec2(textureCoordinates.x, 1.0 - textureCoordinates.y);

    // Set the world vertex for calculating the light direction in the fragment shader
    FragPos = vec3(model * vec4(cubePositions, 1.0));
    
    // Set the transformed normal
    normalCoordinates = mat3(transpose(inverse(model))) * aNormal;
   
}
