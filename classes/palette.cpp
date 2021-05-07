using namespace sf;

struct PaletteItem {
    char id = '\0';
    Vector2i position;
};

class Palette {

    private:
    PaletteItem items[256];

    public:
    Palette();
    void addTile( std::string id, Vector2i position );
    PaletteItem getTile( char id );

};

// Constructor
Palette::Palette() {}

void Palette::addTile( std::string id, Vector2i position ) {

    for( int i = 0; i < 256; i++ ) {

        if( items[i].id == '\0' ) {

            items[i].id = id.at(0);
            items[i].position = position;

            break;

        }

    }

}
PaletteItem Palette::getTile( char id ) {

    for( int i = 0; i < 256; i++ ) {

        if( items[i].id == id ) {

            return items[i];

        }

    }

    return items[0];

}
