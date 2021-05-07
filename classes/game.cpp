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
    Map *mMaps;
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

	glClearColor(1.0,0.3,0.3,1.0);

    glEnable(GL_TEXTURE_2D);
    glLineWidth(2);


    // Setup Player
    cCamera.vPosition.x = j["player"]["position"][0];
    cCamera.vPosition.y = j["player"]["position"][1];

    cCamera.fRotation = j["player"]["rotation"];

    // Setup & Load Maps & Warps
    mMaps = new Map[ j["maps"].size() ];

    for( auto &array : j["maps"].items() ) {
        mMaps[ std::stoi( array.key() ) ] = Map( array.value() );
    }

    mMaps[currentMap].activate();

}

void Game::loop() {

    while ( wWindow.isOpen() ) {

        sf::Time dt = cClock.restart();

        sf::Event event;
		while ( wWindow.pollEvent( event ) ) {
			if ( event.type == sf::Event::Closed )
				wWindow.close();
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
			if ( mMaps[ currentMap ].getChar( Vector2i( cCamera.vPosition ) ) != ' ' ) {
				cCamera.vPosition.x -= cosf( cCamera.fRotation ) * cCamera.fSpeed * dt.asSeconds();
				cCamera.vPosition.y -= sinf( cCamera.fRotation ) * cCamera.fSpeed * dt.asSeconds();
			}
		}

        Warp newWarp = mMaps[ currentMap ].checkWarps( cCamera.vPosition );

        if( newWarp.map != -1 ) {
            currentMap = newWarp.map;
            mMaps[currentMap].activate();
            cCamera.vPosition.x = newWarp.to.x;
            cCamera.vPosition.y = newWarp.to.y;
        }

        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT);

        rRenderer.render( cCamera, mMaps[ currentMap ] );

        // Display
		wWindow.display();

    }

}
