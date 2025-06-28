#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <chrono>

#include <SFML/Graphics.hpp>

namespace View {
    class Page;
}

namespace Core {
class Engine {
public:
    struct GameResult {
        int score;
        std::chrono::seconds time;
    };

    enum class PAGE_STATE {
        MAIN_MENU,
        GAME,
        SCORE
    };

    Engine(std::string game_title, sf::Vector2u window_size, int fps);
    
    void run();

    inline std::string getGameTitle() const {
        return game_title;
    }

    sf::Vector2u getWindowSize() const;

    void handleInput(const sf::Event& event);

    void changePage(PAGE_STATE new_page_state);
    void endGame(int score, std::chrono::seconds time);

    void startGame();

    void exitGame();

private:
    int fps;
    PAGE_STATE page_state;
    GameResult last_game_result;

    std::shared_ptr<View::Page> page;
    
    sf::RenderWindow window;
    std::string game_title;
};
}