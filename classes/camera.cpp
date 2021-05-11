#include <SFML/Graphics.hpp>

using namespace sf;

class Camera {

    public:
        Camera();
        Camera( Vector2i vResolution );

        float fFOV = 3.14159f / 2.0f;

        Vector2i vResolution = Vector2i( 320, 240 );
        Vector2f vPosition = Vector2f( 0.0f, 0.0f );
        float fRotation = 0.0f;
        Vector2f vRayDirection = Vector2f( 0.0f, 0.0f );
        float fSpeed = 2.5f;

        void update();
        Vector2f getRayDir( float a, int x );
		Vector2i tilePosition();

};

Camera::Camera() {}

// Constructor
Camera::Camera( Vector2i resolution ) {

    vResolution.x = resolution.x;
    vResolution.y = resolution.y;

}

void Camera::update() {

    vRayDirection = Vector2f( cos( fRotation ), sin( fRotation ) );

}


Vector2f Camera::getRayDir( float angle, int x ) {

    float fRayAngle = ( angle - fFOV / 2.0f ) + ( (float)x / (float)vResolution.x ) * fFOV;

    return Vector2f( cos( fRayAngle ), sin( fRayAngle ) );

}

Vector2i Camera::tilePosition() {
	return Vector2i( floor(vPosition.x), floor(vPosition.y) );
}
