#include <string>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SOIL2.h"

// Egne includes
#include "shader.h"
#include "Camera.h"
#include "Model.h"

#include "Texture.h"

// Dimensjonene til vinduet
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// For tastatur- og musebevegelser
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

// Kamera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = WIDTH / 2.0f;
bool keys[1024];
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Lysets posisjon i universet:
glm::vec3 lightPos( 1.2f, 1.0f, 2.0f );

int main(){
    

// INITIALISERING OG KLARGJØRING AV BIBLIOTEK + OPPSETT AV VINDU

    // Initialiserer GLWF og sjekker at det gikk OK
    if(glfwInit() == false){
        fprintf(stderr, "GLWF failed to initialise.");
        return -1;
    }

    
    // GLFW benytter noe kalt windowHint. Disse forteller/spør GLFW om å sette visse versjoner
    // av OpenGL + andre instillinger. - Dette er valgfritt, men Mac trenger noen!
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); //Ber her om OpenGL versjon 3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
       

    // Opprette et GLFW-vindu som vi kan benytte til funksjonene
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Datagrafikkprosjekt - Mette Strand Hornnes", nullptr, nullptr );
    
    // Sjekker at vinduet ble opprettet
    if ( nullptr == window ){
        fprintf(stderr, "Failed to create GLFW window");
        glfwTerminate( );
        return -1;
    }
    // Om alt ok velges det at det er dette vinduet OpenGL skal jobbe i
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Setter callback-funksjoner
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW-oppsett
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Denne settes til true for å gi beskjed til GLFW at det skal brukes den moderne måten å håndtere pekere.
    glewExperimental = GL_TRUE;
    
    // Initialiserer GLEW og sjekker at det gikk ok
    if ( GLEW_OK != glewInit( ) ){
        fprintf(stderr, "Failed to initialize GLEW");
        glfwTerminate();
        return -1;
    }
    
    // Definerer viewport-dimensjonene
    // Denne blir kalt hver gang vinduet starter.
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    
// Implementerer Depth i applikasjonen
    glEnable( GL_DEPTH_TEST );
    
    
    // Setup and compile our shaders
    Shader shader( "resources/shaders/cube.vert", "resources/shaders/cube.frag" );
    Shader skyboxShader( "resources/shaders/skybox.vert", "resources/shaders/skybox.frag" );
    

// SPESIFISERER KUBEN:

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
    
    
    
// Oppretter Vertex-array og buffer -object for kuben
    GLuint cubeVAO, cubeVBO;
    glGenVertexArrays( 1, &cubeVAO ); //Antall man ønsker å opprette, arrayen som skal benyttes.
    glGenBuffers( 1, &cubeVBO );
    //Forteller OpenGL hvilken vertex-array som skal brukes.
    glBindVertexArray( cubeVAO );
    //Forteller OpenGL at dette er current bufferen som skal brukes (Skal bufferen modifiseres senere er det denne som skal endres)
    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    //Fyller bufferen med data: Bufferen som skal brukes, størrelsen den på holde av, de vertices som skal lagres, og info at det skal tegnes.
    glBufferData( GL_ARRAY_BUFFER, sizeof( cubeVertices ), &cubeVertices, GL_STATIC_DRAW );
    
    // Posisjon attribute
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    
    // Texture attribute
    glEnableVertexAttribArray( 1 );
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
   
    glBindVertexArray( 0 );
    
    
// Oppretter Vertex-array og buffer -object for skyboxen
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays( 1, &skyboxVAO );
    glGenBuffers( 1, &skyboxVBO );
    glBindVertexArray( skyboxVAO );
    glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
    
    // Posisjon attribute
    glBufferData( GL_ARRAY_BUFFER, sizeof( skyboxVertices ), &skyboxVertices, GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    glBindVertexArray(0);
    

    //Laste inn texture til kuben:
    GLuint cubeTexture = TextureLoading::LoadTexture("resources/img/cube/texture.png");

    //Laste inn texture til skyboxen:
    vector<const GLchar*> faces;
    faces.push_back( "resources/img/skybox/iceflats_lf.tga" );
    faces.push_back( "resources/img/skybox/iceflats_rt.tga" );
    faces.push_back( "resources/img/skybox/iceflats_up.tga" );
    faces.push_back( "resources/img/skybox/iceflats_dn.tga" );
    faces.push_back( "resources/img/skybox/iceflats_ft.tga" );
    faces.push_back( "resources/img/skybox/iceflats_bk.tga" );
    GLuint cubemapTexture = TextureLoading::LoadCubemap( faces );

    glm::mat4 projection = glm::perspective( camera.GetZoom(), ( float )SCREEN_WIDTH / ( float )SCREEN_HEIGHT, 0.1f, 1000.0f );
    
    
    // The Loop
    while ( !glfwWindowShouldClose( window ) )
    {
    
        // Setter frame time (For å sikre smoothe bevegesler)
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Sjekker om noen eventer er aktivert (F.eks key trykker, musepeker flytter osv.)
        glfwPollEvents( );
        // Kaller deretter den tilhørende responsfunksjonen.
        DoMovement();
        
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Hvorfor må det skrives (1)?
        glm::mat4 model(1);
        glm::mat4 view = camera.GetViewMatrix();
        
        // Tegner den første triangelen:
        shader.Use();
        
        // Binder Textures ved å bruke texture units:
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, cubeTexture );
        glUniform1i( glGetUniformLocation( shader.Program, "texture1" ), 0 );
        
        // Henter inn uniform lokactions
        GLint modelLoc = glGetUniformLocation( shader.Program, "model" );
        GLint viewLoc = glGetUniformLocation( shader.Program, "view" );
        GLint projLoc = glGetUniformLocation( shader.Program, "projection" );
        
        // Sender matrisene til shaderen:
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
        
        glBindVertexArray( cubeVAO );
        
        // Kalkulerer modelmatrisen for hvert objekt og sender den til shaderen
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );
        
        // Deretter tegnes trianglene:
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        
        glBindVertexArray( 0 );
        
        // Tegner skyboxen:
        
        // Change depth function so depth test passes when values are equal to depth buffer's content
        glDepthFunc( GL_LEQUAL );
        skyboxShader.Use();
        
        // Remove any translation component of the view matrix
        view = glm::mat4( glm::mat3( camera.GetViewMatrix( ) ) );
        
        glUniformMatrix4fv( glGetUniformLocation( skyboxShader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( glGetUniformLocation( skyboxShader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
        
        // skybox-kuben
        
        glBindVertexArray( skyboxVAO );
        glBindTexture( GL_TEXTURE_CUBE_MAP, cubemapTexture );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );
        glDepthFunc( GL_LESS ); // Setet dybdefunksjonen tilbake til default
        
        glfwSwapBuffers( window );
    }
    
    // Avslutter GLFW. Stopper alle ressurser som er holdt av til dette.
    glfwTerminate( );
    
    return EXIT_SUCCESS;
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
