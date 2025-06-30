#pragma once
#include <string>
#include <SFML/Graphics.hpp>

namespace Utils {
    const std::string WINDOW_TITLE = "To The Hell";
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr int FPS = 60;
    constexpr bool DEBUG_MODE = true;

    const sf::Vector2f PLAYER_SIZE(60, 60);
    const sf::Vector2f PLATFORM_SIZE(100, 12);
}
