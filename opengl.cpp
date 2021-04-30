// STD Classes
#include <iostream>
#include <fstream>
#include <math.h>
#define PI 3.1415926535

// OpenGL
#include <GL/glut.h>
// Ubuntu: sudo apt-get install freeglut3-dev
// Fedora/RedHat: sudo yum install freeglut-devel

// JSON
#include "include/json.hpp"

using json = nlohmann::json;

// SFML
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "classes/tileset.cpp"

using namespace std;

/*
	Base Variables
*/

sf::Vector2i vScreen( 320, 240 );
sf::Vector2i vMap( 4, 4 );

sf::Vector2f vPlayer( 2.0, 2.0 );
float fPlayerA = 1.0f;			// Player Start Rotation
float fFOV = 3.14159f / 3.0f;	// Field of View (3.14159f / 4.0f; = 45deg fov)
float fRenderDistance = 16.0f;	// Maximum rendering distance
float fSpeed = 2.0f;			// Walking Speed

// Default Colors
sf::Color wallColor(250, 175, 175);
sf::Color floorColor(20, 150, 50);
sf::Color skyColor(0, 100, 200);
sf::Color fogColor(0, 100, 200);

int main() {

	ifstream ifs("./game/map.json");
	json jf = json::parse(ifs);

	std::vector<std::string> map;
	map = jf["map"].get<std::vector<std::string>>();

	sf::RenderWindow window(sf::VideoMode( vScreen.x, vScreen.y), "DDA Ray Casting" , sf::Style::Close );

	window.setFramerateLimit( 30 );

	window.setActive( true );

	glClearColor(1.0,0.3,0.3,1.0);

	sf::Texture stoneTexture;
	stoneTexture.loadFromFile("./img/stone.png");

	Tileset mapTileset = Tileset( "./img/tiles.png", sf::Vector2i( 16, 16 ) );

	Tileset skyTileset = Tileset( "./img/skybox.png", sf::Vector2i( 320, 128 ) );

	//sf::Texture skyTexture;
	//skyTexture.loadFromFile("./img/skybox.png");
	//skyTexture.setRepeated( true );

	glEnable(GL_TEXTURE_2D);

	// deltaTime Clock
	sf::Clock deltaClock;

	while ( window.isOpen() ) {

		sf::Time dt = deltaClock.restart();

        // Events
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

        // Keyboard Inputs
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			window.close();	// TODO: Launch a menu with resume, options, and close.
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			fPlayerA -= fSpeed * 0.5f * dt.asSeconds();
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			fPlayerA += fSpeed * 0.5f * dt.asSeconds();
		}

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			float sign = dt.asSeconds();
			if( sf::Keyboard::isKeyPressed(sf::Keyboard::S) == true ) { sign = -sign; }
			vPlayer.x += cosf(fPlayerA) * fSpeed * sign;
			vPlayer.y += sinf(fPlayerA) * fSpeed * sign;
			// if (map.c_str()[(int)vPlayer.y * vMap.y + (int)vPlayer.x] == '#') {
			// 	vPlayer.x -= cosf(fPlayerA) * fSpeed * sign;
			// 	vPlayer.y -= sinf(fPlayerA) * fSpeed * sign;
			// }
		}

		/*
			Clear
		*/

		glClear(GL_COLOR_BUFFER_BIT);

		/*
			Floor & Ceiling Intersection Detection
		*/

		sf::Texture::bind(&skyTileset.tTexture);

		UV skyUV = skyTileset.getSquareUV( sf::Vector2f( fPlayerA / (2 * PI), 0 ) );
		glBegin(GL_QUADS);

		glTexCoord2f(skyUV.RB.x, 1.0); glVertex2f( 1, 0 );
		glTexCoord2f(skyUV.LB.x, 1.0); glVertex2f( -1, 0 );
		glTexCoord2f(skyUV.LT.x, 0.0); glVertex2f( -1, 1 );
		glTexCoord2f(skyUV.RT.x, 0.0); glVertex2f( 1, 1 );

		glEnd();


		/*
			DDA - Wall Intersection Detection
		*/

		float fRayAngle;
		sf::Vector2f vRayDir;

		sf::Vector2f vCameraPlane( 0.0f, 0.6f );

		// Distance from player to ray intersection
		float fRayDistance;

		// Current grid position
		sf::Vector2i vMapPosition;

		// Direction to step in
		sf::Vector2f vStep;

		// Length from current position to the next side
		sf::Vector2f vSideDistance;

		// Length from one grid square to the next
		sf::Vector2f vDeltaDistance;

		// Has a wall been hit?
		bool bWallHit;

		int iSide = 0;
		int iWallHeight = 0;

		for( int x = 0; x < vScreen.x; x++ ) {

			vMapPosition = sf::Vector2i( vPlayer );

			fRayAngle = (fPlayerA - fFOV/2.0f) + ((float)x / (float)vScreen.x) * fFOV;
			vRayDir = sf::Vector2f( cos( fRayAngle ), sin( fRayAngle ) );

			vDeltaDistance = sf::Vector2f( abs(1.0f / vRayDir.x), abs(1.0f / vRayDir.y) );
			vStep = sf::Vector2f( copysign( 1.0, vRayDir.x ), copysign( 1.0, vRayDir.y ) );

			vSideDistance.x = ( vRayDir.x < 0 ) ? ( ( vPlayer.x - vMapPosition.x ) * vDeltaDistance.x ) : ( ( vMapPosition.x + 1.0 - vPlayer.x ) * vDeltaDistance.x );
			vSideDistance.y = ( vRayDir.y < 0 ) ? ( ( vPlayer.y - vMapPosition.y ) * vDeltaDistance.y ) : ( ( vMapPosition.y + 1.0 - vPlayer.y ) * vDeltaDistance.y );

			fRayDistance = 0.0f;

			bWallHit = false;

			while( !bWallHit ) {

				if ( vSideDistance.x < vSideDistance.y ) {
					vMapPosition.x += vStep.x;
					fRayDistance = vSideDistance.x;
					vSideDistance.x += vDeltaDistance.x;
					iSide = 0;
				}
				else {
					vMapPosition.y += vStep.y;
					fRayDistance = vSideDistance.y;
					vSideDistance.y += vDeltaDistance.y;
					iSide = 1;
				}

				if( ( ( vMapPosition.x >= 0 ) && ( vMapPosition.x < vMap.x ) ) && ( ( vMapPosition.y >= 0 ) && ( vMapPosition.y < vMap.y ) ) ) {

					std::string layer = jf["map"][ vMapPosition.y ];

					if ( layer.at(vMapPosition.x) == '#' ) {

						bWallHit = true;

					}

				}

				else
					break;

			}

			if( bWallHit ) {

				sf::Vector2f vIntersection = vPlayer + vRayDir * fRayDistance;

				fRayDistance *= cosf( fPlayerA - fRayAngle );

				sf::Texture::bind(&mapTileset.tTexture);

				UV mapUV = mapTileset.getSquareUV( sf::Vector2f( 0.0, 0.0 ) );

				glLineWidth(2);

				// Wall
				glBegin(GL_LINES);

					if( iSide == 0 ) {
						glTexCoord2f( 0.0, 0.0 );
						glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, 1.0 / fRayDistance );
						glTexCoord2f( 0.0, 1.0 );
						glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, -1.0 / fRayDistance );
					}
					else {
						glTexCoord2f( mapUV.LT.x, mapUV.LT.y );
						glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, 1.0 / fRayDistance );
						glTexCoord2f( mapUV.LB.x, mapUV.LB.y );
						glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, -1.0 / fRayDistance );
					}

				glEnd();

			}

		}

        // Display
		window.display();

	}

	return 0;
}
