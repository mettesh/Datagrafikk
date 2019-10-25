#version 330 core
layout (location = 0) in vec3 position;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    vac4 pos = projection * view * vec4(position, 1.0f);
    gl_Position = pos.xyww;
    TexCoords = positon;
}
