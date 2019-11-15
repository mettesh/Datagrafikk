// Forsøkt å sette sammen simpleLighting og Skybox-kode --> På vei?
// Hva er feilmeldingen
// Hvordan gjøre shader-variablene globale?



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
#include "Camera.h"
#include "Texture.h"

#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768

// Vertex Buffer Identifiers
#define VERTICES 0

// Vertex Array attributes
#define POSITION 0
#define COLOR 1
//#define NORMAL 2


// GLSL Uniform indices
#define TRANSFORM0 0
#define TRANSFORM1 1
#define LIGHT 2
#define MATERIAL 3
#define CAMERA 4

// KUBE:
GLfloat cubeVertices[] =
{
// Posisjoner        // Texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
             
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
             
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
         
// SKYBOX
GLfloat skyboxVertices[] = {
    // Posisjoner
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

// Dimensjonene til vinduet
// const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// For tastatur- og musebevegelser
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

Shader cubeShader;
Shader skyboxShader;

// Kamera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
GLfloat lastX = SCREEN_WIDTH / 2.0f;
GLfloat lastY = SCREEN_WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



// Uniforms values
//GLfloat lightPosition[] { 0.0f, 0.0f, 4.0f };
glm::vec3 lightPosition( 2.2f, 1.0f, 2.0f );

GLfloat lightAmbient[] { 0.1f, 0.1f, 0.2f };
GLfloat lightDiffuse[] { 0.5f, 0.5f, 0.5f };
GLfloat lightSpecular[] { 0.6f, 0.6f, 0.6f };
GLfloat materialShininessColor[] { 1.0f, 1.0f, 1.0f,  1.0f };
GLfloat materialShininess = 32.0f;
GLfloat cameraPosition[] { 0.0f, 0.0f, 4.0f };

// Cube Uniform locations
GLint modelLoc;
GLint viewLoc;
GLint projLoc;
GLint lightPositionPos;
GLint lightAmbientPos;
GLint lightDiffusePos;
GLint lightSpecularPos;
GLint materialShininessColorPos;
GLint materialShininessPos;
GLint cameraPositionPos;

// Skybox Uniform locations
GLint projLocSkybox;
GLint viewLocSkybox;

GLuint cubeVAO;
GLuint skyboxVAO;

GLuint cubemapTexture;
GLuint cubeTexture;


/*
 * Initialize OpenGL
 */
int initGL() {
    
// Oppretter Vertex-array og buffer -object for kuben
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays( 1, &cubeVAO ); //Antall man ønsker å opprette, arrayen som skal benyttes.
    glGenBuffers( 1, &cubeVBO );
    //Forteller OpenGL hvilken vertex-array som skal brukes.
    glBindVertexArray( cubeVAO );
    //Forteller OpenGL at dette er current bufferen som skal brukes (Skal bufferen modifiseres senere er det denne som skal endres)
    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    //Fyller bufferen med data: Bufferen som skal brukes, størrelsen den på holde av, de vertices som skal lagres, og info at det skal tegnes.
    glBufferData( GL_ARRAY_BUFFER, 180 * sizeof( GL_FLOAT ), cubeVertices, GL_STATIC_DRAW );
    
    // Posisjon attribute
    glVertexAttribPointer( POSITION, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    
    // Texture attribute
    glVertexAttribPointer( COLOR, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
   
    glEnableVertexAttribArray(POSITION);
    glEnableVertexAttribArray(COLOR);

   // Disable the vertexArrayen
    glBindVertexArray( 0 );

    
// Oppretter Vertex-array og buffer -object for skyboxen
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays( 1, &skyboxVAO );
    glGenBuffers( 1, &skyboxVBO );
    glBindVertexArray( skyboxVAO );
    glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
    
    // Posisjon attribute
    glBufferData( GL_ARRAY_BUFFER, sizeof( skyboxVertices ), &skyboxVertices, GL_STATIC_DRAW );
    glVertexAttribPointer( POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    
    glEnableVertexAttribArray(POSITION);

    // Disable the vertexArrayen
    glBindVertexArray(0);
    
    // TODO: Hvordan gjøre disse globale??
    // Setup and compile our shaders
    cubeShader = Shader( "resources/shaders/cube.vert", "resources/shaders/cube.frag" );
    skyboxShader = Shader( "resources/shaders/skybox.vert", "resources/shaders/skybox.frag" );

    //Laste inn texture til kuben:
    GLuint cubeTexture = TextureLoading::LoadTexture("resources/img/cube/texture.png");
    
    //Laste inn texture til skyboxen:
    std::vector<const GLchar*> faces;
    faces.push_back( "resources/img/skybox/iceflats_lf.tga" );
    faces.push_back( "resources/img/skybox/iceflats_rt.tga" );
    faces.push_back( "resources/img/skybox/iceflats_up.tga" );
    faces.push_back( "resources/img/skybox/iceflats_dn.tga" );
    faces.push_back( "resources/img/skybox/iceflats_ft.tga" );
    faces.push_back( "resources/img/skybox/iceflats_bk.tga" );
    cubemapTexture = TextureLoading::LoadCubemap( faces );


    // Henter inn uniform-loactions fra cube-shadere
    cubeShader.Use();
    viewLoc = glGetUniformLocation( cubeShader.Program, "view" );
    projLoc = glGetUniformLocation( cubeShader.Program, "projection" );
    modelLoc = glGetUniformLocation( cubeShader.Program, "model" );
    
    lightPositionPos = glGetUniformLocation(cubeShader.Program, "lightPosition");
    lightAmbientPos = glGetUniformLocation(cubeShader.Program, "lightAmbient");
    lightDiffusePos = glGetUniformLocation(cubeShader.Program, "lightDiffuse");
    lightSpecularPos = glGetUniformLocation(cubeShader.Program, "lightSpecular");
    materialShininessColorPos = glGetUniformLocation(cubeShader.Program, "shininessColor");
    materialShininessPos = glGetUniformLocation(cubeShader.Program, "shininess");
    cameraPositionPos = glGetUniformLocation(cubeShader.Program, "cameraPosition");
    
    // Henter inn uniform-loactions fra skybox-shadere
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
    
    // Aktiverer programmet
    //glUseProgram(cubeShader.Program);
    
       
/* * * * * * *
 *
 * Tegner kuben og lys
 *
 * * * * * * */
    cubeShader.Use();
        
    // Binder Textures ved å bruke texture units:
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, cubeTexture );
    glUniform1i( glGetUniformLocation( cubeShader.Program, "texture1" ), 0 );


    // Setter view matrisen
    glm::mat4 view = camera.GetViewMatrix();
    // Henter inn uniform location
    //GLint viewLoc = glGetUniformLocation( shader.Program, "view" );
    // Sender matrisene til shaderen:
    glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );


    // Setter model matrisen
    glm::mat4 model = glm::mat4(1.0);
    //model = glm::rotate(model, /*(float)glfwGetTime() **/0.5f, glm::vec3(0.0f, 1.0f,  0.0f));
    // Henter inn uniform location
    // GLint modelLoc = glGetUniformLocation( shader.Program, "model" );
    // Kalkulerer modelmatrisen for hvert objekt og sender den til shaderen
    glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );

    
    // Sette lysets posisjon:
    glm::vec3 lightPosition(sinf(glfwGetTime() * 1.0f), cosf(glfwGetTime() * 2.0f), 0.8f);
    glUniform3f(lightPositionPos, lightPosition.x, lightPosition.y, lightPosition.z);
    
    glUniform3f(lightAmbientPos, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
    glUniform3fv(lightDiffusePos, 1, lightDiffuse);
    glUniform3fv(lightSpecularPos, 1, lightSpecular);
    glUniform4fv(materialShininessColorPos, 1, materialShininessColor);
    glUniform1f(materialShininessPos, materialShininess);
    glUniform3fv(cameraPositionPos, 1, cameraPosition);
    
    // Aktiverer vertex-arrayen for kuben:
    glBindVertexArray( cubeVAO );

    
    // Deretter tegnes trianglene:
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    
    // Draw the vertex array
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferNames[INDICES]);
    //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    
    
    // Set the remaining uniforms
    
    
    glBindVertexArray(0);

    
/* * * * * * *
*
* Tegner skyboxen:
*
* * * * * * */

    skyboxShader.Use();
    
    // Change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc( GL_LEQUAL );
    

    glm::mat4 viewSkybox = camera.GetViewMatrix();
    // GLint viewLocSkybox = glGetUniformLocation( skyboxShader.Program, "view" );
    // Sender matrisen til shaderen:
    glUniformMatrix4fv( viewLocSkybox, 1, GL_FALSE, glm::value_ptr( viewSkybox ) );
    
    
    

    // Aktiverer vertex-arrayen for skyBox:
    glBindVertexArray( skyboxVAO );
    
    glBindTexture( GL_TEXTURE_CUBE_MAP, cubemapTexture );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glDepthFunc( GL_LESS ); // Setter dybdefunksjonen tilbake til default
    
    // Disable
    glUseProgram(0);
    glBindVertexArray(0);
    
}

void resizeGL(int width, int height) {
    
    
    cubeShader.Use();
    
    // Prevent division by zero
    if (height == 0)
        height = 1;
    
    // Change the projection matrix
    glm::mat4 projection = glm::perspective(3.14f/2.0f, (float)width/height, 0.1f, 100.0f);
    glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
    
    
    skyboxShader.Use();
    
    glm::mat4 projectionSkybox = glm::perspective( camera.GetZoom(), ( float )width / ( float )height, 0.1f, 1000.0f );
    // Henter inn uniform location:
    // GLint projLocSkybox = glGetUniformLocation( skyboxShader.Program, "projection" );
    // Sender matrisen til shaderen:
    glUniformMatrix4fv( projLocSkybox, 1, GL_FALSE, glm::value_ptr( projectionSkybox ) );
    
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
        glfwSetWindowShouldClose(window, GL_TRUE);
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

/*
 * Window size changed callback function for GLFW
 */
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
    
    resizeGL(width, height);
    
}

/*
 * PROGRAMSTART
 */
int main(void) {
    
    // Set error callback
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
    GLFWwindow* window = glfwCreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Minimal", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Setter callback-funksjoner som kalles om en tast er trykket ned, eller mus beveget på seg.
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // Setter endringer av vinduestørrelse
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    
    // Velger at det er dette vinduet OpenGL skal jobbe i
    glfwMakeContextCurrent(window);
    
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
