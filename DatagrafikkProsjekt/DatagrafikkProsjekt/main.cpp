#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2.h"

// Egne includes
#include "shader.h"
#include "camera.h"
#include "texture.h"

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

// Vertex Buffer Identifiers
#define VERTICES 0

// Vertex Array attributes
#define POSITION 0
#define NORMAL 1
#define COLOR 2
#define TANGENT 3
#define BITANGENT 4

// GLSL Uniform indices
#define TRANSFORM0 0
#define TRANSFORM1 1
#define LIGHT 2
#define MATERIAL 3
#define CAMERA 4

// KUBE:
/*
 GLfloat cubeVertices[] =
 {
     // Posisjon            // Texture      // Normal
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 0.0f,     0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f, 0.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 0.0f,     0.0f, 0.0f, 1.0f,
              
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 0.0f, -1.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 0.0f, -1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 1.0f,     0.0f, 0.0f, -1.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,     0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 0.0f, -1.0f,
              
     -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
     -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
     -0.5f, -0.5f, -0.5f,   0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
     -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
     -0.5f,  0.5f,  0.5f,   1.0f, 0.0f,    -1.0f, 0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 0.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     1.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, -0.5f,    1.0f, 1.0f,     0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,    1.0f, 0.0f,     0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,    0.0f, 0.0f,     0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,    0.0f, 1.0f,     0.0f, 1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f, -1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,    1.0f, 1.0f,     0.0f, -1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,    1.0f, 0.0f,     0.0f, -1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,    0.0f, 0.0f,     0.0f, -1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,    0.0f, 1.0f,     0.0f, -1.0f, 0.0f
 };
 */


// Dimensjonene til vinduet
// const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// For tastatur- og musebevegelser
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void generateSkyBoxVerticesAndSetArraysAndBuffers();
void generateCubeVerticesAndSetArraysAndBuffers();
unsigned int loadTexture(const char *path);

// Setter startposisjon til kamera
Camera camera( glm::vec3( 1.0f, 0.0f, 3.0f ) );

GLfloat lastX = SCREEN_WIDTH / 2.0f;
GLfloat lastY = SCREEN_WIDTH / 2.0f;

bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Vertex-array og buffer -object for KUBE og SKYBOX
GLuint cubeVAO, cubeVBO;
GLuint skyboxVAO, skyboxVBO;

// Shadere
Shader cubeShader;
Shader skyboxShader;

// Textures
GLuint cubemapTextureValue;
GLuint cubeTextureValue;
GLuint cubeNormalMapValue;


// Cube & light  Uniform locations
GLint modelLoc;
GLint viewLoc;
GLint projLoc;

GLint cubeTextureLoc;
GLint cubeNormalMapLoc;

// GLint lightColorLoc;
GLint lightPositionLoc;
GLint viewPositionLoc;

// Skybox Uniform locations
GLint projLocSkybox;
GLint viewLocSkybox;

// Uniforms values
GLfloat lightPositionValue[] { 0.5f, 1.0f, 0.3f };
GLfloat cameraPositionValue[] { 1.0f, 0.0f, 4.0f };

// GLfloat lightColorValue[] = {1.0f, 0.5f, 0.31f};



/*
 * Initialize OpenGL
 */
int initGL() {
    
    generateSkyBoxVerticesAndSetArraysAndBuffers();
    generateCubeVerticesAndSetArraysAndBuffers();
    
    // Setup and compile our shaders
    cubeShader = Shader( "resources/shaders/cube.vert", "resources/shaders/cube.frag" );
    skyboxShader = Shader( "resources/shaders/skybox.vert", "resources/shaders/skybox.frag" );

    //Laste inn texture til kuben:
    cubeTextureValue = TextureLoading::LoadTexture("resources/img/cube/texture.jpg");
    cubeNormalMapValue = TextureLoading::LoadTexture("resources/img/cube/texture_normal.jpg");
    
    //Laste inn texture til skyboxen:
    std::vector<const GLchar*> skyBoxTextureFaces;
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_lf.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_rt.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_up.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_dn.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_ft.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_bk.tga" );
    cubemapTextureValue = TextureLoading::LoadCubemap( skyBoxTextureFaces );


    // Henter inn uniform-loactions fra cube-shadere
    cubeShader.Use();
    viewLoc = glGetUniformLocation( cubeShader.Program, "view" );
    projLoc = glGetUniformLocation( cubeShader.Program, "projection" );
    modelLoc = glGetUniformLocation( cubeShader.Program, "model" );
    
    cubeTextureLoc = glGetUniformLocation( cubeShader.Program, "cubeTexture" );
    cubeNormalMapLoc = glGetUniformLocation( cubeShader.Program, "cubeNormalMap" );
    
    //lightColorLoc = glGetUniformLocation(cubeShader.Program, "lightColor" );
    lightPositionLoc = glGetUniformLocation( cubeShader.Program, "lightPos" );
    viewPositionLoc = glGetUniformLocation( cubeShader.Program, "viewPos" );
    
    
    // Henter inn uniform-loactions fra skybox-shader
    skyboxShader.Use();
    projLocSkybox = glGetUniformLocation( skyboxShader.Program, "projection" );
    viewLocSkybox = glGetUniformLocation( skyboxShader.Program, "view" );
    
    
    
    // Implementerer Depth i applikasjonen ?????
    glEnable(GL_DEPTH_TEST);
    
    return 1;
}


