/*
	JSON
*/
#include "include/json.hpp"
using json = nlohmann::json;

/*
	OpenGL
*/
#include <GL/glut.h>

/*
    SFML
*/
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>

using namespace sf;

/*
    Game Engine
*/
#include "map.cpp"
#include "renderer.cpp"

class Game {

    private:
    std::string sTitle;

    Window wWindow;
    Renderer rRenderer;
    Camera cCamera;
    Clock cClock;
    Map *aMaps;
    Music *aMusic;
    int currentMap = 0;

    public:
    Game( std::string url );
    void loop();

};

Game::Game( std::string url ) {

    // Load & Check JSON
    std::ifstream ifs;
    json j;

    try {
        ifs = std::ifstream( url );
        j = json::parse( ifs );
    }
    catch( std::exception& e ) {
        std::cout << e.what() << std::endl;
        std::cout << "ERROR: Something may be wrong with your file." << std::endl;
        exit(0);
    }

    sTitle = j["title"];

    // Setup Rendering
    wWindow.create(
        sf::VideoMode( j["window"][0], j["window"][1] ),
        sTitle,
        sf::Style::Default
    );

    wWindow.setFramerateLimit( 30 );

	wWindow.setActive( true );

	glClearColor( 0.0, 0.0, 0.0, 1.0 );

    glEnable(GL_TEXTURE_2D);
    glLineWidth(1);


    // Setup Player
    cCamera.vResolution.x = j["window"][0];
    cCamera.vResolution.y = j["window"][1];

    cCamera.vPosition.x = j["player"]["position"][0];
    cCamera.vPosition.y = j["player"]["position"][1];

    cCamera.fRotation = j["player"]["rotation"];

    // Load Map Music
    aMusic = new Music[ j["music"].size() ];

    for( auto &array : j["music"].items() ) {
        std::string url = array.value();
        aMusic[ std::stoi( array.key() ) ].openFromFile( url );
    }

    // Setup & Load Maps & Warps
    aMaps = new Map[ j["maps"].size() ];

    for( auto &array : j["maps"].items() ) {
        aMaps[ std::stoi( array.key() ) ] = Map( array.value() );
    }

	// Vector2i position{ x:3, y:0 };
	// aMaps[currentMap].destroyTile( position );

    aMaps[currentMap].activate();
    aMusic[currentMap].play();


}

void Game::loop() {

    while ( wWindow.isOpen() ) {

        sf::Time dt = cClock.restart();

        sf::Event event;
		while ( wWindow.pollEvent( event ) ) {
			if ( event.type == sf::Event::Closed )
				wWindow.close();
            else if (event.type == sf::Event::Resized)
                glViewport(0, 0, event.size.width, event.size.height);
		}

        // Keyboard Inputs
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			wWindow.close();	// TODO: Launch a menu with resume, options, and close.
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			cCamera.fRotation -= cCamera.fSpeed * 0.5 * dt.asSeconds();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			cCamera.fRotation += cCamera.fSpeed * 0.5 * dt.asSeconds();
		}

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			cCamera.vPosition.x += cosf(cCamera.fRotation) * cCamera.fSpeed * dt.asSeconds();
			cCamera.vPosition.y += sinf(cCamera.fRotation) * cCamera.fSpeed * dt.asSeconds();
			if ( aMaps[ currentMap ].getChar( Vector2i( cCamera.vPosition ) ) != ' ' ) {
				cCamera.vPosition.x -= cosf( cCamera.fRotation ) * cCamera.fSpeed * dt.asSeconds();
				cCamera.vPosition.y -= sinf( cCamera.fRotation ) * cCamera.fSpeed * dt.asSeconds();
			}
		}

        Warp newWarp = aMaps[ currentMap ].checkWarps( cCamera.vPosition );

        if( newWarp.map != -1 ) {
            if( aMaps[ currentMap ].iMusic != aMaps[ newWarp.map ].iMusic ) {
                aMusic[ aMaps[ currentMap ].iMusic ].stop();
                aMusic[ aMaps[ newWarp.map ].iMusic ].play();
                aMusic[ aMaps[ newWarp.map ].iMusic ].setLoop( true );
            }
            currentMap = newWarp.map;
            aMaps[currentMap].activate();
            cCamera.vPosition.x = newWarp.to.x + 0.5f;
            cCamera.vPosition.y = newWarp.to.y + 0.5f;
            cCamera.fRotation = newWarp.rotation * ( 3.14159f / 2.0f );
        }

		// --== Hardcoading for lawnmowing game. ==--
		if (/*sTitle == "Field Day" && */aMaps[currentMap].sName == "field") {
			// std::printf("it is\n");
			// Clear grass from camera's 4 directions.
			Vector2i pl = cCamera.tilePosition();	// pl = player location
			// std::cout << pl.x << pl.y << "\n";
			// std::cout << aMaps[currentMap].getChar(pl) << "\n";
			// std::cout << aMaps[currentMap].getChar(Vector2i(pl.x+1, pl.y)) << "\n";
			// std::cout << (aMaps[currentMap].getChar(Vector2i(pl.x+1, pl.y)) == 'g') << "\n";
			// if ( aMaps[currentMap].getChar(Vector2i(pl.x+1, pl.y)) == 'g' ) { std::cout << "there's grass at x+1" << "\n"; }

			if ( aMaps[currentMap].getChar(Vector2i(pl.x+1, pl.y)) == 'g' ) { aMaps[currentMap].destroyTile(Vector2i(pl.x+1, pl.y));  }
			if ( aMaps[currentMap].getChar(Vector2i(pl.x-1, pl.y)) == 'g' ) { aMaps[currentMap].destroyTile(Vector2i(pl.x-1, pl.y)); }
			if ( aMaps[currentMap].getChar(Vector2i(pl.x, pl.y+1)) == 'g' ) { aMaps[currentMap].destroyTile(Vector2i(pl.x, pl.y+1)); }
			if ( aMaps[currentMap].getChar(Vector2i(pl.x, pl.y-1)) == 'g' ) { aMaps[currentMap].destroyTile(Vector2i(pl.x, pl.y-1)); }

		}

        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);

        rRenderer.render( cCamera, aMaps[ currentMap ] );

        // Display
		wWindow.display();

    }

}
