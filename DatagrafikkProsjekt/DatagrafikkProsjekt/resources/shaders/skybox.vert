#version 330 core

// Henter inn vec fra skyboxVertices
layout (location = 0) in vec3 position;

// Variabel som skal sendes videre til neste til neste steg i pipeline (Til fragment shaderen)
out vec3 TexCoords;

// Mottar informasjon om hvordan vise det 2-dimenesjonale bildet som 3D
uniform mat4 projection;

//Mottar informasjon om hvordan verden skal være I forhold til kameraet
uniform mat4 view;


void main()
{
    // Finner posisjonen basert på data sendt inn
    vec4 pos = projection * view * vec4(position, 1.0);
    // ???
    gl_Position = pos.xyww;
    
    // Seter TexCoords til å være det som hentes inn med layout (attirbutt 0)
    TexCoords = position;
}
