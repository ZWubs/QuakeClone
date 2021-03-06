
using namespace sf;

#include "tileset.cpp"
#include "palette.cpp"

struct Warp {
    Vector2f from;
    Vector2f to;
    int map = -1;
    float rotation;
};

class Map {

    private:
	// std::string sName;
    std::string sUrl;
    Vector2i vDimensions;
    std::vector<std::string> vsMap;
    std::vector<Warp> wWarps;

    public:
	std::string sName;
    Map();
    Map( std::string src );
    void activate();

    Vector2i getDimensions();

    char getChar( Vector2i position );
    PaletteItem getTile( Vector2i position );
    UV getTileUV( Vector2i position, float intersection );

    Warp checkWarps( Vector2f position );

	void destroyTile( Vector2i position );

    Tileset tTileset;
    Palette pPalette;
    int iMusic = -1;
    std::vector<char> vcDoors;
    std::vector<float> viSkyColor;
    std::vector<float> viFloorColor;

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

    if( j.find("music") != j.end() )
        iMusic = j["music"];

    for(auto &array : j["palette"].items()) {
		pPalette.addTile( array.key(), Vector2i( array.value()[0], array.value()[1] ) );
	}

    if( j.find("doors") != j.end() ) {
        for( int i = 0; i < j["doors"].size(); i++ ) {
            std::string doorString = j["doors"][i];
            char door = doorString.at(0);
            vcDoors.push_back( door );
        }
    }

    for(auto &array : j["warps"].items()) {
        Warp warp;
        json val = array.value();
        warp.from.x = val["from"][0];
        warp.from.y = val["from"][1];
        warp.to.x = val["to"][0];
        warp.to.y = val["to"][1];
        warp.map = val["map"];
        if( val.find("rot") != val.end() )
            warp.rotation = val["rot"];
        wWarps.push_back( warp );
	}

    if( j.find("skyColor") != j.end() )
        viSkyColor = j["skyColor"].get<std::vector<float>>();
    else
        viSkyColor = { 0, 0, 0 };

    if( j.find("floorColor") != j.end() )
        viFloorColor = j["floorColor"].get<std::vector<float>>();
    else
        viFloorColor = { 0, 0, 0 };

	vsMap = j["map"].get<std::vector<std::string>>();

    tTileset = Tileset( j["texture"], Vector2i( j["tileset"][0], j["tileset"][0] ) );

}

void Map::activate() {

    glEnable(GL_TEXTURE_2D);

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

Warp Map::checkWarps( Vector2f position ) {

    Vector2i vTile = Vector2i( position );

    for( int i = 0; i < wWarps.size(); i++ ) {

        if( vTile.x == wWarps[i].from.x && vTile.y == wWarps[i].from.y ) {

            return wWarps[i];

        }

    }

    Warp wDefault;

    return wDefault;

}

UV Map::getTileUV( Vector2i position, float intersection ) {

    PaletteItem piTile = getTile( position );

    return tTileset.getSliverUV( piTile.position, fmod( intersection, 1.0 ) );

}

// void Map::newWall( Vector2i position, int iPalette ){}

void Map::destroyTile( Vector2i position ) {
	// vsMap[ position.y ].at( position.x ) = " ";
	vsMap[ position.y ].replace( position.x, 1, " ");
}
