#version 330 core
out vec4 TexCoords;

out vec4 color;

//For Texture:
uniform samplerCube skybox;

void main()
{
    color = texture(skybox, TexCoord);
}
