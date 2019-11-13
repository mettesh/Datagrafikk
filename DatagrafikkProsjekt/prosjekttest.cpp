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
#include "Model.h"
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

// Uniforms values
GLfloat lightPosition[] { 0.0f, 0.0f, 4.0f };
GLfloat lightAmbient[] { 0.1f, 0.1f, 0.2f };
GLfloat lightDiffuse[] { 0.5f, 0.5f, 0.5f };
GLfloat lightSpecular[] { 0.6f, 0.6f, 0.6f };
GLfloat materialShininessColor[] { 1.0f, 1.0f, 1.0f,  1.0f };
GLfloat materialShininess = 32.0f;
GLfloat cameraPosition[] { 0.0f, 0.0f, 4.0f };

// Uniform locations
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


// Names
GLuint programName;
GLuint vertexArrayName;
GLuint vertexBufferNames[1];

/*
 * Read shader source file from disk
 */
char *readSourceFile(const char *filename, int *size) {
    
    // Open the file as read only
    FILE *file = fopen(filename, "r");
    
    // Find the end of the file to determine the file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    
    // Rewind
    fseek(file, 0, SEEK_SET);
    
    // Allocate memory for the source and initialize it to 0
    char *source = (char *)malloc(fileSize + 1);
    for (int i = 0; i <= fileSize; i++) source[i] = 0;
    
    // Read the source
    fread(source, fileSize, 1, file);
    
    // Close the file
    fclose(file);
    
    // Store the size of the file in the output variable
    *size = fileSize-1;
    
    // Return the shader source
    return source;
    
}

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
    glBufferData( GL_ARRAY_BUFFER, sizeof( cubeVertices ), &cubeVertices, GL_STATIC_DRAW );
    
    // Posisjon attribute
    // glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( POSITION, 3, GL_FLOAT, GL_FALSE, 5 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    
    // Texture attribute
    // glEnableVertexAttribArray( 1 );
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
    


    // Setup and compile our shaders
    Shader shader( "resources/shaders/cube.vert", "resources/shaders/cube.frag" );
    Shader skyboxShader( "resources/shaders/skybox.vert", "resources/shaders/skybox.frag" );
    Shader lightShader( "resources/shaders/light.vert", "resources/shaders/light.frag" );

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




/*
    // Load and compile vertex shader
    GLuint vertexName = glCreateShader(GL_VERTEX_SHADER); // 2.0
    int vertexLength = 0;
    char *vertexSource = readSourceFile("resources/shaders/simple_lighting33.vert", &vertexLength);
    glShaderSource(vertexName, 1, (const char * const *)&vertexSource, &vertexLength); // 2.0
    GLint compileStatus;
    glCompileShader(vertexName); // 2.0
    glGetShaderiv(vertexName, GL_COMPILE_STATUS, &compileStatus); // 2.0
    if (!compileStatus) {
        GLint logSize = 0;
        glGetShaderiv(vertexName, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *)malloc(sizeof(char) * logSize);
        glGetShaderInfoLog(vertexName, logSize, &logSize, errorLog); // 2.0
        glDeleteShader(vertexName); // 2.0
        printf("VERTEX ERROR %s\n", errorLog);
        return 0;
    }
    free(vertexSource);
    
    // Load and compile fragment shader
    GLuint fragmentName = glCreateShader(GL_FRAGMENT_SHADER);
    int fragmentLength = 0;
    char *fragmentSource = readSourceFile("resources/shaders/simple_lighting33.frag", &fragmentLength);
    glShaderSource(fragmentName, 1, (const char * const *)&fragmentSource, &fragmentLength);
    glCompileShader(fragmentName);
    glGetShaderiv(fragmentName, GL_COMPILE_STATUS, &compileStatus);
    if (!compileStatus) {
        GLint logSize = 0;
        glGetShaderiv(fragmentName, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *)malloc(sizeof(char) * logSize);
        glGetShaderInfoLog(fragmentName, logSize, &logSize, errorLog);
        glDeleteShader(fragmentName);
        
        printf("FRAGMENT ERROR %s\n", errorLog);
        return 0;
    }
    free(fragmentSource);
    
    // Create and link vertex program
    programName = glCreateProgram(); // 2.0
    glAttachShader(programName, vertexName); // 2.0
    glAttachShader(programName, fragmentName);
    glLinkProgram(programName); // 2.0
    GLint linkStatus;
    glGetProgramiv(programName, GL_LINK_STATUS, &linkStatus); // 2.0
    if (!linkStatus) {
        GLint logSize = 0;
        glGetProgramiv(programName, GL_INFO_LOG_LENGTH, &logSize);
        char *errorLog = (char *)malloc(sizeof(char) * logSize);
        glGetProgramInfoLog(programName, logSize, &logSize, errorLog); // 2.0
        
        printf("LINK ERROR %s\n", errorLog);
        return 0;
    }
  */  
    // Get uniform locations

    lightPositionPos = glGetUniformLocation(programName, "lightPosition");
    lightAmbientPos = glGetUniformLocation(programName, "lightAmbient");
    lightDiffusePos = glGetUniformLocation(programName, "lightDiffuse");
    lightSpecularPos = glGetUniformLocation(programName, "lightSpecular");
    materialShininessColorPos = glGetUniformLocation(programName, "shininessColor");
    materialShininessPos = glGetUniformLocation(programName, "shininess");
    cameraPositionPos = glGetUniformLocation(programName, "cameraPosition");
    
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
    glUseProgram(programName); // 2.0
       

