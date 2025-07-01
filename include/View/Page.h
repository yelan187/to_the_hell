#pragma once

#include <SFML/Graphics.hpp>
#include "Common/CommandBase.h"
#include "Common/NotificationBase.h"
#include "Common/PageState.h"

namespace View {

// 使用Common层定义的PAGE_STATE
using Common::PAGE_STATE;
class Page {
public:
    Page(std::string game_title, sf::Vector2u window_size, int fps, sf::RenderWindow& window) :game_title(game_title.data()), window_size(window_size), fps(fps), window(window) {}
    
    void setUpdateCommand(Common::CommandBase* command) {
        update_command = command;
    }

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