/*
 * Draw OpenGL screne
 */
void drawGLScene() {
    
    // Setter clear-farge og dybdebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    DoMovement();
    
    float time = glfwGetTime();
    
    /* * * * * * *
    *
    * Tegner skyboxen:
    *
     * * * * * * */;

        skyboxShader.Use();
        
        // Change depth function so depth test passes when values are equal to depth buffer's content
        glDepthFunc( GL_LEQUAL );

        // TODO: Tidligere. Hva gjør denne:  glm::mat4 viewSkybox = camera.GetViewMatrix();
        glm::mat4 viewSkyboxValue = glm::mat4( glm::mat3( camera.GetViewMatrix( ) ) );
        glUniformMatrix4fv( viewLocSkybox, 1, GL_FALSE, glm::value_ptr( viewSkyboxValue ) );


    
        // Aktiverer vertex-arrayen for skyBox:
        glBindVertexArray( skyboxVAO );
        
        glBindTexture( GL_TEXTURE_CUBE_MAP, cubemapTextureValue );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glDepthFunc( GL_LESS ); // Setter dybdefunksjonen tilbake til default
        
        glUseProgram(0);
        glBindVertexArray(0);
    
    
/* * * * * * *
 *
 * Tegner kuben
 *
 * * * * * * */
    
    // Aktiverer programmet
    cubeShader.Use();
        
    // Henter og setter texture som sendes til cube-fragshader
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i(cubeTextureLoc , 0);
    glBindTexture( GL_TEXTURE_2D, cubeTextureValue );
    
    // Henter og setter normalMap som sendes til cube-fragshader
    glActiveTexture( GL_TEXTURE1 );
    glUniform1i(cubeNormalMapLoc , 1);
    glBindTexture( GL_TEXTURE_2D, cubeNormalMapValue );
    
    
    // Setter view matrisen
    glm::mat4 viewCubeValue = camera.GetViewMatrix();
    // Sender view-matrise til cube-shaderen:
    glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( viewCubeValue ) );

    // Setter model-matrise
    glm::mat4 modelCubeValue = glm::mat4(1.0f);
    //modelCubeValue = glm::rotate(modelCubeValue, time * 0.5f, glm::vec3(0.0f, 1.0f,  0.0f));
    // Sender model-matrise til cube-shaderen:
    glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( modelCubeValue ) );
    
    // Sender resten av lys-matrisene til cube-shaderen:
    
    glm::vec3 lightPositionValue(sinf(time * 1.0f), cosf(time * 2.0f), 0.8f);
    glUniform3f(lightPositionLoc, lightPositionValue.x, lightPositionValue.y, lightPositionValue.z);
    
    //glUniform3fv(lightPositionLoc, 1, lightPositionValue);
    glUniform3fv(viewPositionLoc, 1, cameraPositionValue);
    
    
    

    // Aktiverer vertex-arrayen for kuben:
    glBindVertexArray( cubeVAO );
    // Deretter tegnes trianglene:
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    glBindVertexArray(0);
    
    // Deaktiverer shaderprogram som brukes og vertexarray
    glUseProgram(0);
        
}

void resizeGL(int width, int height) {
    
    // Feilhåndtering for å hindre at det blir deling på 0
    if (height == 0)
        height = 1;
    
    cubeShader.Use();
    glm::mat4 projectionCubeValue = glm::perspective(3.14f/2.0f, (float)width/height, 0.1f, 100.0f);
    glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projectionCubeValue ) );
    
    skyboxShader.Use();
    glm::mat4 projectionSkyboxValue = glm::perspective(camera.GetZoom(), (float)width/height, 0.1f, 1000.0f );
    glUniformMatrix4fv( projLocSkybox, 1, GL_FALSE, glm::value_ptr( projectionSkyboxValue ) );
    
    // Definerer viewport-dimensjonene
    // Denne blir kalt hver gang vinduet starter.
    glViewport(0, 0, width, height); // 2.0
    
}

