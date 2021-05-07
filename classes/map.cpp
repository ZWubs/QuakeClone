
using namespace sf;

#include "tileset.cpp"
#include "palette.cpp"

struct Warp {
    Vector2f from;
    Vector2f to;
    int map;
};

class Map {

    private:
    std::string sName;
    std::string sUrl;
    Vector2i vDimensions;
    std::vector<std::string> vsMap;
    Warp *wWarps;

    public:
    Map();
    Map( std::string src );
    void activate();

    Vector2i getDimensions();

    char getChar( Vector2i position );
    PaletteItem getTile( Vector2i position );
    UV getTileUV( Vector2i position, float intersection );

    Tileset tTileset;
    Palette pPalette;

};

Map::Map() {}

Map::Map( std::string src ) {

    std::ifstream ifs;
    json j;

    try {
        ifs = std::ifstream( src );
        j = json::parse( ifs );
    }
    catch( std::exception& e ) {
        std::cout << e.what() << std::endl;
        std::cout << "ERROR: Something may be wrong with file \"" << src << "\"" << std::endl;
        exit(0);
    }

    sName = j["name"];
    sUrl = src;

    vDimensions.x = j["dimensions"][0];
    vDimensions.y = j["dimensions"][1];

    for(auto &array : j["palette"].items()) {
		pPalette.addTile( array.key(), Vector2i( array.value()[0], array.value()[1] ) );
	}
    /*
    wWarps = new Warp[ j["warps"].size() ];

    for(auto &array : j["warps"].items()) {
        Warp warp = wWarps[ std::stoi( array.key() ) ];
        warp.from.x = array.value()["pos"][0];
        warp.from.y = array.value()["pos"][1];
        warp.to.x = array.value()["warp"][0];
        warp.to.y = array.value()["warp"][1];
        warp.map.y = array.value()["map"];
	}*/

	vsMap = j["map"].get<std::vector<std::string>>();

    tTileset = Tileset( j["texture"], Vector2i( j["tileset"][0], j["tileset"][0] ) );

}

void Map::activate() {

    sf::Texture::bind( &tTileset.tTexture );

}

Vector2i Map::getDimensions() {

    return Vector2i( vDimensions );

}

char Map::getChar( Vector2i position ) {

    return vsMap[ position.y ].at( position.x );

}

PaletteItem Map::getTile( Vector2i position ) {

    char mapChar = getChar( position );

    return pPalette.getTile( mapChar );

}
/*
Warp Map::checkWarp( Vector2f position ) {

    return wWarps[0];

}
*/
UV Map::getTileUV( Vector2i position, float intersection ) {

    PaletteItem piTile = getTile( position );

    return tTileset.getSliverUV( piTile.position, fmod( intersection, 1.0 ) );

}
