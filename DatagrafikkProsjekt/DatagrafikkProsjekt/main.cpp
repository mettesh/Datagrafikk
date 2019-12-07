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

// Dimensjonene til vinduet
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Definerer alle funksjoner da C++ krever dette om de ikke kommer før kallet i koden.
int initGL();
static void glfwErrorCallback(int error, const char* description);
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
void resizeGL(int width, int height);
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );
void generateSkyBoxVerticesAndSetArraysAndBuffers();
void generateCubeVerticesAndSetArraysAndBuffers();
void generateCubeTwoVerticesAndSetArraysAndBuffers();

void drawSkybox();
void drawCube();
void drawCubeTwo();

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
GLuint cubeTwoVAO, cubeTwoVBO;
GLuint skyboxVAO, skyboxVBO;

// Shadere
Shader cubeShader;
Shader cubeTwoShader;
Shader skyboxShader;

// Uniform location kube 1:
GLint modelLoc;
GLint viewLoc;
GLint projLoc;
GLint cubeTextureLoc;
GLint cubeNormalMapLoc;
GLint lightPositionLoc;
GLint lightPositionTwoLoc;
GLint viewPositionLoc;
GLint viewPositionTwoLoc;
GLint lightColorLoc;
GLint lightColorTwoLoc;

// Uniform location kube 2:
GLint viewLocCubeTwo;
GLint projLocCubeTwo;
GLint modelLocCubeTwo;
GLint cubeTextureLocCubeTwo;
GLint lightColorLocCubeTwo;
GLint lightPositionLocCubeTwo;
GLint viewPositionLocCubeTwo;
GLint lightColorTwoLocCubeTwo;
GLint lightPositionTwoLocCubeTwo;
GLint viewPositionTwoLocCubeTwo;

// Uniform locations Skybox
GLint projLocSkybox;
GLint viewLocSkybox;


// Lys uniform values (For begge kubene)
// Light Uniforms values (For begge kubene!)
GLfloat lightPositionOneValue[] { 0.5f, 1.0f, 0.3f };
GLfloat lightPositionTwoValue[] { 0.5f, 1.0f, -1.3f };
// Henter fra Camera
//GLfloat cameraPositionValue[] { 1.0f, 0.0f, 4.0f };
//GLfloat cameraPositionTwoValue[] {5.0f, 5.0f, 4.0f };
GLfloat lightColorValue[] = {1.0f, 1.0f, 1.0f};
GLfloat lightColorTwoValue[] = {0.996f, 0.164f, 0.164f};


// Textures
GLuint cubemapTextureValue;
GLuint cubeTextureValue;
GLuint cubeNormalMapValue;




/* PROGRAMSTART */
int main(void) {
    
    // Setter feilmelding
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
    
    // Setter en input-mode for vinduet. I dette tilfellet settes musepeker til å ikke syntes (heller ikke utenfor vinduet)
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    

    // Henter inn størrelse på vinduet og lagrere dette på plassen til SCREEN_WIDTH - og _HEIGHT
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Setter endringer av vinduestørrelse
    // This function sets the size callback of the specified window, which is called when the window is resized. The callback is provided with the size, in screen coordinates, of the content area of the window.
    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    
    // Velger at det er dette vinduet OpenGL skal jobbe i
    glfwMakeContextCurrent(window);
    
    // TODO: Trengs denne? Kun for eldre versjoner??
    // glewExperimental = GL_TRUE;
    
    // Initialiserer GLEW og sjekker at det gikk ok
    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    
    // Sørge for at GLFW bytter buffere med en gang (Front og Back- Buffer)
    glfwSwapInterval(0);
    
    // Initialiserer OpenGL
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
        
        // Setter clear-farge og dybdebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        drawCubeTwo();
        
        drawSkybox();
        
        drawCube();
        
        
        
        // This function swaps the front and back buffers of the specified window
        // Front buffer = Det som vises på skjermen (Forrige frame)
        // Back buffer = Det som nå tegnes (vurrent frame)
        glfwSwapBuffers(window);
        
        // Sjekker om noen eventer er aktivert (F.eks key trykker, musepeker flytter osv.)
        glfwPollEvents( );
        
        
        // For å bestemme hvor lenge hver frame (bilde) skal vises på skjermen. For å sikre smoothe bevegelser.
        // Brukes i doMovement()
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Kaller deretter den tilhørende responsfunksjonen.
        DoMovement();
        
    }
    
    // Shutdown GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    
    // Exit
    exit(EXIT_SUCCESS);
    
}

