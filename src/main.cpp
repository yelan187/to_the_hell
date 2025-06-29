#include <vector>
#include <string>
#include "Utils/Config.h"
#include <SFML/Graphics.hpp>
#include "Core/Engine.h"

int main() {
    Core::Engine engine(
        Utils::WINDOW_TITLE,
        sf::Vector2u(Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT),
        60
    );
    engine.run();
    return 0;
}