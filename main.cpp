/*
	STD
*/
#include <iostream>
#include <fstream>
#include <math.h>
#define PI 3.1415926535


/*
	Game Engine
*/
#include "classes/game.cpp"

/*
	Main
*/
int main( int argc, char** argv ) {

	if( argc > 1 ) {

		Game game( argv[1] );

		game.loop();

	}

	else {

		std::cout << "ERROR: Please provide a file name" << std::endl;

		exit(0);

	}

}