/*
 * Error callback function for GLFW
 */
static void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// Metode som kalles hver gang en tast presses ned eller slippes opp
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    
    if ( key >= 0 && key <= 1024 )
    {
        if ( GLFW_PRESS == action )
        {
            keys[key] = true;
        }
        else if ( GLFW_RELEASE == action)
        {
            keys[key] = false;
        }
    }
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

void DoMovement( )
{
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }
}

/*
 * Window size changed callback function for GLFW
 */
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
    
    resizeGL(width, height);
    
}


void generateCubeVerticesAndSetArraysAndBuffers()
{
    
    if (cubeVAO == 0)
    {
        

        // positions
        //glm::vec3 pos1(-1.0f,  1.0f, -1.0f);
        //glm::vec3 pos2(-1.0f, -1.0f, -1.0f);
        //glm::vec3 pos3( 1.0f, -1.0f, -1.0f);
        //glm::vec3 pos4( 1.0f,  1.0f, -1.0f);
        
        //glm::vec3 pos5(-1.0f,  1.0f, 1.0f);
        //glm::vec3 pos6(-1.0f, -1.0f, 1.0f);
        //glm::vec3 pos7( 1.0f, -1.0f, 1.0f);
        //glm::vec3 pos8( 1.0f,  1.0f, 1.0f);
        
        glm::vec3 pos1, pos2, pos3, pos4;
        glm::vec3 posTop1, posTop2, posTop3, posTop4, posBottom1, posBottom2, posBottom3, posBottom4, posLeft1, posLeft2, posLeft3, posLeft4, posRight1, posRight2, posRight3, posRight4, posBack1, posBack2, posBack3, posBack4, posFront1, posFront2, posFront3, posFront4;
        
        glm::vec3 nm;
        glm::vec3 nmTop, nmBottom, nmLeft, nmRight, nmBack, nmFront;
        
        glm::vec3 tangentTop1, tangentBottom1, tangentLeft1, tangentRight1, tangentBack1, tangentFront1;
        glm::vec3 tangentTop2, tangentBottom2, tangentLeft2, tangentRight2, tangentBack2, tangentFront2;
        
        glm::vec3 bitangentTop1, bitangentBottom1, bitangentLeft1, bitangentRight1, bitangentBack1, bitangentFront1;
        glm::vec3 bitangentTop2, bitangentBottom2, bitangentLeft2, bitangentRight2, bitangentBack2, bitangentFront2;
        
        GLfloat f;
        
        
        glm::vec3 edge1;
        glm::vec3 edge2;
        glm::vec2 deltaUV1;
        glm::vec2 deltaUV2;
        
        glm::vec3 positions[8];
        positions[0] = glm::vec3(-1.0f,  1.0f, -1.0f);
        positions[1] = glm::vec3(-1.0f, -1.0f, -1.0f);
        positions[2] = glm::vec3( 1.0f, -1.0f, -1.0f);
        positions[3] = glm::vec3( 1.0f,  1.0f, -1.0f);
        
        positions[4] = glm::vec3(-1.0f,  1.0f, 1.0f);
        positions[5] = glm::vec3(-1.0f, -1.0f, 1.0f);
        positions[6] = glm::vec3( 1.0f, -1.0f, 1.0f);
        positions[7] = glm::vec3( 1.0f,  1.0f, 1.0f);
        
        
        // texture coordinates  // Kan gjenbrukes som før
        glm::vec2 uv1(0.0f, 1.0f);
        glm::vec2 uv2(0.0f, 0.0f);
        glm::vec2 uv3(1.0f, 0.0f);
        glm::vec2 uv4(1.0f, 1.0f);
        
        // normal vector   // En til hver side
        //glm::vec3 nm(0.0f, 0.0f, 1.0f);
        //glm::vec3 nm2(0.0f, 0.0f, -1.0f);
        //glm::vec3 nm3(-1.0f, 0.0f, 0.0f);
        //glm::vec3 nm4(1.0f, 0.0f, 0.0f);
        //glm::vec3 nm5(0.0f, -1.0f, 0.0f);
        //glm::vec3 nm6(0.0f, 1.0f, 0.0f);
        
        glm::vec3 normals[8];
         normals[0] = glm::vec3( 0.0f, 0.0f, 1.0f);
         normals[1] = glm::vec3( 0.0f, 0.0f,-1.0f);
         normals[2] = glm::vec3(-1.0f, 0.0f, 0.0f);
         normals[3] = glm::vec3( 1.0f, 0.0f, 0.0f);
         normals[4] = glm::vec3( 0.0f,-1.0f, 0.0f);
         normals[5] = glm::vec3( 0.0f, 1.0f, 0.0f);

                                
        // Sette på alle kantene i riktig rekkefølge!
        int indices[] = {0,1,2,3,1,5,6,2,4,5,1,0,4,5,6,7,3,2,6,7,0,4,7,3};
        
        int sideCounter = 0;
        
        
        int face = 0;
         
        for (face = 0; face < 24; face = face + 4){ // Kjører løkke 4 om gangen. ($ punkter per side!)
            
            pos1 = positions[indices[face]];
            pos2 = positions[indices[face + 1]];
            pos3 = positions[indices[face + 2]];
            pos4 = positions[indices[face + 3]];
            
            
            nm = normals[sideCounter];
            sideCounter++;
            
            switch (sideCounter)
            {
                case 1: // code to be executed if n = 1;
                    
                    
                    printf("case 1", 2);
                    
                    posTop1 = pos1;
                    posTop2 = pos2;
                    posTop3 = pos3;
                    posTop4 = pos4;
                    
                    nmTop = nm;

                    // triangle 1
                    // ----------
                    // Kalkulerer kanter og UV-koordinater til den første trekanten
                    edge1 = posTop2 - posTop1;
                    edge2 = posTop3 - posTop1;
                    deltaUV1 = uv2 - uv1;
                    deltaUV2 = uv3 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentTop1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentTop1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentTop1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentTop1 = glm::normalize(tangentTop1);

                    bitangentTop1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentTop1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentTop1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentTop1 = glm::normalize(bitangentTop1);

                    // triangle 2
                    // ----------
                    edge1 = posTop3 - posTop1;
                    edge2 = posTop4 - posTop1;
                    deltaUV1 = uv3 - uv1;
                    deltaUV2 = uv4 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentTop2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentTop2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentTop2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentTop2 = glm::normalize(tangentTop2);


                    bitangentTop2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentTop2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentTop2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentTop2 = glm::normalize(bitangentTop2);
                    
                    break;
                    
                case 2:

                    printf("case 2", 2);
                    
                    posBottom1 = pos1;
                    posBottom2 = pos2;
                    posBottom3 = pos3;
                    posBottom4 = pos4;
                    
                    nmBottom = nm;
                    
                    edge1 = posBottom2 - posBottom1;
                    edge2 = posBottom3 - posBottom1;
                    deltaUV1 = uv2 - uv1;
                    deltaUV2 = uv3 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentBottom1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentBottom1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentBottom1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentBottom1 = glm::normalize(tangentBottom1);

                    bitangentBottom1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentBottom1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentBottom1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentBottom1 = glm::normalize(bitangentBottom1);

                    // triangle 2
                    // ----------
                    edge1 = posBottom3 - posBottom1;
                    edge2 = posBottom4 - posBottom1;
                    deltaUV1 = uv3 - uv1;
                    deltaUV2 = uv4 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentBottom2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentBottom2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentBottom2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentBottom2 = glm::normalize(tangentBottom2);


                    bitangentBottom2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentBottom2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentBottom2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentBottom2 = glm::normalize(bitangentBottom2);
                    
                    break;
                    
                case 3:
                    
                    printf("case 3", 2);
                    
                    posLeft1 = pos1;
                    posLeft2 = pos2;
                    posLeft3 = pos3;
                    posLeft4 = pos4;
                    
                    nmLeft = nm;
                    
                    edge1 = posLeft2 - posLeft1;
                    edge2 = posLeft3 - posLeft1;
                    deltaUV1 = uv2 - uv1;
                    deltaUV2 = uv3 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentLeft1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentLeft1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentLeft1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentLeft1 = glm::normalize(tangentLeft1);

                    bitangentLeft1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentLeft1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentLeft1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentLeft1 = glm::normalize(bitangentLeft1);

                    // triangle 2
                    // ----------
                    edge1 = posLeft3 - posLeft1;
                    edge2 = posLeft4 - posLeft1;
                    deltaUV1 = uv3 - uv1;
                    deltaUV2 = uv4 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentLeft2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentLeft2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentLeft2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentLeft2 = glm::normalize(tangentLeft2);


                    bitangentLeft2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentLeft2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentLeft2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentLeft2 = glm::normalize(bitangentLeft2);
                    
                    break;
                    
                case 4:
                    
                    printf("case 4", 2);
                    
                    posRight1 = pos1;
                    posRight2 = pos2;
                    posRight3 = pos3;
                    posRight4 = pos4;
                    
                    nmRight = nm;
                    
                    edge1 = posRight2 - posRight1;
                    edge2 = posRight3 - posRight1;
                    deltaUV1 = uv2 - uv1;
                    deltaUV2 = uv3 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentRight1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentRight1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentRight1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentRight1 = glm::normalize(tangentRight1);

                    bitangentRight1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentRight1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentRight1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentRight1 = glm::normalize(bitangentRight1);

                    // triangle 2
                    // ----------
                    edge1 = posRight3 - posRight1;
                    edge2 = posRight4 - posRight1;
                    deltaUV1 = uv3 - uv1;
                    deltaUV2 = uv4 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentRight2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentRight2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentRight2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentRight2 = glm::normalize(tangentRight2);


                    bitangentRight2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentRight2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentRight2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentRight2 = glm::normalize(bitangentRight2);
                    
                    break;
                    

                    
                case 5:
                    
                    printf("case 5", 2);
                    
                    posBack1 = pos1;
                    posBack2 = pos2;
                    posBack3 = pos3;
                    posBack4 = pos4;
                    
                    nmBack = nm;
                    
                    edge1 = posBack2 - posBack1;
                    edge2 = posBack3 - posBack1;
                    deltaUV1 = uv2 - uv1;
                    deltaUV2 = uv3 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentBack1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentBack1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentBack1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentBack1 = glm::normalize(tangentBack1);

                    bitangentBack1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentBack1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentBack1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentBack1 = glm::normalize(bitangentBack1);

                    // triangle 2
                    // ----------
                    edge1 = posBack3 - posBack1;
                    edge2 = posBack4 - posBack1;
                    deltaUV1 = uv3 - uv1;
                    deltaUV2 = uv4 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentBack2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentBack2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentBack2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentBack2 = glm::normalize(tangentBack2);


                    bitangentBack2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentBack2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentBack2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentBack2 = glm::normalize(bitangentBack2);
                    
                    break;
                    
                case 6:
                    
                    printf("case 6", 2);
                    
                    posFront1 = pos1;
                    posFront2 = pos2;
                    posFront3 = pos3;
                    posFront4 = pos4;
                    
                    nmFront = nm;
                    
                    edge1 = posFront2 - posFront1;
                    edge2 = posFront3 - posFront1;
                    deltaUV1 = uv2 - uv1;
                    deltaUV2 = uv3 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentFront1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentFront1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentFront1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentFront1 = glm::normalize(tangentFront1);

                    bitangentFront1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentFront1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentFront1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentFront1 = glm::normalize(bitangentFront1);

                    // triangle 2
                    // ----------
                    edge1 = posFront3 - posFront1;
                    edge2 = posFront4 - posFront1;
                    deltaUV1 = uv3 - uv1;
                    deltaUV2 = uv4 - uv1;

                    f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

                    tangentFront2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
                    tangentFront2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
                    tangentFront2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
                    tangentFront2 = glm::normalize(tangentFront2);


                    bitangentFront2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
                    bitangentFront2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
                    bitangentFront2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
                    bitangentFront2 = glm::normalize(bitangentFront2);
                    
                    break;
                    
                default: // code to be executed if n doesn't match any cases
                    printf("Hei", 2);
            }
            

            
        }
        
        
        GLfloat cubeVertices[] = {
            
            
            posLeft1.x, posLeft1.y, posLeft1.z, nmLeft.x, nmLeft.y, nmLeft.z, uv1.x, uv1.y, tangentLeft1.x, tangentLeft1.y, tangentLeft1.z, bitangentLeft1.x, bitangentLeft1.y, bitangentLeft1.z,
              posLeft2.x, posLeft2.y, posLeft2.z, nmLeft.x, nmLeft.y, nmLeft.z, uv2.x, uv2.y, tangentLeft1.x, tangentLeft1.y, tangentLeft1.z, bitangentLeft1.x, bitangentLeft1.y, bitangentLeft1.z,
              posLeft3.x, posLeft3.y, posLeft3.z, nmLeft.x, nmLeft.y, nm.z, uv3.x, uv3.y, tangentLeft1.x, tangentLeft1.y, tangentLeft1.z, bitangentLeft1.x, bitangentLeft1.y, bitangentLeft1.z,

              posLeft1.x, posLeft1.y, posLeft1.z, nmLeft.x, nmLeft.y, nmLeft.z, uv1.x, uv1.y, tangentLeft2.x, tangentLeft2.y, tangentLeft2.z, bitangentLeft2.x, bitangentLeft2.y, bitangentLeft2.z,
              posLeft3.x, posLeft3.y, posLeft3.z, nmLeft.x, nmLeft.y, nmLeft.z, uv3.x, uv3.y, tangentLeft2.x, tangentLeft2.y, tangentLeft2.z, bitangentLeft2.x, bitangentLeft2.y, bitangentLeft2.z,
              posLeft4.x, posLeft4.y, posLeft4.z, nmLeft.x, nmLeft.y, nmLeft.z, uv4.x, uv4.y, tangentLeft2.x, tangentLeft2.y, tangentLeft2.z, bitangentLeft2.x, bitangentLeft2.y, bitangentLeft2.z,
            
            posBottom1.x, posBottom1.y, posBottom1.z, nmBottom.x, nmBottom.y, nmBottom.z, uv1.x, uv1.y, tangentBottom1.x, tangentBottom1.y, tangentBottom1.z, bitangentBottom1.x, bitangentBottom1.y, bitangentBottom1.z,
             posBottom2.x, posBottom2.y, posBottom2.z, nmBottom.x, nmBottom.y, nmBottom.z, uv2.x, uv2.y, tangentBottom1.x, tangentBottom1.y, tangentBottom1.z, bitangentBottom1.x, bitangentBottom1.y, bitangentBottom1.z,
             posBottom3.x, posBottom3.y, posBottom3.z, nmBottom.x, nmBottom.y, nm.z, uv3.x, uv3.y, tangentBottom1.x, tangentBottom1.y, tangentBottom1.z, bitangentBottom1.x, bitangentBottom1.y, bitangentBottom1.z,

             posBottom1.x, posBottom1.y, posBottom1.z, nmBottom.x, nmBottom.y, nmBottom.z, uv1.x, uv1.y, tangentBottom2.x, tangentBottom2.y, tangentBottom2.z, bitangentBottom2.x, bitangentBottom2.y, bitangentBottom2.z,
             posBottom3.x, posBottom3.y, posBottom3.z, nmBottom.x, nmBottom.y, nmBottom.z, uv3.x, uv3.y, tangentBottom2.x, tangentBottom2.y, tangentBottom2.z, bitangentBottom2.x, bitangentBottom2.y, bitangentBottom2.z,
             posBottom4.x, posBottom4.y, posBottom4.z, nmBottom.x, nmBottom.y, nmBottom.z, uv4.x, uv4.y, tangentBottom2.x, tangentBottom2.y, tangentBottom2.z, bitangentBottom2.x, bitangentBottom2.y, bitangentBottom2.z,
            
            
            // positions            // normal         // texcoords  // tangent                          // bitangent
            posTop1.x, posTop1.y, posTop1.z, nmTop.x, nmTop.y, nmTop.z, uv1.x, uv1.y, tangentTop1.x, tangentTop1.y, tangentTop1.z, bitangentTop1.x, bitangentTop1.y, bitangentTop1.z,
            posTop2.x, posTop2.y, posTop2.z, nmTop.x, nmTop.y, nmTop.z, uv2.x, uv2.y, tangentTop1.x, tangentTop1.y, tangentTop1.z, bitangentTop1.x, bitangentTop1.y, bitangentTop1.z,
            posTop3.x, posTop3.y, posTop3.z, nmTop.x, nmTop.y, nm.z, uv3.x, uv3.y, tangentTop1.x, tangentTop1.y, tangentTop1.z, bitangentTop1.x, bitangentTop1.y, bitangentTop1.z,

            posTop1.x, posTop1.y, posTop1.z, nmTop.x, nmTop.y, nmTop.z, uv1.x, uv1.y, tangentTop2.x, tangentTop2.y, tangentTop2.z, bitangentTop2.x, bitangentTop2.y, bitangentTop2.z,
            posTop3.x, posTop3.y, posTop3.z, nmTop.x, nmTop.y, nmTop.z, uv3.x, uv3.y, tangentTop2.x, tangentTop2.y, tangentTop2.z, bitangentTop2.x, bitangentTop2.y, bitangentTop2.z,
            posTop4.x, posTop4.y, posTop4.z, nmTop.x, nmTop.y, nmTop.z, uv4.x, uv4.y, tangentTop2.x, tangentTop2.y, tangentTop2.z, bitangentTop2.x, bitangentTop2.y, bitangentTop2.z,
            
            
            

             
            
            
 
             
            
            
            posRight1.x, posRight1.y, posRight1.z, nmRight.x, nmRight.y, nmRight.z, uv1.x, uv1.y, tangentRight1.x, tangentRight1.y, tangentRight1.z, bitangentRight1.x, bitangentRight1.y, bitangentRight1.z,
             posRight2.x, posRight2.y, posRight2.z, nmRight.x, nmRight.y, nmRight.z, uv2.x, uv2.y, tangentRight1.x, tangentRight1.y, tangentRight1.z, bitangentRight1.x, bitangentRight1.y, bitangentRight1.z,
             posRight3.x, posRight3.y, posRight3.z, nmRight.x, nmRight.y, nm.z, uv3.x, uv3.y, tangentRight1.x, tangentRight1.y, tangentRight1.z, bitangentRight1.x, bitangentRight1.y, bitangentRight1.z,

             posRight1.x, posRight1.y, posRight1.z, nmRight.x, nmRight.y, nmRight.z, uv1.x, uv1.y, tangentRight2.x, tangentRight2.y, tangentRight2.z, bitangentRight2.x, bitangentRight2.y, bitangentRight2.z,
             posRight3.x, posRight3.y, posRight3.z, nmRight.x, nmRight.y, nmRight.z, uv3.x, uv3.y, tangentRight2.x, tangentRight2.y, tangentRight2.z, bitangentRight2.x, bitangentRight2.y, bitangentRight2.z,
             posRight4.x, posRight4.y, posRight4.z, nmRight.x, nmRight.y, nmRight.z, uv4.x, uv4.y, tangentRight2.x, tangentRight2.y, tangentRight2.z, bitangentRight2.x, bitangentRight2.y, bitangentRight2.z,
             
            
            
            
            posBack1.x, posBack1.y, posBack1.z, nmBack.x, nmBack.y, nmBack.z, uv1.x, uv1.y, tangentBack1.x, tangentBack1.y, tangentBack1.z, bitangentBack1.x, bitangentBack1.y, bitangentBack1.z,
             posBack2.x, posBack2.y, posBack2.z, nmBack.x, nmBack.y, nmBack.z, uv2.x, uv2.y, tangentBack1.x, tangentBack1.y, tangentBack1.z, bitangentBack1.x, bitangentBack1.y, bitangentBack1.z,
             posBack3.x, posBack3.y, posBack3.z, nmBack.x, nmBack.y, nm.z, uv3.x, uv3.y, tangentBack1.x, tangentBack1.y, tangentBack1.z, bitangentBack1.x, bitangentBack1.y, bitangentBack1.z,

             posBack1.x, posBack1.y, posBack1.z, nmBack.x, nmBack.y, nmBack.z, uv1.x, uv1.y, tangentBack2.x, tangentBack2.y, tangentBack2.z, bitangentBack2.x, bitangentBack2.y, bitangentBack2.z,
             posBack3.x, posBack3.y, posBack3.z, nmBack.x, nmBack.y, nmBack.z, uv3.x, uv3.y, tangentBack2.x, tangentBack2.y, tangentBack2.z, bitangentBack2.x, bitangentBack2.y, bitangentBack2.z,
             posBack4.x, posBack4.y, posBack4.z, nmBack.x, nmBack.y, nmBack.z, uv4.x, uv4.y, tangentBack2.x, tangentBack2.y, tangentBack2.z, bitangentBack2.x, bitangentBack2.y, bitangentBack2.z,
             
            
            
            posFront1.x, posFront1.y, posFront1.z, nmFront.x, nmFront.y, nmFront.z, uv1.x, uv1.y, tangentFront1.x, tangentFront1.y, tangentFront1.z, bitangentFront1.x, bitangentFront1.y, bitangentFront1.z,
             posFront2.x, posFront2.y, posFront2.z, nmFront.x, nmFront.y, nmFront.z, uv2.x, uv2.y, tangentFront1.x, tangentFront1.y, tangentFront1.z, bitangentFront1.x, bitangentFront1.y, bitangentFront1.z,
             posFront3.x, posFront3.y, posFront3.z, nmFront.x, nmFront.y, nm.z, uv3.x, uv3.y, tangentFront1.x, tangentFront1.y, tangentFront1.z, bitangentFront1.x, bitangentFront1.y, bitangentFront1.z,

             posFront1.x, posFront1.y, posFront1.z, nmFront.x, nmFront.y, nmFront.z, uv1.x, uv1.y, tangentFront2.x, tangentFront2.y, tangentFront2.z, bitangentFront2.x, bitangentFront2.y, bitangentFront2.z,
             posFront3.x, posFront3.y, posFront3.z, nmFront.x, nmFront.y, nmFront.z, uv3.x, uv3.y, tangentFront2.x, tangentFront2.y, tangentFront2.z, bitangentFront2.x, bitangentFront2.y, bitangentFront2.z,
             posFront4.x, posFront4.y, posFront4.z, nmFront.x, nmFront.y, nmFront.z, uv4.x, uv4.y, tangentFront2.x, tangentFront2.y, tangentFront2.z, bitangentFront2.x, bitangentFront2.y, bitangentFront2.z
             
            
            
            
            
        };
        
    

         
        
         //Antall man ønsker å opprette, arrayen som skal benyttes.
         glGenVertexArrays( 1, &cubeVAO );
         //Forteller OpenGL hvilken vertex-array som skal brukes.
         glBindVertexArray( cubeVAO );
         
         //Antall man ønsker å opprette, arrayen som skal benyttes.
         glGenBuffers( 1, &cubeVBO );
         //Forteller OpenGL at dette er current bufferen som skal brukes (Skal bufferen modifiseres senere er det denne som skal endres)
         glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
         

         glBufferData( GL_ARRAY_BUFFER, 6 * 14 * 6 * sizeof( GL_FLOAT ), cubeVertices, GL_STATIC_DRAW );
         
         glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (GLvoid*)0);
         glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
         glVertexAttribPointer(COLOR, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (const void*)(6 * sizeof(GLfloat)));
         glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (const void*)(8 * sizeof(GLfloat)));
         glVertexAttribPointer(BITANGENT, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GL_FLOAT), (const void*)(11 * sizeof(GLfloat)));
         
         // Aktivere attributtene
         glEnableVertexAttribArray(POSITION);
         glEnableVertexAttribArray(COLOR);
         glEnableVertexAttribArray(NORMAL);
         glEnableVertexAttribArray(TANGENT);
         glEnableVertexAttribArray(BITANGENT);
   
    }
}

