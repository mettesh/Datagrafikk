// BIBLIOTEK
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SOIL2.h"

// EGNE INCLUDES
#include "shader.h"
#include "camera.h"
#include "texture.h"

// FUNKSJONER
// Definerer alle funksjonene da C++ krever dette om de ikke kommer før kallet i koden.
int initGL();
static void glfwErrorCallback(int error, const char* description);
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height);
void resizeGL(int width, int height);
void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void mouseCallback( GLFWwindow *window, double xPos, double yPos );
void doMovement();
void generateSkyBoxVerticesAndSetArraysAndBuffers();
void generateCubeVerticesAndSetArraysAndBuffers();
void generatehexPrismVerticesAndSetArraysAndBuffers();
void generatePyramidVerticesAndSetArraysAndBuffers();
void drawSkybox();
void drawCube();
void drawHexPrism();
void drawPyramid();
void setLightMatricesForCubeAndPyramide();

// VINDUEDIMENSJONER
#define DEFAULT_WIDTH 1024
#define DEFAULT_HEIGHT 768
int SCREEN_WIDTH, SCREEN_HEIGHT;

// KAMERA OG BEVEGELSE
// Setter startposisjon til kamera
Camera camera( glm::vec3( 0.0f, 0.0f, 7.0f ) );
GLfloat lastX = SCREEN_WIDTH / 2.0f;
GLfloat lastY = SCREEN_WIDTH / 2.0f;
// Deklarer variabel for å holde på tastatur-keys
bool keys[1024];
bool firstMouse = true;
// For å sikre jevne bevegelser regnes dette ut
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// VERTEX ARRAY ATTRIBUTTER
#define POSITION 0
#define NORMAL 1
#define COLOR 2
#define TANGENT 3

// VERTEXARRAY- OG BUFFERE
GLuint cubeVAO, cubeVBO;
GLuint hexPrismVAO, hexPrismVBO;
GLuint pyramidVAO, pyramidVBO;
GLuint skyboxVAO, skyboxVBO;

// SHADERE
Shader cubeAndPyramidShader;
Shader hexPrismShader;
Shader skyboxShader;

// UNIFORM LOCATIONS FOR KUBE OG PYRAMIDE:
GLint modelLoc;
GLint viewLoc;
GLint projLoc;
GLint cubeTextureLoc;
GLint normalMapLoc;
GLint depthMapLoc;
GLint heighScaleLoc;
GLint lightPositionOneLoc;
GLint lightPositionTwoLoc;
GLint lightColorOneLoc;
GLint lightColorTwoLoc;
GLint viewPositionLoc;

// UNIFORM LOCATIONS FOR PRISME:
GLint viewLochexPrism;
GLint projLochexPrism;
GLint modelLochexPrism;
GLint textureLochexPrism;
GLint lightColorOneLochexPrism;
GLint lightPositionOneLochexPrism;
GLint lightColorTwoLochexPrism;
GLint lightPositionTwoLochexPrism;
GLint viewPositionLochexPrism;

// UNIFORM LOCATIONS FOR SKYBOX:
GLint projLocSkybox;
GLint viewLocSkybox;

// LYS VALUES (For alle objektene)
GLfloat lightPositionOneValue[] { 9.0f, 1.0f, -1.5f };
GLfloat lightPositionTwoValue[] { -3.0f, 1.0f, -1.5f };
GLfloat lightColorOneValue[] = {1.0f, 1.0f, 1.0f};
GLfloat lightColorTwoValue[] = {0.976f, 0.282f, 0.376f}; // Rød

glm::mat4 viewValue;

// TEXTURES
GLuint skyBoxTextureValue;
GLuint cubeTextureValue;
GLuint cubeNormalMapValue;
GLuint cubeDepthMapValue;
GLuint hexPrismTextureValue;
GLuint pyramidTextureValue;
GLuint pyramidNormalMapValue;
GLuint pyramidDepthMapValue;

GLfloat timer = 0.1f;


