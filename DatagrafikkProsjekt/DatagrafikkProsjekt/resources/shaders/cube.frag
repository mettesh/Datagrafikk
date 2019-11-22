#version 330 core

// Får inn koordinater til hvor texture skal legges fra vert-shader
in vec2 cubeTextureCoordinates;


// Får innn farge fra (light.vert)
in vec2 interpolatedColor;
in vec3 Normal;
in vec3 worldVertex;


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


// Henter inn cube-texturen
uniform sampler2D cubeTexture;


// Sender ut resultat
out vec4 textureResult;


void main()
{
    /*** TEXTURE ****/
    // Setter texture med innlasted bilde(texture) og korrdinater hvor det skal settes
    textureResult = texture(cubeTexture, cubeTextureCoordinates);

    
    /*** LYS ****/
    
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
    ambient = vec4(lightAmbient, 1) * textureResult;
    
    // Calculate the diffuse component
    diffuse = vec4(max(dot(L, NN), 0.0) * lightDiffuse, 1) * textureResult;
    
    // Calculate the specular component
    specular = vec4(pow(max(dot(R, V), 0.0), shininess) * lightSpecular, 1) * shininessColor;
    
    // Put it all together
    outputLightColor = ambient + diffuse + specular;
    
}
