#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "Common/Config/Config.h"
#include "App/GameApp.h"

int main() {
    App::GameApp game_app(
        Common::Config::GlobalConfig::WINDOW_TITLE,
        sf::Vector2u(Common::Config::GlobalConfig::WINDOW_WIDTH, Common::Config::GlobalConfig::WINDOW_HEIGHT),
        Common::Config::GlobalConfig::FPS,
        Common::Config::GlobalConfig::DEBUG_MODE
    );
    game_app.run();
    return 0;
}