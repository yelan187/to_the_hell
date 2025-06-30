#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include "Utils/Config.h"
#include "View/Page.h"
#include "View/UI/Player.h"
#include "View/UI/Platform.h"
#include "View/UI/Enemy.h"     // 敌人UI组件
#include "View/UI/Bullet.h"    // 子弹UI组件
#include "View/UI/Pickup.h"    // 豆子UI组件
#include "View/UI/SkillBar.h"  // 技能栏UI组件

namespace View {

class GameView : public Page {
public:

    GameView(std::string game_title, sf::Vector2u window_size, int fps, sf::RenderWindow& window, bool debug = false) : 
        Page(game_title, window_size, fps, window), 
        debug(debug),
        player(window, Utils::PLAYER_SIZE),
        skill_bar(sf::Vector2f(10.0f, 100.0f), sf::Vector2f(50.0f, 50.0f)) // 左侧位置
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
    
    // 统一的技能命令设置（包含射击）
    void setPlayerSkillCommand(Common::CommandBase* command) {
        playerSkillCommand = command;
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
    
    // 游戏实体容器
    std::vector<View::UI::Enemy> enemies;
    std::vector<View::UI::Bullet> bullets;
    std::vector<View::UI::Pickup> pickups;
    
    // 技能栏
    View::UI::SkillBar skill_bar;

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
    
    // 技能命令
    Common::CommandBase* playerSkillCommand;
};

}