#include <SFML/Graphics.hpp>

using namespace sf;

class Camera {

    public:
        Camera();
        Camera( Vector2i vResolution );

        float fFOV = 3.14159f / 3.0f;

        Vector2i vResolution = Vector2i( 320, 240 );
        Vector2f vPosition = Vector2f( 0.0f, 0.0f );
        float fRotation = 0.0f;
        float fSpeed = 2.0f;

        Vector2f getRayDir( float a, int x );

};

Camera::Camera() {}

// Constructor
Camera::Camera( Vector2i resolution ) {

    vResolution.x = resolution.x;
    vResolution.y = resolution.y;

}


Vector2f Camera::getRayDir( float angle, int x ) {

    float fRayAngle = ( angle - fFOV / 2.0f ) + ( (float)x / (float)vResolution.x ) * fFOV;

    return Vector2f( cos( fRayAngle ), sin( fRayAngle ) );;

}