/* Programstart */
int main(void) {
    
    // Setter feilmelding
    glfwSetErrorCallback(glfwErrorCallback);
    
    // Initialiserer GLWF og sjekker at det gikk OK
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        exit(EXIT_FAILURE);
    }
    
    // GLFW benytter noe kalt windowHint. Disse forteller/spør GLFW om å sette visse versjoner
    // av OpenGL + andre instillinger. - Dette er valgfritt, men Mac trenger noen ekstra!
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
    glfwSetKeyCallback( window, keyCallback );
    glfwSetCursorPosCallback( window, mouseCallback );
    
    // Setter en input-mode for vinduet. I dette tilfellet settes musepeker til å ikke syntes (heller ikke utenfor vinduet)
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Henter inn størrelse på vinduet og lagrere dette på plassen til SCREEN_WIDTH - og _HEIGHT
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Setter endringer av vinduestørrelse. Denne kalles hver gang vinduets størrelse justeres
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
        
        // Henter view-matrise utifra camera sine posisjoner. Brukes for alle objektene
        viewValue = camera.getViewMatrix();
        
        resizeGL(SCREEN_WIDTH, SCREEN_HEIGHT);
        
        drawHexPrism();
        
        drawSkybox();
        
        drawCube();
        
        drawPyramid();
        
        // Svapper front- og backbuffere til vinduet
        // Front buffer = Det som vises på skjermen (Forrige frame)
        // Back buffer = Det som nå tegnes (current frame)
        glfwSwapBuffers(window);
        
        // Sjekker om noen eventer er aktivert (F.eks key trykker, musepeker flytter osv.)
        glfwPollEvents( );
        
        // For å bestemme hvor lenge hver frame (bilde) skal vises på skjermen. For å sikre smoothe bevegelser.
        // Brukes i doMovement()
        GLfloat currentFrame = glfwGetTime( );
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Kaller deretter den tilhørende responsfunksjonen.
        doMovement();
        
    }
    
    // Shutdown GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
    
    // Exit
    exit(EXIT_SUCCESS);
    
}

/* Henter inn og kompilerer shadere, laster inn textures og henter Uniform-locations  */
int initGL() {
    
    // Setter opp og kompilerer shaderne
    cubeAndPyramidShader = Shader( "resources/shaders/cubeAndPyramid.vert", "resources/shaders/cubeAndPyramid.frag" );
    hexPrismShader = Shader( "resources/shaders/hexPrism.vert", "resources/shaders/hexPrism.frag" );
    skyboxShader = Shader( "resources/shaders/skybox.vert", "resources/shaders/skybox.frag" );
    
    // Generer vertecies til de ulike objektene
    generateCubeVerticesAndSetArraysAndBuffers();
    generatehexPrismVerticesAndSetArraysAndBuffers();
    generatePyramidVerticesAndSetArraysAndBuffers();
    generateSkyBoxVerticesAndSetArraysAndBuffers();
    
    //Laste inn diffuse, normal og displacement-texture til kube og prisme. Kun diffse texture til pyramide:
    cubeTextureValue = TextureLoading::loadTexture("resources/img/objects/wood.jpg");
    cubeNormalMapValue = TextureLoading::loadTexture("resources/img/objects/wood_normal.jpg");
    cubeDepthMapValue = TextureLoading::loadTexture("resources/img/objects/wood_disp.jpg");
    hexPrismTextureValue = TextureLoading::loadTexture("resources/img/objects/bricks.jpg");
    pyramidTextureValue = TextureLoading::loadTexture("resources/img/objects/bricks.jpg");
    pyramidNormalMapValue = TextureLoading::loadTexture("resources/img/objects/bricks_normal.jpg");
    pyramidDepthMapValue = TextureLoading::loadTexture("resources/img/objects/bricks_disp.jpg");
    
    //Laste inn texture til skyboxen:
    std::vector<const GLchar*> skyBoxTextureFaces;
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_lf.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_rt.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_up.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_dn.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_ft.tga" );
    skyBoxTextureFaces.push_back( "resources/img/skybox/iceflats_bk.tga" );
    skyBoxTextureValue = TextureLoading::loadSkyBox( skyBoxTextureFaces );
    
    // Henter inn uniform-loactions fra kube- og pyramideshader
    cubeAndPyramidShader.use();
    viewLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "view" );
    projLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "projection" );
    modelLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "model" );
    // Texture
    cubeTextureLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "cubeTexture" );
    normalMapLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "cubeNormalMap" );
    depthMapLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "depthMap" );
    // Dypbde (til parallax mapping)
    heighScaleLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "heightScale" );
    // Lys 1
    lightColorOneLoc = glGetUniformLocation(cubeAndPyramidShader.Program, "lightOneColor");
    lightPositionOneLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "lightOnePos" );
    // Lys 2
    lightColorTwoLoc = glGetUniformLocation(cubeAndPyramidShader.Program, "lightTwoColor");
    lightPositionTwoLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "lightTwoPos" );
    
    viewPositionLoc = glGetUniformLocation( cubeAndPyramidShader.Program, "viewPos" );
    
    
    // Henter inn uniform-loactions fra hexagon prisme-shader
    hexPrismShader.use();
    // Prismeplassering
    viewLochexPrism = glGetUniformLocation( hexPrismShader.Program, "view" );
    projLochexPrism = glGetUniformLocation( hexPrismShader.Program, "projection" );
    modelLochexPrism = glGetUniformLocation( hexPrismShader.Program, "model" );
    // Texture
    textureLochexPrism = glGetUniformLocation( hexPrismShader.Program, "cubeTexture" );
    // Lys 1
    lightColorOneLochexPrism = glGetUniformLocation(hexPrismShader.Program, "lightOneColor");
    lightPositionOneLochexPrism = glGetUniformLocation( hexPrismShader.Program, "lightOnePos" );
    // Lys 2
    lightColorTwoLochexPrism = glGetUniformLocation(hexPrismShader.Program, "lightTwoColor");
    lightPositionTwoLochexPrism = glGetUniformLocation( hexPrismShader.Program, "lightTwoPos" );
    
    viewPositionLochexPrism = glGetUniformLocation( hexPrismShader.Program, "viewPos" );
    
    
    // Henter inn uniform-loactions fra skybox-shader
    skyboxShader.use();
    projLocSkybox = glGetUniformLocation( skyboxShader.Program, "projection" );
    viewLocSkybox = glGetUniformLocation( skyboxShader.Program, "view" );
    

    glEnable(GL_DEPTH_TEST);
    
    return 1;
}


