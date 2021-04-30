#include <SFML/Graphics.hpp>

#include "uv.cpp"

using namespace sf;

class Tileset {

    private:
    Vector2i vDimensions;
    Vector2i vTile;

    Vector2f indexToVector( int iIndex );

    public:
    Tileset( std::string url, Vector2i tileSize );
    Texture tTexture;

    void loadTexture( std::string url );
    Texture getTexture();
    UV getSquareUV( Vector2f vOffset );
    UV getSliverUV( float iIndex, Vector2f vOffset );

};

// Constructor
Tileset::Tileset( std::string url, Vector2i tileSize ) {

    loadTexture( url );

    vTile = Vector2i( tileSize );

}

void Tileset::loadTexture( std::string url ) {

    tTexture.loadFromFile( url );
	tTexture.setRepeated( true );

    vDimensions = Vector2i( tTexture.getSize() );

}

Vector2f Tileset::indexToVector( int iIndex ) {

    return Vector2f(
        ( iIndex % vTile.x ) * vTile.x,
        ( iIndex / vTile.y ) * vTile.y
    );

}

UV Tileset::getSquareUV( Vector2f vOffset ) {

    UV uv(
        (float)vOffset.x * vTile.x / vDimensions.x,
        ( vDimensions.y - (float)vOffset.y * vTile.y ) / vDimensions.y,
        ( (float)vOffset.x * vTile.x + vTile.x ) / vDimensions.x,
        ( vDimensions.y - (float)vOffset.y * vTile.y - vTile.y ) / vDimensions.y
    );

    return uv;

}

UV Tileset::getSliverUV( float iIndex, Vector2f vOffset ) {

    UV uv(
        (float)vOffset.x * vTile.x / vDimensions.x,
        ( vDimensions.y - (float)vOffset.y * vTile.y ) / vDimensions.y,
        ( (float)vOffset.x * vTile.x + vTile.x ) / vDimensions.x,
        ( vDimensions.y - (float)vOffset.y * vTile.y - vTile.y ) / vDimensions.y
    );

    return uv;

}
