#include "uv.cpp"

using namespace sf;

class Tileset {

    private:
    Vector2i vDimensions;
    Vector2i vTile;

    Vector2f indexToVector( int iIndex );

    public:
    Tileset();
    Tileset( std::string url, Vector2i tileSize );
    Texture tTexture;

    void loadTexture( std::string url );
    Texture getTexture();
    UV getSquareUV( Vector2f vOffset );
    UV getSliverUV( Vector2i vTileOffset, float fOffset );

};

// Constructor
Tileset::Tileset() {}

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
        ( iIndex % ( vDimensions.x / vTile.x ) ),
        ( iIndex / ( vDimensions.y / vTile.y ) )
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

/*
    @param iIndex (int) index of tile
    @param fOffset (float) float Offset of sliver
*/
UV Tileset::getSliverUV( Vector2i vTileOffset, float fOffset ) {

    UV uv(
        ( (float)vTileOffset.x + fOffset ) * vTile.x / vDimensions.x,
        ( (float)vTileOffset.y * vTile.y ) / vDimensions.y,
        ( ( (float)vTileOffset.x + fOffset ) * vTile.x + vTile.x ) / vDimensions.x,
        ( (float)vTileOffset.y * vTile.y + vTile.y ) / vDimensions.y
    );

    return uv;

}