/* Generer vertices og setter til arrays og buffere */
void generateSkyBoxVerticesAndSetArraysAndBuffers() {
    
    // Punktene som tilsammen bygger kuben
     glm::vec3 skyBoxPositions[8];
     skyBoxPositions[0] = glm::vec3(-1.0f,  1.0f, -1.0f);  // Oppe bak venstre
     skyBoxPositions[1] = glm::vec3(-1.0f, -1.0f, -1.0f);  // Nede bak venstre
     skyBoxPositions[2] = glm::vec3( 1.0f, -1.0f, -1.0f);  // Nede bak høyre
     skyBoxPositions[3] = glm::vec3( 1.0f,  1.0f, -1.0f);  // Oppe bak høyre
     skyBoxPositions[4] = glm::vec3(-1.0f,  1.0f, 1.0f);   // Oppe frem venstre
     skyBoxPositions[5] = glm::vec3(-1.0f, -1.0f, 1.0f);   // Nede frem venstre
     skyBoxPositions[6] = glm::vec3( 1.0f, -1.0f, 1.0f);   // Nede frem høyre
     skyBoxPositions[7] = glm::vec3( 1.0f,  1.0f, 1.0f);   // Oppe frem høyre
    
    // For å sette opp kantene i riktig rekkefølge!
    //                 Bak   -  Bunn   - Venstre -  Front  -  Høyre  -  Topp
    int indices[] = {0,3,2,1,  6,5,1,2,  1,5,4,0,  4,5,6,7,  3,7,6,2,  0,4,7,3};
     
    // Deklarerer en vector som skal holde på de ferdige verdiene til kuben
    std::vector<GLfloat> skyBoxVertices;
     
     // Deklarer vec3 som skal holde på de 4 ulike posisjons-punktene til hver side
     glm::vec3 pos1, pos2, pos3, pos4;
      
     // Løkka kjører 6 ganger - en gang for hver side.
     for (int face = 0; face < 24; face = face + 4){
         
         // Henter ut de korrektene posisjons-koordinatene for denne siden
         pos1 = skyBoxPositions[indices[face]];
         pos2 = skyBoxPositions[indices[face + 1]];
         pos3 = skyBoxPositions[indices[face + 2]];
         pos4 = skyBoxPositions[indices[face + 3]];
         
         // Har nå alt for å bygge en side. Legger dette til i en midlertidig array
         std::vector<GLfloat> oneSideVertices = {
             // posisjoner
             pos1.x, pos1.y, pos1.z,
             pos2.x, pos2.y, pos2.z,
             pos3.x, pos3.y, pos3.z,

             pos1.x, pos1.y, pos1.z,
             pos3.x, pos3.y, pos3.z,
             pos4.x, pos4.y, pos4.z,
         };
         
         // Appender den ferdige siden til cubeVertices
         for (auto &vertex : oneSideVertices) skyBoxVertices.push_back(vertex);
     }
    
    /*  // Brukte tidligere denne men har videreutviklet!
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
     */

    // Oppretter en vertex-array for skyBoxen og forteller bruker denne
    glGenVertexArrays( 1, &skyboxVAO );
    glBindVertexArray( skyboxVAO );
    
    // Oppretter en buffer for skyBoxen og forteller OpenGL at dette er bufferen som skal brukes
    glGenBuffers( 1, &skyboxVBO );
    glBindBuffer( GL_ARRAY_BUFFER, skyboxVBO );
    
    //Fyller bufferen med data: Bufferen som skal brukes, størrelsen den på holde av, de vertices som skal lagres, og info at det skal tegnes.
    glBufferData( GL_ARRAY_BUFFER, 108 * sizeof( GL_FLOAT ), skyBoxVertices.data(), GL_STATIC_DRAW );
    // Spesifiserer attributtene (Slik at man vet hvilke vertices som er for de ulike attributtene)
    glVertexAttribPointer( POSITION, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( GLfloat ), ( GLvoid * ) 0 );
    // Aktivere attributten
    glEnableVertexAttribArray(POSITION);
}

