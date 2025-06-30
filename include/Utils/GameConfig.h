#pragma once
#include <SFML/Graphics.hpp>
#include <string>

namespace Utils {

class GameConfig {
public:
    // 获取单例实例
    static GameConfig& getInstance();
    
    // 实体尺寸配置（直接访问）
    sf::Vector2f PLATFORM_SIZE;
    sf::Vector2f PLAYER_SIZE;
    sf::Vector2f ENEMY_SIZE;
    sf::Vector2f BULLET_SIZE;
    sf::Vector2f PICKUP_SIZE;
    sf::Vector2f ARROW_SIZE;
    
    // 游戏速度配置（直接访问）
    float DEFAULT_SCROLL_SPEED;
    float ARROW_SPEED;
    float SPRINT_DISTANCE;
    
    // 生成间隔配置（直接访问）
    float PLATFORM_GENERATE_BASE_INTERVAL;
    float ENEMY_GENERATE_BASE_INTERVAL;
    float PICKUP_GENERATE_BASE_INTERVAL;
    
    // 技能冷却时间（直接访问）
    float ARROW_SKILL_COOLDOWN;
    float SPRINT_SKILL_COOLDOWN;
    
    // 配置文件操作
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    
    // 重置为默认值
    void resetToDefaults();
    
    // 难度调整方法
    void adjustForDifficulty(float difficulty_multiplier);

private:
    GameConfig(); // 私有构造函数，实现单例模式
    GameConfig(const GameConfig&) = delete;
    GameConfig& operator=(const GameConfig&) = delete;
    
    // 初始化默认值
    void initializeDefaults();
};

} // namespace Utils
