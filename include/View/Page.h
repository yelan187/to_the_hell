#pragma once

#include <SFML/Graphics.hpp>
#include "Common/CommandBase.h"
#include "Common/NotificationBase.h"

namespace View {

enum class PAGE_STATE {
    GAME,
    EXIT,
    SCORE,
    MAIN_MENU
};
class Page {
public:
    Page(std::string game_title, sf::Vector2u window_size, int fps, sf::RenderWindow& window) :game_title(game_title.data()), window_size(window_size), fps(fps), window(window) {}
    void setUpdateCommand(Common::CommandBase* command) {
        update_command = command;
    }
    virtual void init() {}
    void exit() {
        window.close();
    }

    virtual void render() {}
    virtual void handleInput(const sf::Event& event) {}
    
    Common::CommandBase* update_command;

protected:
    // game info
    sf::RenderWindow& window;
    std::string game_title;
    sf::Vector2u window_size;
    int fps;
};
}