#version 330 core
// Mottar TexCoords som ble sendt ut fra vertex-shaderen
in vec3 TexCoords;

// Sender color videre i neste steg i pipeline
out vec4 color;

// Henter inn data fra koden (samplerCube brukes til skyboxes)
uniform samplerCube skybox;

void main()
{
    // Finner color utifra verdiene som ligger i skybox og TexCoords
    color = texture(skybox, TexCoords);
}
