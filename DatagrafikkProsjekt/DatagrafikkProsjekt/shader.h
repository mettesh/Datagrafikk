#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>

//Oppretter en Shader-klasse med en variabel (Program) + 1 konstruktør og 1 metode
class Shader
{
public:
    GLuint Program;
    // Genererer shadere. Parametre sendt med : "resources/shaders/shader.vert", "resources/shaders/shader.frag"
    Shader() {}
    Shader( const GLchar *vertexPath, const GLchar *fragmentPath )
    {
// 1. Hente inn kildekoden fra begge shaderne fra filstien
        //Oppretter 2 string-objekter
        std::string vertexCode;
        std::string fragmentCode;
        //Oppretter 2 ifstream-objekter (Til å lese fra filene)
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // Sørger for at ifstream-objektene kan kaste exceptions
        vShaderFile.exceptions ( std::ifstream::badbit );
        fShaderFile.exceptions ( std::ifstream::badbit );
        try
        {
            // Benytter ifstream-objektene til å åpne shader-filene
            vShaderFile.open( vertexPath );
            fShaderFile.open( fragmentPath );
            //Oppretter 2 stringstream (Som vil lese inn data)
            std::stringstream vShaderStream, fShaderStream;
            // Leser filenes buffer inn til streams
            vShaderStream << vShaderFile.rdbuf( );
            fShaderStream << fShaderFile.rdbuf( );
            // Lukker filhandlerne
            vShaderFile.close( );
            fShaderFile.close( );
            // Konverterer stream til String
            vertexCode = vShaderStream.str( );
            fragmentCode = fShaderStream.str( );
        }
        catch ( std::ifstream::failure e )
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        //Stringene lest inn fra buffer over legges i hver sin GLchar-variabel
        //Gjør de om til c_str (Som er en av 2 typer Stringer C har. C string = array med char)
        const GLchar *vShaderCode = vertexCode.c_str( );
        const GLchar *fShaderCode = fragmentCode.c_str( );
        
// 2. Kompliere shaderne
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];
        // Vertex Shader
        vertex = glCreateShader( GL_VERTEX_SHADER );
        glShaderSource( vertex, 1, &vShaderCode, NULL );
        glCompileShader( vertex );
        // Printe ut feilmelding om kompileringen feiler
        glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( vertex, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Fragment Shader (Gjør det samme som for Vertex shaderen)
        fragment = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( fragment, 1, &fShaderCode, NULL );
        glCompileShader( fragment );
        glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( fragment, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        
        // Oppretter shader-program
        this->Program = glCreateProgram( );
        glAttachShader( this->Program, vertex );
        glAttachShader( this->Program, fragment );
        glLinkProgram( this->Program );
        // Printer feilmelding om linkingen feiler
        glGetProgramiv( this->Program, GL_LINK_STATUS, &success );
        if (!success)
        {
            glGetProgramInfoLog( this->Program, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // Nå som shaderne er linket inn i programmet trenger vi ikke lenger disse shaderne. Sletter derfor disse.
        glDeleteShader( vertex );
        glDeleteShader( fragment );
        
    }
    // Metode for å bruke valgt program.
    void Use( )
    {
        glUseProgram( this->Program );
    }
};

#endif
