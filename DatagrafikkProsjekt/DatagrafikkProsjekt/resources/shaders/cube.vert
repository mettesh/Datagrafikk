#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;

// Incoming normal
layout (location = 2) in vec3 normal;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Output variables
// out vec3 interpolatedColor;
out vec3 N;
out vec3 worldVertex;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
    
    
    // Set the world vertex for calculating the light direction in the fragment shader
    worldVertex = vec3(model * vec4(position, 1));
    
    // Set the transformed normal
    N = mat3(model) * normal;
    
    // We assign the color to the outgoing variable.
    //interpolatedColor = texCoord;
}
