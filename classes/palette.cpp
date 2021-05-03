#include <SFML/Graphics.hpp>

#include "uv.cpp"

using namespace sf;

class Tileset {

    private:
    

    public:
    Tileset( std::string url, Vector2i tileSize );

};

// Constructor
Tileset::Tileset( std::string url, Vector2i tileSize ) {

    loadTexture( url );

    vTile = Vector2i( tileSize );

}
