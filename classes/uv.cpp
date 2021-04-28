#include <SFML/Graphics.hpp>

using namespace sf;

class UV {

    public:
    UV( float l, float t, float r, float b );
    Vector2f LT;
    Vector2f RT;
    Vector2f LB;
    Vector2f RB;

};

// Constructor
UV::UV( float l, float t, float r, float b ) {

    LT = Vector2f( l, t );
    RT = Vector2f( r, t );
    LB = Vector2f( l, b );
    RB = Vector2f( r, b );

}
