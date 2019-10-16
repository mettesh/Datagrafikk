// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "Camera.h"

// Dimensjonene til vinduet
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );

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
    
/******************************
INITIALISERING OG KLARGJØRING AV BIBLIOTEK + OPPSETT AV VINDU
*******/
    // Initialiserer GLWF og sjekker at det gikk OK
    if(glfwInit() == false){
        fprintf(stderr, "GLWF failed to initialise.");
        return -1;
    }
    
    // GLFW benytter noe kalt windowHint. Disse forteller/spør GLFW om å sette visse versjoner
    // av OpenGL + andre instillinger. - Dette er valgfritt, men Mac trenger noen!
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); //Ber her om OpenGL versjon 3.3
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_TRUE );
    
    // Opprette et GLFW-vindu som vi kan benytte til funksjonene
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Lighting - Color", nullptr, nullptr );
    
    // Sjekker at vinduet ble opprettet
    if ( nullptr == window ){
        fprintf(stderr, "Failed to create GLFW window");
        glfwTerminate( );
        return -1;
    }
    // Om alt ok velges det at det er dette vinduet OpenGL skal jobbe i
    glfwMakeContextCurrent( window );
    
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
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
    
    
// IMPLEMENTERER DEPTH INN I APPLIKASJONEN
    glEnable( GL_DEPTH_TEST );
    
//FOR TEXTURE!
    // Enables alpha-support. Kan nå loade JPEGs.
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    

/******************************
SPESIFISERER KUBEN:
*******/

    GLfloat vertices[] =
    {
        //X     Y       Z    // Normal
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };
    
    
    
// Oppretter Vertex-array og buffer -object (ID til denne)
    GLuint VBO, boxVAO;
    glGenVertexArrays( 1, &boxVAO ); //Antall man ønsker å opprette, arrayen som skal benyttes.
    glGenBuffers( 1, &VBO );
    
    glBindVertexArray( boxVAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    
    // Position attribute
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * )0 );
    glEnableVertexAttribArray( 0 );
    
    // Normal attribute
    glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ), ( GLvoid * )( 3 * sizeof( GLfloat ) ) );
    glEnableVertexAttribArray( 1 );
    glBindVertexArray( 0 );
    
    
    
    // Oppretter bugger for light-vertex!
    GLuint lightVAO;
    glGenVertexArrays( 1, &lightVAO ); //Antall man ønsker å opprette, arrayen som skal benyttes.
    glGenBuffers( 1, &VBO );
    
    glBindVertexArray( lightVAO );
    glBindBuffer( GL_ARRAY_BUFFER, VBO );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );
    
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 6 * sizeof( GLfloat ),( GLvoid * ) 0 );
    glEnableVertexAttribArray( 0 );
    
    glBindVertexArray( 0 );
    
    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( GLfloat )SCREEN_WIDTH / ( GLfloat )SCREEN_HEIGHT, 0.1f, 1000.0f);
    
    
    // Bygger og kompilerer shader-programmet
    Shader lightingShader( "resources/shaders/lighting.vert", "resources/shaders/lighting.frag" );
    Shader lampShader( "resources/shaders/lamp.vert", "resources/shaders/lamp.frag" );
    
    
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
    
        // For å sikre smoothe bevegesler
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Sjekker om noen eventer er aktivert )F.eks key trykker, musepeker flytter osv.).
        glfwPollEvents( );
        // Kaller deretter den tilhørende responsfunksjonen.
        DoMovement();
        
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
/*LYS - lighting*/
        lightingShader.Use( );
        GLint objectColorLoc = glGetUniformLocation( lightingShader.Program, "objectColor" );
        GLint lightColorLoc = glGetUniformLocation( lightingShader.Program, "lightColor" );
        GLint lightPosLoc = glGetUniformLocation( lightingShader.Program, "lightPos" );
        GLint viewPosLoc = glGetUniformLocation( lightingShader.Program, "viewPos" );
        
        glUniform3f( objectColorLoc, 10.0f, 0.5f, 0.3f );
        glUniform3f( lightColorLoc, 1.0f, 1.0f, 1.0f );
        
        
        glm::vec3 lightPos(sinf(glfwGetTime() * 0.8f), cosf(glfwGetTime() * 0.5f), 0.8f);
        glUniform3f( lightPosLoc, lightPos.x, lightPos.y, lightPos.z);

        
        glUniform3f( viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z );
        
        glm::mat4 view(1);
        view = camera.GetViewMatrix( );
        
        GLint modelLoc = glGetUniformLocation( lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation( lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation( lightingShader.Program, "projection");
        
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
        
        glBindVertexArray( boxVAO );
        // -----> Hvorfor (1)???
        glm::mat4 model(1);
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ));
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );
        
        
        
        
/*LYS - lamp*/
        lampShader.Use( );
        
        modelLoc = glGetUniformLocation( lampShader.Program, "model");
        viewLoc = glGetUniformLocation( lampShader.Program, "view");
        projLoc = glGetUniformLocation( lampShader.Program, "projection");
        
        glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );
        glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );
        
        model = glm::mat4(1);
        model = glm::translate( model, lightPos );
        // Gjør lyset mindr:
        model = glm::scale( model, glm::vec3( 0.2f ) );
        glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ));
        
        glBindVertexArray( lightVAO );
        glDrawArrays( GL_TRIANGLES, 0, 36 );
        glBindVertexArray( 0 );
        
        
        glfwSwapBuffers( window );
    }
    
    // Lediggjør minnet holdt av til vector-array og -buffer så fort det ikke lenger trengs.
    glDeleteVertexArrays( 1, &boxVAO );
    glDeleteVertexArrays( 1, &lightVAO );
    glDeleteBuffers( 1, &VBO );
    
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