void generateCubeVerticesAndSetArraysAndBuffers() {
    
    // Punktene som tilsammen bygger kuben
    glm::vec3 positions[8];
    positions[0] = glm::vec3(-1.0f,  1.0f, -1.0f);  // Oppe bak venstre
    positions[1] = glm::vec3(-1.0f, -1.0f, -1.0f);  // Nede bak venstre
    positions[2] = glm::vec3( 1.0f, -1.0f, -1.0f);  // Nede bak høyre
    positions[3] = glm::vec3( 1.0f,  1.0f, -1.0f);  // Oppe bak høyre
    positions[4] = glm::vec3(-1.0f,  1.0f, 1.0f);   // Oppe frem venstre
    positions[5] = glm::vec3(-1.0f, -1.0f, 1.0f);   // Nede frem venstre
    positions[6] = glm::vec3( 1.0f, -1.0f, 1.0f);   // Nede frem høyre
    positions[7] = glm::vec3( 1.0f,  1.0f, 1.0f);   // Oppe frem høyre
    
    // Texture-koordinater. Samme for hver side
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    
    // For å sette opp kantene i riktig rekkefølge!
    //                 Bak   -  Bunn   - Venstre -  Front  -  Høyre  -  Topp
    int indices[] = {0,3,2,1,  6,5,1,2,  1,5,4,0,  4,5,6,7,  3,7,6,2,  0,4,7,3};
    
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
        
        // Tar kryssproduktet av kantene til triangelen for å finne normalverdien
        glm::vec3 normalFirstTriangel = normalize(cross(edge1, edge2));

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
        
        // Tar kryssproduktet av kantene til triangelen for å finne normalverdien
        glm::vec3 normalSecondTriangel = normalize(cross(edge1, edge2));
        
        // Legger sammen disse verdiene for å få normalverdi for firkanten
        nm = normalize(normalFirstTriangel + normalSecondTriangel);

        // Har nå alt for å bygge en side. Legger dette til i en midlertidig array
        std::vector<GLfloat> oneSideVertices = {
            // posisjoner            // normal         // texture  // tangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z
        };
        
        // Appender den ferdige siden til cubeVertices
        for (auto &vertex : oneSideVertices) cubeVertices.push_back(vertex);
    }
    
    
    // Oppretter en vertex-array for kuben
    glGenVertexArrays( 1, &cubeVAO );
    // Forteller OpenGL at denne vertex-array skal brukes.
    glBindVertexArray( cubeVAO );
     
    // Oppretter en buffer for kuben
    glGenBuffers( 1, &cubeVBO );
    // Forteller OpenGL at dette er bufferen som skal brukes (Skal bufferen modifiseres senere er det denne som skal endres)
    glBindBuffer( GL_ARRAY_BUFFER, cubeVBO );
    
    // Setter data til bufferen. Forteller hvor mye plass det tar
    glBufferData( GL_ARRAY_BUFFER, 6 * 11 * 6 * sizeof( GL_FLOAT ), cubeVertices.data(), GL_STATIC_DRAW );
    // Spesifiserer attributtene (Slik at man vet hvilke vertices som er for de ulike attributtene)
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (GLvoid*)0);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(COLOR, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (const void*)(6 * sizeof(GLfloat)));
    glVertexAttribPointer(TANGENT, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GL_FLOAT), (const void*)(8 * sizeof(GLfloat)));
     
    // Aktiverer attributtene
    glEnableVertexAttribArray(POSITION);
    glEnableVertexAttribArray(COLOR);
    glEnableVertexAttribArray(NORMAL);
    glEnableVertexAttribArray(TANGENT);
}

