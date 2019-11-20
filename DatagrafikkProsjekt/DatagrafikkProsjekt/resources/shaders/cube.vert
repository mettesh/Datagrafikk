#version 330 core

// Henter inn vec-verdier fra cubeVertices - De 3 første posisjon, de 3 neste texCoord
layout (location = 0) in vec3 cubePositions;
layout (location = 1) in vec2 textureCoordinates;

// Får inn matriser satt i drawGLScene()
uniform mat4 model;
uniform mat4 view;
// Får inn matrise satt i og resizeGL()
uniform mat4 projection;


// Sender texturekoordinater videre til frag-shader
out vec2 cubeTextureCoordinates;

void main()
{
    // Setter posisjonen til kuben i verden
    gl_Position = projection * view * model * vec4(cubePositions, 1.0f);
    // Kalkulerer texturekoordinater til kuben som skal sendes til frag-shader
    cubeTextureCoordinates = vec2(textureCoordinates.x, 1.0 - textureCoordinates.y);
}
