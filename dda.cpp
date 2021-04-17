#include <iostream>
#include <math.h>
#define PI 3.1415926535

#include <SFML/Graphics.hpp>


using namespace std;

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

	cout << "A";

	sf::RenderWindow window(sf::VideoMode( vScreen.x, vScreen.y), "DDA Ray Casting");

	sf::Image buffer;
	buffer.create( vScreen.x, vScreen.y, sf::Color(0, 0, 0) );

	sf::Texture texture;
	texture.loadFromImage(buffer);

	sf::Sprite bufferSprite;
	bufferSprite.setTexture(texture);

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
		window.clear();

        /*
			DDA - Intersection Detection
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

			bWallHit = false;

			while( !bWallHit ) {

				if ( vSideDistance.x < vSideDistance.y ) {
					vSideDistance.x += vDeltaDistance.x;
					vMapPosition.x += vStep.x;
					iSide = 0;
				}
				else {
					vSideDistance.y += vDeltaDistance.y;
					vMapPosition.y += vStep.y;
					iSide = 1;
				}

				if( ( ( vMapPosition.x >= 0 ) && ( vMapPosition.x < vMap.x ) ) && ( ( vMapPosition.y >= 0 ) && ( vMapPosition.y < vMap.y ) ) ) {

					if ( map.c_str()[vMapPosition.y * vMap.y + vMapPosition.x ] == '#' ) {

						bWallHit = true;

					}

				}

				else
					break;

			}

			fRayDistance = 0.0f;

			if( bWallHit ) {

				if( iSide == 0 ) fRayDistance = ( vMapPosition.x - vPlayer.x + ( 1.0f - vStep.x ) / 2.0f ) / vRayDir.x;
				else fRayDistance = ( vMapPosition.y - vPlayer.y + ( 1.0f - vStep.y ) / 2.0f ) / vRayDir.y;

				fRayDistance *= cosf( fPlayerA - fRayAngle );

				iWallHeight = (int)( (float)vScreen.y / fRayDistance );

				int drawStart = -iWallHeight / 2 + vScreen.y / 2;
				if(drawStart < 0) drawStart = 0;
				int drawEnd = iWallHeight / 2 + vScreen.y / 2;
				if(drawEnd >= vScreen.y) drawEnd = vScreen.y - 1;

				float fogMix = 1;	// 0 = wall, 1 = fog
				if( /*fRenderDistance != 0 &&*/ fRayDistance < fRenderDistance ) {

					fogMix = ( fRayDistance / fRenderDistance );	// + 1
					if (fogMix > 1) fogMix = 1;
					else if (fogMix < 0) fogMix = 0;
					// if ( iSide == 0 ) fogMix *=0.75;	// Darken based on N/S or E/W walls. looks a little odd at a distance. use to darken wall color directly instead

					// TODO: fog start distance
					// TODO: Better Edge/corner shading for visability (Fake AO) (store a depth pass, then do a color pass???)
				}

				sf::Color finalWallColor(	// for something better, see https://www.alanzucconi.com/2016/01/06/colour-interpolation/
					(wallColor.r + (fogColor.r - wallColor.r) * fogMix),
					(wallColor.g + (fogColor.g - wallColor.g) * fogMix),
					(wallColor.b + (fogColor.b - wallColor.b) * fogMix)
				);

				// Draw colors to screen
				for( int y = 0; y < vScreen.y; y++ ) {
					if( y > drawStart && y <= drawEnd ) buffer.setPixel( x, y, finalWallColor );
					else if( y > (vScreen.y/2) ) buffer.setPixel( x, y, floorColor );
					else buffer.setPixel( x, y, skyColor );

				}

			}

		}

		// window.close();

        // Update Texture to new bugger
		texture.update( buffer );

        // Display
		window.draw( bufferSprite );
		window.display();

	}

	return 0;
}