void generatePyramidVerticesAndSetArraysAndBuffers() {
    
    // Punktene som tilsammen bygger pyramiden
    glm::vec3 positions[5];
    positions[0] = glm::vec3( 0.0f,  1.0f, 0.0f); // Toppunkt
    positions[1] = glm::vec3(-1.0f,-1.0f, -1.0f);  // Nede bakerst til venstre
    positions[2] = glm::vec3( 1.0f, -1.0f,-1.0f);  // Nede bakerst til høyre
    positions[3] = glm::vec3(-1.0f, -1.0f, 1.0f);  // Nede framme til venstre
    positions[4] = glm::vec3( 1.0f, -1.0f, 1.0f);  // Nede fremme til høyre
    
    // Texture-koordinater. Samme for hver side
    glm::vec2 uv1(0.5f, 0.0f);
    glm::vec2 uv2(1.0f, 1.0f);
    glm::vec2 uv3(0.0f, 1.0f);
    
    //               Venstre - Høyre  - Front  -  Bak  -  Bunn1  - Bunn2
    int indices[] = { 0,1,3,   0,4,2,   0,3,4,   0,2,1,   1,2,3,   4,3,2};
    
    // Deklarerer en vector som skal holde på de ferdige verdiene til kuben
    std::vector<GLfloat> pyramidVertices;
    
    // Deklarer vec3 som skal holde på de 3 ulike posisjons-punktene til hver side
    glm::vec3 pos1, pos2, pos3;
    
    // Deklarerer vec3 som skal holde på normalen til hver side
    glm::vec3 nm;
    
    // Deklarerer vec3 som skal holde på tangent
    glm::vec3 tangent1;
    glm::vec3 tangent2;
     
    // Løkka kjører 6 ganger - en gang for hver trekant.
    for (int face = 0; face < 18; face = face + 3){
        
        // Henter ut de korrektene posisjons-koordinatene for denne siden
        pos1 = positions[indices[face]];
        pos2 = positions[indices[face + 1]];
        pos3 = positions[indices[face + 2]];
        
        // Om face er mer enn 11, vet jeg at bunnen nå skal bygges. Denne må få satt texture annerledes da det skal utgjøre en firkant
        if(face > 11){
            uv1 = glm::vec2(1.0f, 0.0f);
        }

        /* Kalkulerer verdier for trekant til denne siden */
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;
        
        // Tar kryssproduktet av kantene til triangelen for å finne normalverdien
        nm = normalize(cross(edge1, edge2));

        GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
        tangent1 = glm::normalize(tangent1);
        
        
        // Har nå alt for å bygge en side. Legger dette til i en midlertidig array
        std::vector<GLfloat> onePyramidSideVertices = {
            // positions            // normal         // texcoords  // tangent
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z,
        };
        
        // Appender den ferdige siden til pyramidVertices
        for (auto &vertex : onePyramidSideVertices) pyramidVertices.push_back(vertex);
    }
    
    // Oppretter en vertex-array for pyramiden
    glGenVertexArrays( 1, &pyramidVAO );
    // Forteller OpenGL at denne vertex-array skal brukes.
    glBindVertexArray( pyramidVAO );
     
    // Oppretter en buffer for pyramiden
    glGenBuffers( 1, &pyramidVBO );
    // Forteller OpenGL at dette er bufferen som skal brukes
    glBindBuffer( GL_ARRAY_BUFFER, pyramidVBO );
     
    // Setter data til bufferen. Forteller hvor mye plass det tar
    glBufferData( GL_ARRAY_BUFFER, 3 * 11 * 6 * sizeof( GL_FLOAT ), pyramidVertices.data(), GL_STATIC_DRAW );
    // Spesifiserer attributtene (Slik at man vet hvilke vertices som er for de ulike attributtene)
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

void generatehexPrismVerticesAndSetArraysAndBuffers() {
 
    // Punktene som tilsammen bygger hexagon prismen
    glm::vec3 positions[16];
    positions[0]  = glm::vec3(-0.5f, -1.0f, -1.0f); // Bakerst venstre nede
    positions[1]  = glm::vec3( 0.5f, -1.0f, -1.0f); // Bakerst høyre nede
    positions[2]  = glm::vec3(-1.0f, -1.0f, -0.5f); // Bak venstre nede
    positions[3]  = glm::vec3( 1.0f, -1.0f, -0.5f); // Bak høyre nede
    
    positions[4]  = glm::vec3(-1.0f, -1.0f,  0.5f); // Forann venstre nede
    positions[5]  = glm::vec3( 1.0f, -1.0f,  0.5f); // Forann høyre nede
    positions[6]  = glm::vec3(-0.5f, -1.0f,  1.0f); // Fremst venstre nede
    positions[7]  = glm::vec3( 0.5f, -1.0f,  1.0f); // Fremst høyre nede
    
    positions[8]  = glm::vec3(-0.5f,  1.0f, -1.0f); // Bakerst venstre oppe
    positions[9]  = glm::vec3( 0.5f,  1.0f, -1.0f); // Bakerst høyre oppe
    positions[10] = glm::vec3(-1.0f,  1.0f, -0.5f); // Bak venstre oppe
    positions[11] = glm::vec3( 1.0f,  1.0f, -0.5f); // Bak høyre oppe
    
    positions[12] = glm::vec3(-1.0f,  1.0f,  0.5f); // Forann venstre oppe
    positions[13] = glm::vec3( 1.0f,  1.0f,  0.5f); // Forann høyre oppe
    positions[14] = glm::vec3(-0.5f,  1.0f,  1.0f); // Fremst venstre oppe
    positions[15] = glm::vec3( 0.5f,  1.0f,  1.0f); // Fremst høyre oppe
    
    // Texture-koordinater. Samme for hver side
    glm::vec2 uv1(0.0f, 1.0f);
    glm::vec2 uv2(0.0f, 0.0f);
    glm::vec2 uv3(1.0f, 0.0f);
    glm::vec2 uv4(1.0f, 1.0f);
    
                            
    int indices[] = {
        9,1,0,8,    // Bakerst
        8,0,2,10,   // Bak venstre
        11,3,1,9,   // Bak høyre
        10,2,4,12,  // Venstre
        13,5,3,11,  // Høyre
        12,4,6,14,  // Frem venstre
        15,7,5,13,  // Frem høyre
        14,6,7,15,  // Front
        // Topp:
        12,10,8,14,
        14,8,9,15,
        15,9,11,13,
        // Bunn:
        4,2,0,6,
        6,0,1,7,
        7,1,3,5
    };
        
    // Deklarerer en vector som skal holde på de ferdige verdiene til prismen
    std::vector<GLfloat> hexPrismVertices;
    
    // Deklarer vec3 som skal holde på de 4 ulike posisjons-punktene til hver side
    glm::vec3 pos1, pos2, pos3, pos4;
    
    // Deklarerer vec3 som skal holde på normalen til hver side
    glm::vec3 nm;
     
    // Løkka kjører 14 ganger - Prismen blir bygget av totalt 14 firkanter
    for (int face = 0; face < 56; face = face + 4){
        
        // Henter ut de korrektene posisjons-koordinatene for denne siden
        pos1 = positions[indices[face]];
        pos2 = positions[indices[face + 1]];
        pos3 = positions[indices[face + 2]];
        pos4 = positions[indices[face + 3]];
        
        // Finner kantene for første triangel av firkant
        glm::vec3 edge1 = pos2 - pos1;
        glm::vec3 edge2 = pos3 - pos1;
        
        // Tar kryssproduktet av kantene til første triangelen for å finne normalverdien
        glm::vec3 normalFirstTriangel = normalize(cross(edge1, edge2));

        // Finner kantene for andre triangel av firkant
        edge1 = pos3 - pos1;
        edge2 = pos4 - pos1;
        
        // Tar kryssproduktet av kantene til denne triangelen for å finne normalverdien
        glm::vec3 normalSecondTriangel = normalize(cross(edge1, edge2));

        nm = normalize(normalFirstTriangel + normalSecondTriangel);
        
        switch(face)
        {
            case 32:
            case 44:
                uv1 = glm::vec2(0.5f, 0.75f);
                uv2 = glm::vec2(0.5f, 0.25f);
                uv3 = glm::vec2(1.0f, 0.0f);
                uv4 = glm::vec2(1.0f, 1.0f);
            break;
            case 40:
            case 52:
                uv1 = glm::vec2(0.0f, 1.0f);
                uv2 = glm::vec2(0.0f, 0.0f);
                uv3 = glm::vec2(0.5f, 0.25f);
                uv4 = glm::vec2(0.5f, 0.75f);
            break;
           default :
                uv1 = glm::vec2(0.0f, 1.0f);
                uv2 = glm::vec2(0.0f, 0.0f);
                uv3 = glm::vec2(1.0f, 0.0f);
                uv4 = glm::vec2(1.0f, 1.0f);
        }
        
        // Har nå alt for å bygge en side. Legger dette til i en midlertidig array
        std::vector<GLfloat> oneSideVertices = {
            // positions            // normal         // texcoords
            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
            pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,

            pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y,
            pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y,
            pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y
        };
        
        // Appender den ferdige siden til hexPrismVertices
        for (auto &vertex : oneSideVertices) hexPrismVertices.push_back(vertex);
    }
    
    // Oppretter en vertex-array for prismen
    glGenVertexArrays( 1, &hexPrismVAO );
    // Forteller OpenGL at denne vertex-array skal brukes.
    glBindVertexArray( hexPrismVAO );
    
    // Oppretter en buffer for prismen
    glGenBuffers( 1, &hexPrismVBO );
    // Forteller OpenGL at dette er bufferen som skal brukes (Skal bufferen modifiseres senere er det denne som skal endres)
    glBindBuffer( GL_ARRAY_BUFFER, hexPrismVBO );
    
    // Setter data til bufferen. Forteller hvor mye plass det tar
    glBufferData( GL_ARRAY_BUFFER, 6 * 8 * 14 * sizeof( GL_FLOAT ), hexPrismVertices.data(), GL_STATIC_DRAW );
    // Spesifiserer attributtene (Slik at man vet hvilke vertices som er for de ulike attributtene)
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)0);
    glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GLfloat)));
    glVertexAttribPointer(COLOR, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (const void*)(6 * sizeof(GLfloat)));
    
    // Aktivere attributtene
    glEnableVertexAttribArray(POSITION);
    glEnableVertexAttribArray(NORMAL);
    glEnableVertexAttribArray(COLOR);
}


