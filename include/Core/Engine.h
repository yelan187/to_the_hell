#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <SFML/Graphics.hpp>

namespace View {
    class Page;
}

namespace Core {
class Engine {
public:
    enum class PAGE_STATE {
        MAIN_MENU,
        GAME,
        SCORE
    };

    Engine(std::string game_title, sf::Vector2u window_size, int fps);
    
    void run();

    void handleInput(const sf::Event& event);

    void changePage(PAGE_STATE new_page_state);

    void startGame();

    void exitGame();

private:
    int fps;
    PAGE_STATE page_state;

    std::shared_ptr<View::Page> page;
    
    sf::RenderWindow window;
};
}