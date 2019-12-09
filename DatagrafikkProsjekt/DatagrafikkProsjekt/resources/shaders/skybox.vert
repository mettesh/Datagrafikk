#version 330 core

// Henter inn vec fra skyboxVertices
layout (location = 0) in vec3 position;

// Får inn matriser satt i drawGLScene() og resizeGL()
uniform mat4 projection;  // Mottar informasjon om hvordan vise det 2-dimenesjonale bildet som 3D
uniform mat4 view;  //Mottar informasjon om hvordan verden skal være I forhold til kameraet

// Variabel som skal sendes videre til neste til neste steg i pipeline (Til fragment shaderen)
out vec3 TexCoords;

void main()
{
    // Finner posisjonen basert på data sendt inn
    vec4 pos = projection * view * vec4(position, 1.0);
    
    // Plasserer "kuben" i verden
    gl_Position = pos.xyww;
    
    // Sender posisjon videre til fragmentShader
    TexCoords = position;
}