// Tegner kuben
    shader.Use();
        
    // Binder Textures ved å bruke texture units:
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, cubeTexture );
    glUniform1i( glGetUniformLocation( shader.Program, "texture1" ), 0 );


    // Set the view matrisen
    
    //glm::mat4 view = glm::mat4(1.0f);
    //view = glm::translate(view, glm::vec3(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]));
    //glUniformMatrix4fv(viewMatrixPos, 1, GL_FALSE, &view[0][0]); // 2.0
    glm::mat4 view = camera.GetViewMatrix();
    // Henter inn uniform location
    GLint viewLoc = glGetUniformLocation( shader.Program, "view" );
    // Sender matrisene til shaderen:
    glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( view ) );


    // Set the model matrisen

    //glm::mat4 model = glm::mat4(1.0);
    //model = glm::rotate(model, /*(float)glfwGetTime() **/0.5f, glm::vec3(0.0f, 1.0f,  0.0f));
    //glUniformMatrix4fv(modelMatrixPos, 1, GL_FALSE, &model[0][0]);
    glm::mat4 model(1);
    // Henter inn uniform location
    GLint modelLoc = glGetUniformLocation( shader.Program, "model" );
    // Kalkulerer modelmatrisen for hvert objekt og sender den til shaderen
    glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( model ) );


    // Sette projection matrisen:

    glm::mat4 projection = glm::perspective( camera.GetZoom(), ( float )SCREEN_WIDTH / ( float )SCREEN_HEIGHT, 0.1f, 1000.0f );
    // Henter inn uniform location:
    GLint projLoc = glGetUniformLocation( shader.Program, "projection" );
    // Sender matrisen til shaderen:
    glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projection ) );



    // Sette lysets posisjon:
    glm::vec3 lightPosition(sinf(glfwGetTime() * 1.0f), cosf(glfwGetTime() * 2.0f), 0.8f);
    glUniform3f(lightPositionPos, lightPosition.x, lightPosition.y, lightPosition.z);
    

    // Set the remaining uniforms
    glUniform3f(lightAmbientPos, lightAmbient[0], lightAmbient[1], lightAmbient[2]);
    glUniform3fv(lightDiffusePos, 1, lightDiffuse);
    glUniform3fv(lightSpecularPos, 1, lightSpecular);
    glUniform4fv(materialShininessColorPos, 1, materialShininessColor);
    glUniform1f(materialShininessPos, materialShininess);
    glUniform3fv(cameraPositionPos, 1, cameraPosition);
    
    // Aktiverer vertex-arrayen for kuben:
    //glBindVertexArray(vertexArrayName); // 3.0
    glBindVertexArray( cubeVAO );

    // Deretter tegnes trianglene:
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    
    // Draw the vertex array
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferNames[INDICES]);
    
    
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
    
    glBindVertexArray(0);


// Tegner skyboxen:

    // Change depth function so depth test passes when values are equal to depth buffer's content
    glDepthFunc( GL_LEQUAL );
    skyboxShader.Use();

    // Remove any translation component of the view matrix
    view = glm::mat4( glm::mat3( camera.GetViewMatrix( ) ) );

    glUniformMatrix4fv( glGetUniformLocation( skyboxShader.Program, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );
    glUniformMatrix4fv( glGetUniformLocation( skyboxShader.Program, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );

    // Aktiverer vertex-arrayen for skyBox:
    glBindVertexArray( skyboxVAO );
    
    glBindTexture( GL_TEXTURE_CUBE_MAP, cubemapTexture );
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    glDepthFunc( GL_LESS ); // Setet dybdefunksjonen tilbake til default
    
    // Disable
    glUseProgram(0);
    glBindVertexArray(0);
    
}

void resizeGL(int width, int height) {
    
    // Prevent division by zero
    if (height == 0)
        height = 1;
    
    // Change the projection matrix
    glm::mat4 proj = glm::perspective(3.14f/2.0f, (float)width/height, 0.1f, 100.0f);
    glUseProgram(programName);
    glUniformMatrix4fv(projectionMatrixPos, 1, GL_FALSE, &proj[0][0]);
    glUseProgram(0);
    
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
        
        // Draw OpenGL screne
        drawGLScene();
        
        // Swap buffers
        glfwSwapBuffers(window);
        
        // Sjekker om noen eventer er aktivert (F.eks key trykker, musepeker flytter osv.)
        glfwPollEvents( );
        // Kaller deretter den tilhørende responsfunksjonen.
        DoMovement();
        
    }
    
    // Shutdown GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    
    // Exit
    exit(EXIT_SUCCESS);
    
}
