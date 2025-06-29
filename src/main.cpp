#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Utils/Config.h"
#include "App/GameApp.h"

int main() {
    App::GameApp game_app(
        Utils::WINDOW_TITLE,
        sf::Vector2u(Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT),
        Utils::FPS
    );
    game_app.run();
    return 0;
}