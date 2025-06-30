#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include "Utils/Config.h"
#include "View/Page.h"
#include "View/UI/Player.h"
#include "View/UI/Platform.h"

namespace View {

class GameView : public Page {
public:

    GameView(std::string game_title, sf::Vector2u window_size, int fps, sf::RenderWindow& window, bool debug = false) : 
        Page(game_title, window_size, fps, window), 
        debug(debug),
        player(window, Utils::PLAYER_SIZE)
    {
        if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
            return;
        }
    }
    
    // properties
    void setTotalScoreText(const std::string& text) {
        total_score_text.setString(text);
    }
    void setGameTimeText(const std::string& text) {
        game_time_text.setString(text);
    }
    void setDebugInfoText(const std::string& text) {
        if (debug) {
            debug_info_text.setString(text);
        }
    }
    // commands
    void setPlayerLeftCommand(Common::CommandBase* command) {
        playerLeftCommand = command;
    }
    void setPlayerRightCommand(Common::CommandBase* command) {
        playerRightCommand = command;
    }
    void setPlayerJumpCommand(Common::CommandBase* command) {
        playerJumpCommand = command;
    }
    void setPlayerDownCommand(Common::CommandBase* command) {
        playerDownCommand = command;
    }
    void setPlayerStopLeftCommand(Common::CommandBase* command) {
        playerStopLeftCommand = command;
    }
    void setPlayerStopRightCommand(Common::CommandBase* command) {
        playerStopRightCommand = command;
    }
    void setPlayerStopJumpCommand(Common::CommandBase* command) {
        playerStopJumpCommand = command;
    }
    void setPlayerStopDownCommand(Common::CommandBase* command) {
        playerStopDownCommand = command;
    }
    void setGameOverCommand(Common::CommandBase* command) {
        gameover_command = command;
    }
    // notification
    Common::NotificationFunc getNotificationCallback() {
        return &notification_callback;
    }
    // update
    void updateframe(Common::FrameInfo frame_info);
    
    void init();
    void render() override;
    void handleInput(const sf::Event& event) override;
private:
    void gameOver(Common::GameOver value);
    static void notification_callback(Common::NotificationParam* param, void* view);
    // game info
    bool debug;
    sf::Font font;
    sf::Text game_time_text;
    sf::Text total_score_text;
    sf::Text debug_info_text;
    // player info
    View::UI::Player player;
    
    std::vector<int> platforms_id;
    std::vector<View::UI::Platform> platforms;

    // commands
    Common::CommandBase* playerLeftCommand;
    Common::CommandBase* playerRightCommand;
    Common::CommandBase* playerJumpCommand;
    Common::CommandBase* playerDownCommand;
    
    Common::CommandBase* playerStopLeftCommand;
    Common::CommandBase* playerStopRightCommand;
    Common::CommandBase* playerStopJumpCommand;
    Common::CommandBase* playerStopDownCommand;

    Common::CommandBase* gameover_command;
};

}