/* Setter matriser og tegner objektene */
void drawSkybox() {
    
    skyboxShader.use();
    
    // Henter inn og setter texture som sendes til shaderen
    glBindTexture( GL_TEXTURE_CUBE_MAP, skyBoxTextureValue );
    
    // Endrer depth-funksjonen. Sjekker at verdien er lik depth-buffer
    glDepthFunc( GL_LEQUAL );

    // Henter inn view-matrise og gjør denne over til en mat4 da dette er en skybox, og kuben skal wrappes rundt
    glm::mat4 viewSkyboxValue = glm::mat3( viewValue );
    // Sender view-matrise til shaderen:
    glUniformMatrix4fv( viewLocSkybox, 1, GL_FALSE, glm::value_ptr( viewSkyboxValue ) );
    
    
    glm::mat4 projection = glm::perspective(camera.getZoom(), (GLfloat)SCREEN_WIDTH/(GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
    glUniformMatrix4fv( projLocSkybox, 1, GL_FALSE, glm::value_ptr( projection ) );

    // Aktiverer vertex-arrayen for skyBox:
    glBindVertexArray( skyboxVAO );
    
    // Tegner trianglene som bygger kuben
    glDrawArrays( GL_TRIANGLES, 0, 36 );
    
    // Setter dybdefunksjonen tilbake til default
    glDepthFunc( GL_LESS );
    
    // Deaktiverer shaderprogram som brukes og vertexarray
    glUseProgram(0);
    glBindVertexArray(0);
}

void drawCube() {
        
    // Aktiverer shader-programmet
    cubeAndPyramidShader.use();
        
    // Henter og setter diffuse, normal og depth-texture som sendes til cube-fragshader
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i(cubeTextureLoc , 0);
    glBindTexture( GL_TEXTURE_2D, cubeTextureValue );
    
    glActiveTexture( GL_TEXTURE1 );
    glUniform1i(normalMapLoc , 1);
    glBindTexture( GL_TEXTURE_2D, cubeNormalMapValue );
    
    glActiveTexture( GL_TEXTURE2 );
    glUniform1i(depthMapLoc , 2);
    glBindTexture( GL_TEXTURE_2D, cubeDepthMapValue );
    
    // Sender view-matrise til cube-shaderen:
    glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( viewValue ) );

    // Setter model-matrise
    glm::mat4 modelCubeValue = glm::mat4(1.0f);
    // Flytter kubens origo 4 punkter til venstre
    modelCubeValue = glm::translate(modelCubeValue, glm::vec3(-4.0, 0.0, 0.0));
    // Sender model-matrise til shaderen:
    glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( modelCubeValue ) );
    
    setLightMatricesForCubeAndPyramide();
    
    // Aktiverer vertex-arrayen for kuben:
    glBindVertexArray( cubeVAO );
    // Deretter tegnes trianglene:
    glDrawArrays( GL_TRIANGLES, 0, 36 ); // 12 trekanter - 3 punkter

    // Deaktiverer shaderprogram og vertexarray som brukes
    glBindVertexArray(0);
    glUseProgram(0);
}

