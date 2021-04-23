/*
	STD
*/

#include <iostream>
#include <math.h>
#define PI 3.1415926535

/*
	OpenGL
*/

#include <GL/glut.h>
// Ubuntu: sudo apt-get install freeglut3-dev
// Fedora/RedHat: sudo yum install freeglut-devel

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
#include "classes/camera.cpp"
#include "classes/scene.cpp"

/*
	Main
*/
int main() {

	Camera camera = Camera();

	std::cout << camera.getRayDir( 0.0, 0 ).x;
}
