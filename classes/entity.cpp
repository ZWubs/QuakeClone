#include <SFML/Graphics.hpp>

using namespace sf;

class Entity {

private:
    int iHealth;
    Vector2f vPosition;
	// Sprite sSprite;
	// float fFacing;		// Redundant since Sprite also tracks facing?
	// int iSpriteState;	// Redundant since Sprite also tracks state?

	sf::Texture texture		// Placeholder for when sprites is functional

public:
    Entity()
    void Update( float fDelta );

};

// Constructor
Entity::Entity() {

};

void Update( float fDelta /* events */ /* camera */);

};