void drawPyramid() {
    
    float time = glfwGetTime();
    
    // Aktiverer programmet
    cubeAndPyramidShader.use();
        
    // Henter og setter texture som sendes til cube-fragshader
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i(cubeTextureLoc , 0);
    glBindTexture( GL_TEXTURE_2D, pyramidTextureValue );
    
    // Henter og setter normalMap som sendes til cube-fragshader
    glActiveTexture( GL_TEXTURE1 );
    glUniform1i(normalMapLoc , 1);
    glBindTexture( GL_TEXTURE_2D, pyramidNormalMapValue );
    
    // Henter og setter normalMap som sendes til cube-fragshader
    glActiveTexture( GL_TEXTURE2 );
    glUniform1i(depthMapLoc , 2);
    glBindTexture( GL_TEXTURE_2D, pyramidDepthMapValue );
    

    // Skalerer slik at pyramiden blir dobbelt så stor. Deretter setter matrisen
    glm::mat4 viewtriangleValue = glm::scale(viewValue, glm::vec3(2.0f, 2.0f, 2.0f));
    // Sender view-matrise til cube-shaderen:
    glUniformMatrix4fv( viewLoc, 1, GL_FALSE, glm::value_ptr( viewtriangleValue ) );

    // Setter model-matrise
    glm::mat4 modeltriangleValue = glm::mat4(1.0f);
    modeltriangleValue = glm::rotate(modeltriangleValue, time * 0.5f, glm::vec3(0.0f, 1.0f,  0.0f));
    // Sender model-matrise til cube-shaderen:
    glUniformMatrix4fv( modelLoc, 1, GL_FALSE, glm::value_ptr( modeltriangleValue ) );
    

    setLightMatricesForCubeAndPyramide();
    
    // Aktiverer vertex-arrayen for kuben
    glBindVertexArray( pyramidVAO );
    // Deretter tegnes trianglene
    glDrawArrays( GL_TRIANGLES, 0, 18 );  // 6 trekanter - 3 punkter

    // Deaktiverer shaderprogram som brukes og vertexarray
    glBindVertexArray(0);
    glUseProgram(0);
}

