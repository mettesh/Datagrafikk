#version 330

// Incoming variables.

// Får innn farge fra light.vert
in vec3 interpolatedColor;

// Får inn. . .
in vec3 Normal;
in vec3 worldVertex;

/* Kode som kommer inn fra main.cpp
glm::vec3 lightPosition( 2.2f, 1.0f, 2.0f );
GLfloat lightAmbient[] { 0.1f, 0.1f, 0.2f };
GLfloat lightDiffuse[] { 0.5f, 0.5f, 0.5f };
GLfloat lightSpecular[] { 0.6f, 0.6f, 0.6f };
GLfloat materialShininessColor[] { 1.0f, 1.0f, 1.0f,  1.0f };
GLfloat materialShininess = 32.0f;
GLfloat cameraPosition[] { 0.0f, 0.0f, 4.0f };
*/

// Matriser som blir satt i main.cpp:
uniform vec3 lightPosition;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;

uniform vec4 shininessColor;
uniform float shininess;
uniform vec3 cameraPosition;

// Endelig farge på lys sendes ut
out vec4 outputLightColor;

// Vectors
vec3 L;
vec3 NN;
vec3 V;
vec3 R;

// Colors
vec4 lightColor;
vec4 ambient;
vec4 diffuse;
vec4 specular;


void main()
{
    // Settr lightColor til å være farg man har fått sendt inn
    //lightColor = vec4(interpolatedColor, 1);
    
    // Normalize the interpolated normal to ensure unit length
    NN = normalize(Normal);
    
    // Find the unit length normal giving the direction from the vertex to the light
    L = normalize(lightPosition - worldVertex);
    
    // Find the unit length normal giving the direction from the vertex to the camera
    V = normalize(cameraPosition - worldVertex);
    
    // Find the unit length reflection normal
    R = normalize(reflect(-L, NN));
    
    // Calculate the ambient component
    ambient = vec4(lightAmbient, 1) * lightColor;
    
    // Calculate the diffuse component
    diffuse = vec4(max(dot(L, NN), 0.0) * lightDiffuse, 1) * lightColor;
    
    // Calculate the specular component
    specular = vec4(pow(max(dot(R, V), 0.0), shininess) * lightSpecular, 1) * shininessColor;
    
    // Put it all together
    outputLightColor = ambient + diffuse + specular;
    
}
