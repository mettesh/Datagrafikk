#pragma once

#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

class TextureLoading
{
public:
    static GLuint loadTexture( GLchar *path )
    {
        // Generer texture-ID og laste texture-data
        GLuint textureID;
        glGenTextures( 1, &textureID );
        
        int imageWidth, imageHeight;
        
        unsigned char *image = SOIL_load_image( path, &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB );
        
        // Binder dette textures
        glBindTexture( GL_TEXTURE_2D, textureID );
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
        glGenerateMipmap( GL_TEXTURE_2D );
        
        // Parametere
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glBindTexture( GL_TEXTURE_2D,  0);
        
        SOIL_free_image_data( image );
        
        return textureID;
    }
    
    static GLuint loadSkyBox( std::vector<const GLchar * > skyBoxTextureFaces)
    {
        GLuint textureID;
        glGenTextures( 1, &textureID );
        
        int imageWidth, imageHeight;
        unsigned char *image;
        
        glBindTexture( GL_TEXTURE_CUBE_MAP, textureID );
        
        // Lopper igjennom alle sidene (texturene) sendt med som parameter
        for ( GLuint i = 0; i < skyBoxTextureFaces.size( ); i++ )
        {
            // Hente inn bilde og sette bredde og høyde
            image = SOIL_load_image( skyBoxTextureFaces[i], &imageWidth, &imageHeight, 0, SOIL_LOAD_RGB );
            
            // Spesifiserer bildet:
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image );
            
            // Frigjør minnet
            SOIL_free_image_data( image );
        }
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
        glBindTexture( GL_TEXTURE_CUBE_MAP, 0);
        
        return textureID;
    }
};
