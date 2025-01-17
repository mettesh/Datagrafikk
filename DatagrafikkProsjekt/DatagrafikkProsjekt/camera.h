#pragma once
#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Definerer alle mulige veier som kamera skal bevege seg + Høyden på parallax scalering. Bruker disse for å kunne forholde meg til navn isteden for tall
enum Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    MORE,
    LESS
};

// Forhåndsdefinerte kameraverdier.
const GLfloat YAW           = -90.0f; // Retning kamera skal peke
const GLfloat PITCH         =  0.0f;   // Vinkel
const GLfloat SPEED         =  6.0f;   // Hvor fort kameraet skal bevege seg
const GLfloat SENSITIVTY    =  0.25f;  // Hvor sensitiv bevegelsene skal være
const GLfloat ZOOM          =  46.0f;  // Hvor inn-zoomet kamera skal starte
const GLfloat HEIGHTSCALE   = 0.1;


// Oppretter en kamera-klasse som skal motta input og regne ut vinkler, vectorer og matriser som skal brukes i OpenGL
class Camera
{
public:
    // Konstruktør for vektor-verdier (Retning og hastighet)
    Camera( glm::vec3 position = glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3 up = glm::vec3( 0.0f, 1.0f, 0.0f ), GLfloat yaw = YAW, GLfloat pitch = PITCH ) : front( glm::vec3( 0.0f, 0.0f, -1.0f ) ), movementSpeed( SPEED ), mouseSensitivity( SENSITIVTY ), zoom( ZOOM ), heighScale( HEIGHTSCALE )
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors( );
    }
    
    // Returnerer matrise utregnet
    glm::mat4 getViewMatrix( )
    {
        return glm::lookAt( this->position, this->position + this->front, this->up );
    }
    
    // Prosesserer input den mottar fra tastatur: (deltaTime brukes for å sørge for en jevn bevegelse)
    void processKeyboard( Camera_Movement direction, GLfloat deltaTime )
    {
        GLfloat speed = this->movementSpeed * deltaTime;
        
        if ( direction == FORWARD )
        {
            this->position += this->front * speed;
            
            // Minker zoom-verdien for å få en illusjon av at man nærmer seg "skybox-bilde"
            if ( this->zoom >= 1.0f && this->zoom <= 50.0f )
            {
                this->zoom -= 0.00015f;
            }
            
            if ( this->zoom <= 1.0f )
            {
                this->zoom = 1.0f;
            }
            
            if ( this->zoom >= 50.0f )
            {
                this->zoom = 50.0f;
            }
        }
        
        if ( direction == BACKWARD )
        {
            this->position -= this->front * speed;
            
            // Øker zoom-verdien for å få en illusjon av at man nærmer seg "skybox-bilde"
            if ( this->zoom >= 1.0f && this->zoom <= 50.0f )
            {
                this->zoom += 0.00015f;
            }
            if ( this->zoom <= 1.0f )
            {
                this->zoom = 1.0f;
            }
            
            if ( this->zoom >= 50.0f )
            {
                this->zoom = 50.0f;
            }
        }
        
        if ( direction == LEFT )
        {
            this->position -= this->right * speed;
        }
        
        if ( direction == RIGHT )
        {
            this->position += this->right * speed;
        }
        if ( direction == DOWN )
            
        {
            this->position -= this->up * speed;
        }
        if ( direction == UP )
        {
            this->position += this->up * speed;
        }
        

        if ( direction == MORE )
        {
            this->heighScale += 0.0001;
        }
        if ( direction == LESS )
        {
            // Sjekker slik at den ikke passerer 0 og går utover
            if(this->heighScale <= 0.0){
                this->heighScale = 0.0;
            }
            
            this->heighScale -= 0.0001;
        }
    }
    
    // Prosesserer input den mottar fra mus. (Setter y og x-posisjon)
    void processMouseMovement( GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true )
    {
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;
        
        this->yaw   += xOffset;
        this->pitch += yOffset;
        
        // Sørger for at universet ikke "snus" om man går utenfor grensene til skjermen
        if ( constrainPitch )
        {
            if ( this->pitch > 89.0f )
            {
                this->pitch = 89.0f;
            }
            
            if ( this->pitch < -89.0f )
            {
                this->pitch = -89.0f;
            }
        }
        
        // oppdaterer vektorene
        this->updateCameraVectors( );
    }
    
    glm::vec3 getPosition()
    {
        return this->position;
    }
    
    GLfloat getZoom( )
    {
        return this->zoom;
    }
    
    GLfloat getParallaxHeightScale( )
    {
        return this->heighScale;
    }
    
private:
    // Kamera-attributter
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    GLfloat yaw;
    GLfloat pitch;
    
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;
    
    GLfloat heighScale;
    
    // Kalkulerer front-vektoren fra kameraets (nye) vinkel.
    void updateCameraVectors( )
    {
        // Calculate the new Front vector
        glm::vec3 front;
        front.x = cos( glm::radians( this->yaw ) ) * cos( glm::radians( this->pitch ) );
        front.y = sin( glm::radians( this->pitch ) );
        front.z = sin( glm::radians( this->yaw ) ) * cos( glm::radians( this->pitch ) );
        this->front = glm::normalize( front );
        
        this->right = glm::normalize( glm::cross( this->front, this->worldUp ) );
        this->up = glm::normalize( glm::cross( this->right, this->front ) );
    }
};
