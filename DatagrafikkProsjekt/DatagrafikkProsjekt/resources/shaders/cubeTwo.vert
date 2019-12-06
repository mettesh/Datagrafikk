#version 330 core

layout (location = 0) in vec3 cubePosition;
layout (location = 1) in vec3 normalCoordinates;
layout (location = 2) in vec2 textureCoordinates;

// Får inn matriser satt i drawGLScene() og resizeGL()
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


// Output variables
out vec2 cubeTextureCoordinates;
out vec3 Normal;
out vec3 FragPos;


void main()
{
    // Setter posisjonen til kuben i verden
    gl_Position = projection * view * model * vec4(cubePositions, 1.0f);
    
    // Kalkulerer texturekoordinater til kuben som skal sendes til frag-shader
    cubeTextureCoordinates = vec2(textureCoordinates.x, 1.0 - textureCoordinates.y);

    // Set the world vertex for calculating the light direction in the fragment shader
    //worldVertex = vec3(model * vec4(cubePositions, 1));
    FragPos = vec3(model * vec4(cubePositions, 1.0));
    
    // Set the transformed normal
    //Normal = mat3(model) * aNormal;
    Normal = mat3(transpose(inverse(model))) * normalCoordinates;
    
    // We assign the color to the outgoing variable.
   //interpolatedColor = textureCoordinates;
}