void drawHexPrism() {
    
    float time = glfwGetTime();
    
    // Aktiverer shader-programmet
    hexPrismShader.use();
        
    // Setter texture for prismen og sender til shader
    glActiveTexture( GL_TEXTURE0 );
    glUniform1i(textureLochexPrism , 0);
    glBindTexture( GL_TEXTURE_2D, hexPrismTextureValue );

    // Sender view-matrise til shaderen:
    glUniformMatrix4fv( viewLochexPrism, 1, GL_FALSE, glm::value_ptr( viewValue ) );

    // Setter model-matrise
    glm::mat4 modelhexPrismValue = glm::mat4(1.0);
    // Flytter kubens origo 4 punkter til høyre
    modelhexPrismValue = glm::translate(modelhexPrismValue, glm::vec3(4.0, 0.0, 0.0));
    // Sender model-matrise til shaderen:
    glUniformMatrix4fv( modelLochexPrism, 1, GL_FALSE, glm::value_ptr( modelhexPrismValue ) );
    

    // Sender lys-matrisene til prisme-shaderen:
    //Lys 1:
    glm::vec3 lightPositionOneValue(sinf(time * 1.0f), cosf(time * 1.0f), 0.8f);
    glUniform3f(lightPositionOneLochexPrism, lightPositionOneValue.x, lightPositionOneValue.y, lightPositionOneValue.z);
    glUniform3fv(lightColorOneLochexPrism, 1, lightColorOneValue);
    //Lys 2:
    glUniform3fv(lightPositionTwoLochexPrism, 1, lightPositionTwoValue);
    glUniform3fv(lightColorTwoLochexPrism, 1, lightColorTwoValue);
    
    glUniform3f(viewPositionLochexPrism, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    
    // Aktiverer vertex-arrayen for kuben:
    glBindVertexArray( hexPrismVAO );
       
    // Deretter tegnes trianglene:
    glDrawArrays( GL_TRIANGLES, 0, 84 ); // 22 trekanter a 3 punkter
    
    // Deaktiverer shaderprogram som brukes og vertexarray
    glUseProgram(0);
    glBindVertexArray(0);
}


/* Endrer størrelse og projectionmatriser ved endring av vindu */
void resizeGL(int width, int height) {
    
    // Feilhåndtering for å hindre at det blir deling på 0
    if (height == 0)
        height = 1;
    
    glm::mat4 projectionValue = glm::perspective(3.14f/2.0f, (float)width/height, 0.1f, 100.0f);
    
    cubeAndPyramidShader.use();
    glUniformMatrix4fv( projLoc, 1, GL_FALSE, glm::value_ptr( projectionValue ) );
    
    hexPrismShader.use();
    glUniformMatrix4fv( projLochexPrism, 1, GL_FALSE, glm::value_ptr( projectionValue ) );
    

    
    // Definerer viewport-dimensjonene
    glViewport(0, 0, width, height); // 2.0
    
}

/* Setter diverse lys-matriser for kube og pyramide */
void setLightMatricesForCubeAndPyramide() {
    
    float time = glfwGetTime();
    
    // Sender lys-matrisene til cube-shaderen:
    //Lys 1:
    glm::vec3 lightPositionOneValue(sinf(time * 1.0f), cosf(time * 1.0f), 0.8f);
    glUniform3f(lightPositionOneLoc, lightPositionOneValue.x, lightPositionOneValue.y, lightPositionOneValue.z);
    glUniform3fv(lightColorOneLoc, 1, lightColorOneValue);
    //Lys 2:
    glUniform3fv(lightPositionTwoLoc, 1, lightPositionTwoValue);
    glUniform3fv(lightColorTwoLoc, 1, lightColorTwoValue);
    
    glUniform3f(viewPositionLoc, camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

}


/* Error callback-funksjon for GLFW */
static void glfwErrorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

/* Window size changed callback-funksjon for GLFW */
void glfwWindowSizeCallback(GLFWwindow* window, int width, int height) {
    resizeGL(width, height);
}

// Funksjon som kalles hver gang en tast presses ned eller slippes opp
void keyCallback( GLFWwindow *window, int key, int scancode, int action, int mode) {
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

// Funksjon som kalles hver gang musen beveger seg
void mouseCallback( GLFWwindow *window, double xPos, double yPos ) {
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
    
    camera.processMouseMovement( xOffset, yOffset );
}

// Funksjon som setter kamera riktig etter hvilke knapper som er presset ned
void doMovement( ) {
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.processKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.processKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.processKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.processKeyboard( RIGHT, deltaTime );
    }
    if ( keys[GLFW_KEY_Z] )
    {
        camera.processKeyboard( DOWN, deltaTime );
    }
    if ( keys[GLFW_KEY_X] )
    {
        camera.processKeyboard( UP, deltaTime );
    }
}
