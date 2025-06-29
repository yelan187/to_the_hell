#pragma once

#include <SFML/Graphics.hpp>
#include "Common/CommandBase.h"
#include "Common/NotificationBase.h"

namespace View {

enum class PAGE_STATE {
    GAME,
    SCORE,
    MAIN_MENU
};
class Page {
public:
    Page(std::string game_title, sf::Vector2u window_size, int fps) :game_title(game_title.data()), window_size(window_size), fps(fps) {
        window.create(sf::VideoMode(window_size.x, window_size.y), game_title, sf::Style::Default);
        window.setFramerateLimit(fps);
    }
    void setUpdateCommand(Common::CommandBase* command) {
        update_command = command;
    }
    void run(){
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                } else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
                    handleInput(event);
                }
            }
            Common::UpdateParam param;
            param.value = 1.0f / fps;
            update_command->execute(param);
            render();
        }
    }

protected:
    // command
    Common::CommandBase* update_command;

    virtual void render() {}
    virtual void handleInput(const sf::Event& event) {}
    
    // game info
    sf::RenderWindow window;
    std::string game_title;
    sf::Vector2u window_size;
    int fps;
};
}