void generateSkyBoxVerticesAndSetArraysAndBuffers() {
    
    
    if (cubeVAO == 0)
    {
        
        GLfloat skyboxVertices[] =
        {
            // Posisjon
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
         };
        
        glGenVertexArrays( 1, &skyboxVAO );
        glGenBuffers( 1, &skyboxVBO );
        glBindVertexArray( skyboxVAO );
        glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
        
        //Fyller bufferen med data: Bufferen som skal brukes, størrelsen den på holde av, de vertices som skal lagres, og info at det skal tegnes.
        glBufferData( GL_ARRAY_BUFFER, 108 * sizeof( GL_FLOAT ), skyboxVertices, GL_STATIC_DRAW );
        
        // Posisjon attribute
        glVertexAttribPointer( POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
        
        // Aktivere attributten
        glEnableVertexAttribArray(POSITION);
    }
}


/*
 * PROGRAMSTART
 */
int main(void) {
    
    glfwSetErrorCallback(glfwErrorCallback);
    
    // Initialiserer GLWF og sjekker at det gikk OK
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    
       // GLFW benytter noe kalt windowHint. Disse forteller/spør GLFW om å sette visse versjoner
    // av OpenGL + andre instillinger. - Dette er valgfritt, men Mac trenger noen!
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); //Ber her om OpenGL versjon 3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_TRUE );

    // Opprette et GLFW-vindu + sjekker at det gikk ok
    GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Datagrafikk 2019 - Prosjekt - Mette Strand Hornnes", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Setter callback-funksjoner som kalles om en tast er trykket ned, eller mus beveget på seg.
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // TODO: Trengs denne?
    // glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // TODO: Trengs denne?
    // glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Setter endringer av vinduestørrelse
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    
    // Velger at det er dette vinduet OpenGL skal jobbe i
    glfwMakeContextCurrent(window);
    
    // TODO: Trengs denne?
    // glewExperimental = GL_TRUE;
    
    // Initialiserer GLEW og sjekker at det gikk ok
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Sørge for at GLFW bytter buffere med en gang
    glfwSwapInterval(0);
    
    // Initialize OpenGL
    if (!initGL()) {
        printf("Failed to initialize OpenGL\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Setter opp OpenGL-viewet
    resizeGL(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    
    // Run a loop until the window is closed
    while (!glfwWindowShouldClose(window)) {
        
        // Setter frame time (For å sikre smoothe bevegesler)
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Draw OpenGL screne
        drawGLScene();
        
        // Swap buffers
        glfwSwapBuffers(window);
        
        // Sjekker om noen eventer er aktivert (F.eks key trykker, musepeker flytter osv.)
        glfwPollEvents( );
        // Kaller deretter den tilhørende responsfunksjonen.
        //DoMovement();
        
    }
    
    // Shutdown GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    
    // Exit
    exit(EXIT_SUCCESS);
    
}
