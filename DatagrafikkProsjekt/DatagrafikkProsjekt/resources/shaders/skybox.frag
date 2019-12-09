#version 330 core
// Mottar TexCoords som ble sendt ut fra vertex-shaderen
in vec3 TexCoords;

// Henter inn texture fra koden (samplerCube brukes til skyboxes)
uniform samplerCube cubemapTexture;

// Sender color videre i neste steg i pipeline
out vec4 finalFragmentColor;

void main() {
    // Finner fragmentFarge verdiene som ligger i skybox og TexCoords
    finalFragmentColor = texture(cubemapTexture, TexCoords);
}
