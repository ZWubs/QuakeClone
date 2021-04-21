// STD Classes
#include <iostream>
#include <math.h>
#define PI 3.1415926535

// OpenGL
#include <GL/glut.h>
// Ubuntu: sudo apt-get install freeglut3-dev
// Fedora/RedHat: sudo yum install freeglut-devel

// SFML
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

using namespace std;

/*
	Base Variables
*/

sf::Vector2i vScreen( 320, 240 );
sf::Vector2i vMap( 16, 16 );

sf::Vector2f vPlayer( 8.0, 8.0 );
float fPlayerA = 1.0f;			// Player Start Rotation
float fFOV = 3.14159f / 2.5f;	// Field of View (3.14159f / 4.0f; = 45deg fov)
float fRenderDistance = 16.0f;	// Maximum rendering distance
float fSpeed = 2.0f;			// Walking Speed

// Default Colors
sf::Color wallColor(250, 175, 175);
sf::Color floorColor(20, 150, 50);
sf::Color skyColor(0, 100, 200);
sf::Color fogColor(0, 100, 200);

int main() {

	sf::RenderWindow window(sf::VideoMode( vScreen.x, vScreen.y), "DDA Ray Casting" , sf::Style::Close );

	window.setFramerateLimit( 30 );

	window.setActive( true );

	glClearColor(1.0,0.3,0.3,1.0);

	wstring map;
	map += L"################";
	map += L"#..............#";
	map += L"#.........######";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#.#.#..........#";
	map += L"#..............#";
	map += L"#.#.#..........#";
	map += L"#..............#";
	map += L"################";

	sf::Image img_data;
	if (!img_data.loadFromFile("./img/stone.png")) {
	    cout << "Could not load ./img/square.png" << endl;
	    return false;
	}

	sf::Vector2u img_size = img_data.getSize();

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		img_size.x, img_size.y,
		0,
		GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr()
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glEnable(GL_TEXTURE_2D);

	// deltaTime Clock
	sf::Clock deltaClock;

	while (window.isOpen()) {

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
			if (map.c_str()[(int)vPlayer.y * vMap.y + (int)vPlayer.x] == '#') {
				vPlayer.x -= cosf(fPlayerA) * fSpeed * sign;
				vPlayer.y -= sinf(fPlayerA) * fSpeed * sign;
			}
		}

		/*
			Clear
		*/
		//window.clear();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*
			Floor & Ceiling Intersection Detection
		*/
		/*
		glColor3f( 1.0, 1.0, 1.0 );

		for( int y = 0; y < vScreen.y; y++ ) {

			if( y > vScreen.y / 2 ) {

				float fRayAngle = (fPlayerA - fFOV/2.0f) + ((float)x / (float)vScreen.x) * fFOV;
				sf::Vector2f vRayDir = sf::Vector2f( cos( fRayAngle ), sin( fRayAngle ) );

				glTexCoord2f( fmod( vIntersection.y, 1.0 ), 0.0 );
				glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, 1.0 / fRayDistance );
				glTexCoord2f( fmod( vIntersection.y, 1.0 ), 1.0 );
				glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, -1.0 / fRayDistance );

			}

		}*
		/

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

					if ( map.c_str()[ vMapPosition.y * vMap.y + vMapPosition.x ] == '#' ) {

						bWallHit = true;

					}

				}

				else
					break;

			}

			if( bWallHit ) {

				sf::Vector2f vIntersection = vPlayer + vRayDir * fRayDistance;

				fRayDistance *= cosf( fPlayerA - fRayAngle );

				glLineWidth(2);

				// Wall
				glBegin(GL_LINES);

					if( iSide == 0 ) {
						glColor3f(0.75,0.75,0.75);
						glTexCoord2f( fmod( vIntersection.y, 1.0 ), 0.0 );
						glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, 1.0 / fRayDistance );
						glTexCoord2f( fmod( vIntersection.y, 1.0 ), 1.0 );
						glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, -1.0 / fRayDistance );
					}
					else {
						glColor3f(1,1,1);
						glTexCoord2f( fmod( vIntersection.x, 1.0 ), 0.0 );
						glVertex2f( (float)x / vScreen.x * 2.0 - 1.0, 1.0 / fRayDistance );
						glTexCoord2f( fmod( vIntersection.x, 1.0 ), 1.0 );
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
