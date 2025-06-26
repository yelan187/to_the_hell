#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include "Core/Engine.h"

int main() {
    Core::Engine engine(
        std::string("To The Hell"),
        sf::Vector2u(800, 600),
        60
    );
    engine.run();
    return 0;
}