#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include <vector>
#include "View/Page.h"
#include "View/UI/Player.h"
#include "View/UI/Platform.h"

namespace View {

class GameView : public Page {
public:

    GameView(std::string game_title, sf::Vector2u window_size, int fps, sf::RenderWindow& window) : 
        Page(game_title, window_size, fps, window), 
        debug(true),
        player(window)
    {
        if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
            return;
        }
    }
    
    // properties
    void update(Common::FrameInfo frame_info);
    // notification
    Common::NotificationFunc getNotificationCallback() {
        return &notification_callback;
    }
    void handleInput(const sf::Event& event) override;
    void init();
    void render() override;
    void handleInput(const sf::Event& event) override;
private:
    static void notification_callback(Common::NotificationParam* param, void* view);
    // info
    sf::Font font;
    sf::Text game_time_text;
    sf::Text total_score_text;

    bool debug;
    sf::Text debug_info_text;

    View::UI::Player player;
    
    std::vector<int> platforms_id;
    std::vector<View::UI::Platform> platforms;

public:
    Common::CommandBase* getPlatFormsIdCommand;
    
    Common::CommandBase* playerLeftCommand;
    Common::CommandBase* playerRightCommand;
    Common::CommandBase* playerJumpCommand;
    Common::CommandBase* playerDownCommand;
    
    Common::CommandBase* playerStopLeftCommand;
    Common::CommandBase* playerStopRightCommand;
    Common::CommandBase* playerStopJumpCommand;
    Common::CommandBase* playerStopDownCommand;
};

}