/* Initiliaserer OpenGL */
int initGL() {
    
    generateSkyBoxVerticesAndSetArraysAndBuffers();
    generateCubeVerticesAndSetArraysAndBuffers();
    generateCubeTwoVerticesAndSetArraysAndBuffers();
    
    // Setup and compile our shaders
    cubeShader = Shader( "resources/shaders/cube.vert", "resources/shaders/cube.frag" );
    cubeTwoShader = Shader( "resources/shaders/cubeTwo.vert", "resources/shaders/cubeTwo.frag" );
    skyboxShader = Shader( "resources/shaders/skybox.vert", "resources/shaders/skybox.frag" );

    //Laste inn texture til kuben:
    cubeTextureValue = TextureLoading::LoadTexture("resources/img/cube/rock.png");

    cubeNormalMapValue = TextureLoading::LoadTexture("resources/img/cube/rock_normal.png");
    
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
    // Kubeplassering
    viewLoc = glGetUniformLocation( cubeShader.Program, "view" );
    projLoc = glGetUniformLocation( cubeShader.Program, "projection" );
    modelLoc = glGetUniformLocation( cubeShader.Program, "model" );
    // Texture
    cubeTextureLoc = glGetUniformLocation( cubeShader.Program, "cubeTexture" );
    cubeNormalMapLoc = glGetUniformLocation( cubeShader.Program, "cubeNormalMap" );
    // Lys 1
    lightColorLoc = glGetUniformLocation(cubeShader.Program, "lightOneColor");
    lightPositionLoc = glGetUniformLocation( cubeShader.Program, "lightOnePos" );
    viewPositionLoc = glGetUniformLocation( cubeShader.Program, "viewOnePos" );
    // Lys 2
    lightColorTwoLoc = glGetUniformLocation(cubeShader.Program, "lightTwoColor");
    lightPositionTwoLoc = glGetUniformLocation( cubeShader.Program, "lightTwoPos" );
    viewPositionTwoLoc = glGetUniformLocation( cubeShader.Program, "viewTwoPos" );
    
    
    
    
    cubeTwoShader.Use();
    // Kubeplassering
    viewLocCubeTwo = glGetUniformLocation( cubeTwoShader.Program, "view" );
    projLocCubeTwo = glGetUniformLocation( cubeTwoShader.Program, "projection" );
    modelLocCubeTwo = glGetUniformLocation( cubeTwoShader.Program, "model" );
    // Texture
    cubeTextureLocCubeTwo = glGetUniformLocation( cubeTwoShader.Program, "cubeTexture" );
    // Lys 1
    lightColorLocCubeTwo = glGetUniformLocation(cubeShader.Program, "lightOneColor");
    lightPositionLocCubeTwo = glGetUniformLocation( cubeShader.Program, "lightOnePos" );
    viewPositionLocCubeTwo = glGetUniformLocation( cubeShader.Program, "viewOnePos" );
    // Lys 2
    lightColorTwoLocCubeTwo = glGetUniformLocation(cubeShader.Program, "lightTwoColor");
    lightPositionTwoLocCubeTwo = glGetUniformLocation( cubeShader.Program, "lightTwoPos" );
    viewPositionTwoLocCubeTwo = glGetUniformLocation( cubeShader.Program, "viewTwoPos" );
    


    // Henter inn uniform-loactions fra skybox-shader
    skyboxShader.Use();
    projLocSkybox = glGetUniformLocation( skyboxShader.Program, "projection" );
    viewLocSkybox = glGetUniformLocation( skyboxShader.Program, "view" );
    
    
    // Implementerer Depth i applikasjonen ?????
    glEnable(GL_DEPTH_TEST);
    
    return 1;
}

/* Endrer størrelse og projectionmatriser ved endring av vindu */
void resizeGL(int width, int height) {
    
    // Feilhåndtering for å hindre at det blir deling på 0
    if (height == 0)
        height = 1;
    
    cubeShader.Use();
    glm::mat4 projectionCubeValue = glm::perspective(3.14f/2.0f, (float)width/height, 0.1f, 100.0f);
    glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projectionCubeValue ) );
    
    cubeTwoShader.Use();
    glm::mat4 projectionCubeTwoValue = glm::perspective(3.14f/2.0f, (float)width/height, 0.1f, 100.0f);
    glUniformMatrix4fv( projLocCubeTwo, 1, GL_FALSE, glm::value_ptr( projectionCubeTwoValue ) );
    
    skyboxShader.Use();
    glm::mat4 projectionSkyboxValue = glm::perspective(camera.GetZoom(), (float)width/height, 0.1f, 1000.0f );
    glUniformMatrix4fv( projLocSkybox, 1, GL_FALSE, glm::value_ptr( projectionSkyboxValue ) );
    
    // Definerer viewport-dimensjonene
    // Denne blir kalt hver gang vinduet starter.
    glViewport(0, 0, width, height); // 2.0
    
}

