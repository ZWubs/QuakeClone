#include <SFML/Graphics.hpp>

using namespace sf;

#include "camera.cpp"

struct DDAVariables {

    // Calculated Ray Angle
    float fRayAngle;

    // Calculate Ray Direction
    Vector2f vRayDir;

    // Current grid position
    sf::Vector2i vMapPosition;

    // Direction to step in
    sf::Vector2f vStep;

    // Length from current position to the next side
    sf::Vector2f vSideDistance;

    // Length from one grid square to the next
    sf::Vector2f vDeltaDistance;

};

struct DDAReturn {

    // Distance from player to ray intersection
    float fRayDistance = 0.0f;

    // Boolean of a wall being hit
    bool bHit = false;

    // Intersection point on wall of ray
    Vector2f vIntersection = Vector2f( 0.0f, 0.0f );

    // Tile of intersection point
    Vector2i vTile = Vector2i( 0, 0 );

    // North South or East West
    int iSide = 0;

};

class Renderer {

    public:
        Renderer();
        void render( Camera camera, Map map );
        DDAReturn DDA( Camera camera, Map map, DDAVariables variables );

};

Renderer::Renderer() {}

void Renderer::render( Camera c, Map m  ) {

    DDAVariables v;

    for( int x = 0; x < c.vResolution.x; x++ ) {

        v.vMapPosition = sf::Vector2i( c.vPosition );

        v.fRayAngle = ( c.fRotation - c.fFOV / 2.0f ) + ( (float)x / (float)c.vResolution.x ) * c.fFOV;
        v.vRayDir = Vector2f( cos( v.fRayAngle ), sin( v.fRayAngle ) );

        v.vDeltaDistance = sf::Vector2f( abs( 1.0f / v.vRayDir.x), abs( 1.0f / v.vRayDir.y ) );
        v.vStep = sf::Vector2f( copysign( 1.0, v.vRayDir.x ), copysign( 1.0f, v.vRayDir.y ) );

        v.vSideDistance.x = ( v.vRayDir.x < 0 ) ? ( ( c.vPosition.x - v.vMapPosition.x ) * v.vDeltaDistance.x ) : ( ( v.vMapPosition.x + 1.0 - c.vPosition.x ) * v.vDeltaDistance.x );
        v.vSideDistance.y = ( v.vRayDir.y < 0 ) ? ( ( c.vPosition.y - v.vMapPosition.y ) * v.vDeltaDistance.y ) : ( ( v.vMapPosition.y + 1.0 - c.vPosition.y ) * v.vDeltaDistance.y );

        DDAReturn r = DDA( c, m, v );

        if( r.bHit ) {

            float fOffset = ( r.iSide == 0 ) ? ( r.vIntersection.y ) : ( r.vIntersection.x );
            UV uv = m.getTileUV( r.vTile, fOffset );

            r.fRayDistance *= cosf( c.fRotation - v.fRayAngle );

            if( r.iSide == 0 ) glColor3f( 1.0, 1.0, 1.0 );
            else glColor3f( 0.9, 0.9, 0.9 );

            glBegin(GL_LINES);

            glTexCoord2f( uv.LT.x, uv.LT.y );
            glVertex2f( (float)x / c.vResolution.x * 2.0 - 1.0, 1.0 / ( r.fRayDistance ) );
            glTexCoord2f( uv.LB.x, uv.LB.y );
            glVertex2f( (float)x / c.vResolution.x * 2.0 - 1.0, -1.0 / ( r.fRayDistance ) );

            glEnd();

        }

    }

}

DDAReturn Renderer::DDA( Camera c, Map m, DDAVariables v ) {

    DDAReturn r;

    while( !r.bHit ) {

        if ( v.vSideDistance.x < v.vSideDistance.y ) {
            v.vMapPosition.x += v.vStep.x;
            r.fRayDistance = v.vSideDistance.x;
            v.vSideDistance.x += v.vDeltaDistance.x;
            r.iSide = 0;
        }
        else {
            v.vMapPosition.y += v.vStep.y;
            r.fRayDistance = v.vSideDistance.y;
            v.vSideDistance.y += v.vDeltaDistance.y;
            r.iSide = 1;
        }

        if( ( ( v.vMapPosition.x >= 0 ) && ( v.vMapPosition.x < m.getDimensions().x ) ) && ( ( v.vMapPosition.y >= 0 ) && ( v.vMapPosition.y < m.getDimensions().y ) ) ) {

            if ( m.getChar( v.vMapPosition ) != ' ' ) {

                r.bHit = true;

                r.vTile = v.vMapPosition;

            }

        }

        else
            break;

    }

    r.vIntersection = c.vPosition + v.vRayDir * r.fRayDistance;

    return r;

}
