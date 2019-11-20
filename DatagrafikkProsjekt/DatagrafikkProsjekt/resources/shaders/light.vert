#version 330

// Incoming vertex position, Model Space.
layout (location = 0) in vec3 aPosition;

// Incoming vertex color.
layout (location = 1) in vec3 aNormal;

// Incoming normal
//layout (location = 2) in vec3 normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

// Output variables
//out vec3 interpolatedColor;
out vec3 Normal;
out vec3 worldVertex;

void main() {
    
    // Normally gl_Position is in Clip Space and we calculate it by multiplying together all the matrices
    gl_Position = projection * (view * (model * vec4(aPosition, 1)));
    
    // Set the world vertex for calculating the light direction in the fragment shader
    worldVertex = vec3(model * vec4(aPosition, 1));
    
    // Set the transformed normal
    Normal = mat3(model) * aNormal;
    
    // We assign the color to the outgoing variable.
    //interpolatedColor = color;
    
}