void generateCubeTwoVerticesAndSetArraysAndBuffers() {
 
    
    // Punktene som tilsammen bygger kuben
    glm::vec3 positions[8];
    positions[0] = glm::vec3( 3.0f,  1.0f, -1.0f);
    positions[1] = glm::vec3( 3.0f, -1.0f, -1.0f);
    positions[2] = glm::vec3( 5.0f, -1.0f, -1.0f);
    positions[3] = glm::vec3( 5.0f,  1.0f, -1.0f);
    positions[4] = glm::vec3( 3.0f,  1.0f, 1.0f);
    positions[5] = glm::vec3( 3.0f, -1.0f, 1.0f);
    positions[6] = glm::vec3( 5.0f, -1.0f, 1.0f);
    positions[7] = glm::vec3( 5.0f,  1.0f, 1.0f);
    
    
    // Texture-koordinater. Samme for hver side
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    
    // Normal-koordinater. En per side
    glm::vec3 normals[8];
     normals[0] = glm::vec3( 0.0f, 0.0f, 1.0f);
     normals[1] = glm::vec3( 0.0f, 0.0f,-1.0f);
     normals[2] = glm::vec3(-1.0f, 0.0f, 0.0f);
     normals[3] = glm::vec3( 1.0f, 0.0f, 0.0f);
     normals[4] = glm::vec3( 0.0f,-1.0f, 0.0f);
     normals[5] = glm::vec3( 0.0f, 1.0f, 0.0f);

                            
    // For å sette opp kantene i riktig rekkefølge!
    int indices[] = {0,1,2,3,1,5,6,2,4,5,1,0,4,5,6,7,3,2,6,7,0,4,7,3};
    
    // Teller for å styre hvilken normal som skal brukes til en side
    int sideCounter = 0;
    
    // Deklarerer en vector som skal holde på de ferdige verdiene til kuben
    std::vector<GLfloat> cubeTwoVertices;
    
    // Deklarer vec3 som skal holde på de 4 ulike posisjons-punktene til hver side
    glm::vec3 pos1, pos2, pos3, pos4;
    
    // Deklarerer vec3 som skal holde på normalen til hver side
    glm::vec3 nm;
     
    // Løkka kjører 6 ganger - en gang for hver side.
    for (int face = 0; face < 24; face = face + 4){
        
        // Henter ut de korrektene posisjons-koordinatene for denne siden
        pos1 = positions[indices[face]];
        pos2 = positions[indices[face + 1]];
        pos3 = positions[indices[face + 2]];
        pos4 = positions[indices[face + 3]];
        
        // Henter ut korrekt normal-verdi for denne siden
        nm = normals[sideCounter];
        // plusser på en slik at neste normal-verdi blir valgt for neste side
        sideCounter++;
        
        // Har nå alt for å bygge en side. Legger dette til i en midlertidig array
        std::vector<GLfloat> oneSide = {
            // positions            // normal         // texcoords
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y
        };
        
        // Appender den ferdige siden til cubeVertices
        for(int i = 0; i < 48; i++){
            cubeTwoVertices.push_back(oneSide[i]);
        }
        
        // Frigjør minnet. TODO: Må denne gjøres?
        oneSide.clear();
    }
    
    
    //Antall man ønsker å opprette, arrayen som skal benyttes.
    glGenVertexArrays( 1, &cubeTwoVAO );
    //Forteller OpenGL hvilken vertex-array som skal brukes.
    glBindVertexArray( cubeTwoVAO );
    
    //Antall man ønsker å opprette, arrayen som skal benyttes.
    glGenBuffers( 1, &cubeTwoVBO );
    //Forteller OpenGL at dette er current bufferen som skal brukes (Skal bufferen modifiseres senere er det denne som skal endres)
    glBindBuffer( GL_ARRAY_BUFFER, cubeTwoVBO );
    

    glBufferData( GL_ARRAY_BUFFER, 6 * 8 * 6 * sizeof( GL_FLOAT ), cubeTwoVertices.data(), GL_STATIC_DRAW );
    
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(COLOR, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (const void*)(6 * sizeof(GLfloat)));
    
    // Aktivere attributtene
    glEnableVertexAttribArray(POSITION);
    glEnableVertexAttribArray(NORMAL);
    glEnableVertexAttribArray(COLOR);

                                
    
    
}

