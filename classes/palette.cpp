using namespace sf;

struct PaletteItem {
    std::string id;
    Vector2i position;
};

class Palette {

    private:
    PaletteItem items[256];

    public:
    Palette();
    void addTile( std::string id, Vector2i position );
    PaletteItem getTile( std::string id );

};

// Constructor
Palette::Palette() {}

void Palette::addTile( std::string id, Vector2i position ) {

    for( int i = 0; i < 256; i++ ) {

        if( items[i].id == "" ) {

            items[i].id = id;
            items[i].position = position;

            break;

        }

    }

}
PaletteItem Palette::getTile( std::string id ) {

    for( int i = 0; i < 256; i++ ) {

        if( items[i].id == id ) {

            return items[i];

        }

    }

    return items[0];

}
