#include <iostream>
#include <math.h>

#include <SFML/Graphics.hpp>


using namespace std;

int nScreenWidth = 320;			// Console Screen Size X (columns)
int nScreenHeight = 240;			// Console Screen Size Y (rows)
int nMapWidth = 16;				// World Dimensions
int nMapHeight = 16;

float fPlayerX = 8.0f;			// Player Start Position
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;			// Player Start Rotation
float fFOV = 3.14159f / 5.0f;	// Field of View
float fDepth = 16.0f;			// Maximum rendering distance
float fSpeed = 0.005f;			// Walking Speed


int main() {

	sf::RenderWindow window(sf::VideoMode( nScreenWidth, nScreenHeight), "Ray Caster Engine");

	sf::Image buffer;
	buffer.create(nScreenWidth, nScreenHeight, sf::Color(0, 0, 0));

	sf::Texture texture;
	texture.loadFromImage(buffer);

	sf::Sprite bufferSprite;
	bufferSprite.setTexture(texture);

	wstring map;
	map += L"################";
	map += L"#.#............#";
	map += L"###............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#..............#";
	map += L"#....##..##....#";
	map += L"#....#....#....#";
	map += L"#....#....#....#";
	map += L"#....#....#....#";
	map += L"#....#....#....#";
	map += L"################";

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			fPlayerA -= fSpeed * 0.75f;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			fPlayerA += fSpeed * 0.75f;
		}

		if( sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			int sign = 1;
			if( sf::Keyboard::isKeyPressed(sf::Keyboard::S) == true ) { sign = -1; }
			fPlayerX += sinf(fPlayerA) * fSpeed * sign;
			fPlayerY += cosf(fPlayerA) * fSpeed * sign;
			if (map.c_str()[(int)fPlayerX * nMapWidth + (int)fPlayerY] == '#') {
				fPlayerX -= sinf(fPlayerA) * fSpeed * sign;
				fPlayerY -= cosf(fPlayerA) * fSpeed * sign;
			}
		}


		window.clear();

		for (int x = 0; x < nScreenWidth; x++) {
			float fRayAngle = (fPlayerA - fFOV/2.0f) + ((float)x / (float)nScreenWidth) * fFOV;

			float fStepSize = 0.1f;
			float fDistanceToWall = 0.0f;

			bool bHitWall = false;
			bool bBoundary = false;

			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth) {
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight) {
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else {
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#') {
						bHitWall = true;
					}
				}
			}

			// Calculate distance to ceiling and floor
			int nCeiling = (float)(nScreenHeight/2.0) - nScreenHeight / ((float)fDistanceToWall);
			int nFloor = nScreenHeight - nCeiling;

			sf::Color color = sf::Color(0,0,0);

			for( int y = 0; y < nScreenHeight; y++ ) {
				if( y < nCeiling ) {
					color = sf::Color(0,0,0);
				}
				else if( y > nFloor ) {
					color = sf::Color(0,0,0);
				}
				else {
					int val = 255 - 256* fDistanceToWall/fDepth;
					color = sf::Color( val, val, val );
				}
				buffer.setPixel( x, y, color);
			}

		}

		texture.update( buffer );

		window.draw( bufferSprite );
		window.display();
	}

	return 0;
}