void generateCubeVerticesAndSetArraysAndBuffers() {
    // Punktene som tilsammen bygger kuben
    glm::vec3 positions[8];
    positions[0] = glm::vec3(-1.0f,  1.0f, -1.0f);
    positions[1] = glm::vec3(-1.0f, -1.0f, -1.0f);
    positions[2] = glm::vec3( 1.0f, -1.0f, -1.0f);
    positions[3] = glm::vec3( 1.0f,  1.0f, -1.0f);
    positions[4] = glm::vec3(-1.0f,  1.0f, 1.0f);
    positions[5] = glm::vec3(-1.0f, -1.0f, 1.0f);
    positions[6] = glm::vec3( 1.0f, -1.0f, 1.0f);
    positions[7] = glm::vec3( 1.0f,  1.0f, 1.0f);
    
    
    // Texture-koordinater. SAmme for hver side
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    
    // Normal-koordinater. En per side
    glm::vec3 normals[8];
     normals[0] = glm::vec3( 0.0f, 0.0f, 1.0f);
     normals[1] = glm::vec3( 0.0f, 0.0f,-1.0f);
     normals[2] = glm::vec3(-1.0f, 0.0f, 0.0f);
     normals[3] = glm::vec3( 1.0f, 0.0f, 0.0f);
     normals[4] = glm::vec3( 0.0f,-1.0f, 0.0f);
     normals[5] = glm::vec3( 0.0f, 1.0f, 0.0f);

                            
    // For å sette opp kantene i riktig rekkefølge!
    int indices[] = {0,1,2,3,1,5,6,2,4,5,1,0,4,5,6,7,3,2,6,7,0,4,7,3};
    
    // Teller for å styre hvilken normal som skal brukes til en side
    int sideCounter = 0;
    
    // Deklarerer en vector som skal holde på de ferdige verdiene til kuben
    std::vector<GLfloat> cubeVertices;
    
    // Deklarer vec3 som skal holde på de 4 ulike posisjons-punktene til hver side
    glm::vec3 pos1, pos2, pos3, pos4;
    
    // Deklarerer vec3 som skal holde på normalen til hver side
    glm::vec3 nm;
    
    // Deklarerer vec3 som skal holde på tangent og bittangent for hver side
    glm::vec3 tangent1; //bitangent1;
    glm::vec3 tangent2; //bitangent2;
     
    // Løkka kjører 6 ganger - en gang for hver side.
    for (int face = 0; face < 24; face = face + 4){
        
        // Henter ut de korrektene posisjons-koordinatene for denne siden
        pos1 = positions[indices[face]];
        pos2 = positions[indices[face + 1]];
        pos3 = positions[indices[face + 2]];
        pos4 = positions[indices[face + 3]];
        
        // Henter ut korrekt normal-verdi for denne siden
        nm = normals[sideCounter];
        // plusser på en slik at neste normal-verdi blir valgt for neste side
        sideCounter++;
                                
    
        /* Kalkulerer verdier for 1. trekant til denne siden */
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);

        /* Kalkulerer verdier for 2. trekant til denne siden */
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        deltaUV1 = uv3 - uv1;
        deltaUV2 = uv4 - uv1;

        f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent2 = glm::normalize(tangent2);

        
        // Har nå alt for å bygge en side. Legger dette til i en midlertidig array
        std::vector<GLfloat> oneSideVertices = {
            // positions            // normal         // texcoords  // tangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z
        };
        
        // Appender den ferdige siden til cubeVertices
        for(int i = 0; i < 66; i++){
            cubeVertices.push_back(oneSideVertices[i]);
        }
        
        // Frigjør minnet. TODO: Må denne gjøres?
        oneSideVertices.clear();
        
    }
    
     //Antall man ønsker å opprette, arrayen som skal benyttes.
     glGenVertexArrays( 1, &cubeVAO );
     //Forteller OpenGL hvilken vertex-array som skal brukes.
     glBindVertexArray( cubeVAO );
     
     //Antall man ønsker å opprette, arrayen som skal benyttes.
     glGenBuffers( 1, &cubeVBO );
     //Forteller OpenGL at dette er current bufferen som skal brukes (Skal bufferen modifiseres senere er det denne som skal endres)
     glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
     

     glBufferData( GL_ARRAY_BUFFER, 6 * 11 * 6 * sizeof( GL_FLOAT ), cubeVertices.data(), GL_STATIC_DRAW );
     
     glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (GLvoid*)0);
     glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
     glVertexAttribPointer(COLOR, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (const void*)(6 * sizeof(GLfloat)));
     glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (const void*)(8 * sizeof(GLfloat)));
     
     // Aktivere attributtene
     glEnableVertexAttribArray(POSITION);
     glEnableVertexAttribArray(COLOR);
     glEnableVertexAttribArray(NORMAL);
     glEnableVertexAttribArray(TANGENT);


}

