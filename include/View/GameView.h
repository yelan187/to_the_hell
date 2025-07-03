#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include "Common/Config/Config.h"
#include "Common/FrameInfo.h"
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
        player(window, Common::Config::GameConfig::PLAYER_SIZE),
        skill_bar(sf::Vector2f(10.0f, 180.0f), sf::Vector2f(75.0f, 75.0f)),
        current_background_name(""),
        is_transitioning(false),
        transition_progress(0.0f)
    {
        // 初始化移到init()方法中
    }
    
    // properties
    void setFrameInfo(Common::FrameInfo* frame_info) {
        this->frame_info = frame_info;
    }
    void setTotalScore(std::string* total_score) {
        this->total_score = total_score;
    }
    void setGameTime(std::string* game_time) {
        this->game_time = game_time;
    }
    void setPlatformInfo(std::string* platform_info) {
        this->platform_info = platform_info;
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
    // others
    void init();
    void render() override;
    void handleInput(const sf::Event& event) override;
private:
    void updateframe();
    void gameOver();
    void preloadBackgrounds(); // 预加载所有背景图片
    void setBackgroundScale(sf::Sprite& sprite, const sf::Texture& texture); // 设置背景缩放
    void switchBackground(const std::string& background_file); // 切换背景（带淡入淡出）
    void updateBackgroundTransition(float delta_time); // 更新背景过渡效果
    // properties
    Common::FrameInfo* frame_info;
    std::string* total_score;
    std::string* game_time;
    std::string* platform_info;
    // notification
    static void notification_callback(Common::NotificationId id, void* view);
    // game info
    bool debug;
    sf::Font font;
    sf::Text game_time_text;
    sf::Text total_score_text;
    sf::Text platform_info_text;
    sf::Text player_hp_text;
    
    // 背景系统
    std::map<std::string, sf::Texture> background_textures; // 预加载的背景纹理
    sf::Sprite current_background_sprite;  // 当前背景精灵
    sf::Sprite next_background_sprite;     // 下一个背景精灵（用于淡入淡出）
    std::string current_background_name;   // 当前背景名称
    bool is_transitioning;                 // 是否正在过渡
    float transition_progress;             // 过渡进度 (0.0 - 1.0)
    // player info
    View::UI::Player player;
    // platform info
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
    Common::CommandBase* playerSkillCommand;
};

}