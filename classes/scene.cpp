#include <SFML/Graphics.hpp>

using namespace sf;

class Scene {

    private:
    Entity[] entities;
    Tileset[] tilesets;

    public:
    Scene();

};

// Constructor
Scene::Scene() {

    fFov = 60;

}

Scene::loadScene() {}