void generateSkyBoxVerticesAndSetArraysAndBuffers() {
    
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


void drawSkybox() {
    
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
}

void drawCube() {
    
    float time = glfwGetTime();
    
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
    
    //Lys 1:
    glm::vec3 lightPositionOneValue(sinf(time * 1.0f), cosf(time * 1.0f), 0.8f);
    glUniform3f(lightPositionLoc, lightPositionOneValue.x, lightPositionOneValue.y, lightPositionOneValue.z);
    //glUniform3fv(lightPositionLoc, 1, lightPositionOneValue);
    glUniform3fv(lightColorLoc, 1, lightColorValue);
    
    glUniform3f(viewPositionLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    
    //Lys 2:
    glm::vec3 lightPositionTwoValue(sinf(time * 2.0f), 2.0f, 0.8f);
    glUniform3f(lightPositionTwoLoc, lightPositionTwoValue.x, lightPositionTwoValue.y, lightPositionTwoValue.z);
    //glUniform3fv(lightPositionTwoLoc, 1, lightPositionTwoValue);
    
    glUniform3fv(lightColorTwoLoc, 1, lightColorTwoValue);
    
    glUniform3f(viewPositionTwoLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

    
    

     

    // Aktiverer vertex-arrayen for kuben:
    glBindVertexArray( cubeVAO );
    // Deretter tegnes trianglene:
    glDrawArrays( GL_TRIANGLES, 0, 36 );

    glBindVertexArray(0);
    // Deaktiverer shaderprogram som brukes og vertexarray
    glUseProgram(0);
}

void drawCubeTwo() {
    
    float time = glfwGetTime();
    
    // Aktiverer programmet
    cubeTwoShader.Use();
        
    // Henter og setter texture som sendes til cube-fragshader
    // Samme texture som andre kuben
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i(cubeTextureLocCubeTwo , 0);
    glBindTexture( GL_TEXTURE_2D, cubeTextureValue );

    // Setter view matrisen
    glm::mat4 viewCubeTwoValue = camera.GetViewMatrix();
    // Sender view-matrise til cube-shaderen:
    glUniformMatrix4fv( viewLocCubeTwo, 1, GL_FALSE, glm::value_ptr( viewCubeTwoValue ) );

    // Setter model-matrise
    glm::mat4 modelCubeTwoValue = glm::mat4(1.0);
    //model = glm::rotate(model, time * 0.5f, glm::vec3(0.0f, 1.0f,  0.0f));
    // Sender model-matrise til cube-shaderen:
    glUniformMatrix4fv( modelLocCubeTwo, 1, GL_FALSE, glm::value_ptr( modelCubeTwoValue ) );
    
    
    //Lys 1:
    glm::vec3 lightPositionOneValue(sinf(time * 1.0f), cosf(time * 1.0f), 0.8f);
    glUniform3f(lightPositionLocCubeTwo, lightPositionOneValue.x, lightPositionOneValue.y, lightPositionOneValue.z);
    //glUniform3fv(lightPositionLoc, 1, lightPositionOneValue);
    glUniform3fv(lightColorLocCubeTwo, 1, lightColorTwoValue);
    
    glUniform3f(viewPositionLocCubeTwo, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    
    //Lys 2:
    glm::vec3 lightPositionTwoValue(sinf(time * 2.0f), 2.0f, 0.8f);
    glUniform3f(lightPositionTwoLocCubeTwo, lightPositionTwoValue.x, lightPositionTwoValue.y, lightPositionTwoValue.z);
    //glUniform3fv(lightPositionTwoLoc, 1, lightPositionTwoValue);
    
    glUniform3fv(lightColorTwoLocCubeTwo, 1, lightColorTwoValue);
    
    glUniform3f(viewPositionTwoLocCubeTwo, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    

    // Aktiverer vertex-arrayen for kuben:
    glBindVertexArray( cubeTwoVAO );
       
    // Deretter tegnes trianglene:
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    
    // Deaktiverer shaderprogram som brukes og vertexarray
    glUseProgram(0);
    glBindVertexArray(0);
}


/* Error callback function for GLFW */
static void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

/* Window size changed callback function for GLFW */
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
    resizeGL(width, height);
}

// Metode som kalles hver gang en tast presses ned eller slippes opp
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode) {
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

void MouseCallback( GLFWwindow *window, double xPos, double yPos ) {
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

void DoMovement( ) {
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


