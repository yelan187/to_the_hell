#include <vector>
#include <string>

#include <SFML/Graphics.hpp>
#include "Core/Engine.h"

int main() {
    Core::Engine engine(
        std::string("To The Hell"),
        sf::Vector2u(1600, 900),
        60
    );
    engine.run();
    return 0;
}