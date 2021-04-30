#include <SFML/Graphics.hpp>

using namespace sf;

class Camera {

private:
    float fFov;
    Vector2i vResolution;
    Vector2f vPosition;
    Vector2f vPlane;

public:
    Camera();

    Vector2f getRayDir( float a, int x );

};

// Constructor
Camera::Camera() {

    fFov = 60;

    vResolution = Vector2i( 320, 240 );
    vPosition = Vector2f( 0.0, 0.0 );
    vPlane = Vector2f( 0.0, 0.0 );

}


Vector2f Camera::getRayDir( float angle, int x ) {

    float fRayAngle = ( angle - fFov / 2.0f ) + ( (float)x / (float)vResolution.x ) * fFov;

    return Vector2f( cos( fRayAngle ), sin( fRayAngle ) );;

}
