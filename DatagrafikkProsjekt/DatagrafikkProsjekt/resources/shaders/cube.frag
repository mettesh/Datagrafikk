#version 330 core

// Får inn koordinater til hvor texture skal legges fra vert-shader
in vec2 cubeTextureCoordinates;

// Henter inn cube-texturen
uniform sampler2D cubeTexture;


// Sender ut resultat
out vec4 textureResult;


void main()
{
    // Setter texture med innlasted bilde(texture) og korrdinater hvor det skal settes
    textureResult = texture(cubeTexture, cubeTextureCoordinates);
}
