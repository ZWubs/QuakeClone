#include <SFML/Graphics.hpp>

#include "uv.cpp"

using namespace sf;

class Tileset {

    private:
    Vector2i vDimensions;
    Vector2i vTile;

    public:
    Tileset( std::string url, Vector2i tileSize );
    Texture tTexture;

    void loadTexture( std::string url );
    Texture getTexture();
    UV getUVs( Vector2f vOffset );

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

UV Tileset::getUVs( Vector2f vOffset ) {

    UV uv(
        (float)vOffset.x * vTile.x / vDimensions.x,
        ( vDimensions.y - (float)vOffset.y * vTile.y ) / vDimensions.y,
        ( (float)vOffset.x * vTile.x + vTile.x ) / vDimensions.x,
        ( vDimensions.y - (float)vOffset.y * vTile.y - vTile.y ) / vDimensions.y
    );

    return uv;

}
