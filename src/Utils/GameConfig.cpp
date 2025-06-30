#include "Utils/GameConfig.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace Utils {

GameConfig& GameConfig::getInstance() {
    static GameConfig instance;
    return instance;
}

GameConfig::GameConfig() {
    initializeDefaults();
}

void GameConfig::initializeDefaults() {
    // 实体尺寸配置（默认值）
    PLATFORM_SIZE = sf::Vector2f(100.0f, 12.0f);
    PLAYER_SIZE = sf::Vector2f(30.0f, 60.0f);
    ENEMY_SIZE = sf::Vector2f(40.0f, 40.0f);
    BULLET_SIZE = sf::Vector2f(8.0f, 8.0f);
    PICKUP_SIZE = sf::Vector2f(20.0f, 20.0f);
    ARROW_SIZE = sf::Vector2f(30.0f, 8.0f);
    
    // 游戏速度配置（默认值）
    DEFAULT_SCROLL_SPEED = 100.0f;
    ARROW_SPEED = 300.0f;
    SPRINT_DISTANCE = 150.0f;
    
    // 生成间隔配置（默认值）
    PLATFORM_GENERATE_BASE_INTERVAL = 1.0f;
    ENEMY_GENERATE_BASE_INTERVAL = 3.0f;
    PICKUP_GENERATE_BASE_INTERVAL = 0.8f;
    
    // 技能冷却时间（默认值）
    ARROW_SKILL_COOLDOWN = 3.0f;
    SPRINT_SKILL_COOLDOWN = 5.0f;
}

bool GameConfig::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Warning: Could not open config file " << filename << ", using defaults." << std::endl;
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::istringstream iss(line);
        std::string key;
        if (std::getline(iss, key, '=')) {
            std::string value;
            if (std::getline(iss, value)) {
                // 移除可能的空格
                key.erase(0, key.find_first_not_of(" \t"));
                key.erase(key.find_last_not_of(" \t") + 1);
                value.erase(0, value.find_first_not_of(" \t"));
                value.erase(value.find_last_not_of(" \t") + 1);
                
                // 解析配置项
                if (key == "platform_width") {
                    PLATFORM_SIZE.x = std::stof(value);
                } else if (key == "platform_height") {
                    PLATFORM_SIZE.y = std::stof(value);
                } else if (key == "player_width") {
                    PLAYER_SIZE.x = std::stof(value);
                } else if (key == "player_height") {
                    PLAYER_SIZE.y = std::stof(value);
                } else if (key == "enemy_width") {
                    ENEMY_SIZE.x = std::stof(value);
                } else if (key == "enemy_height") {
                    ENEMY_SIZE.y = std::stof(value);
                } else if (key == "bullet_width") {
                    BULLET_SIZE.x = std::stof(value);
                } else if (key == "bullet_height") {
                    BULLET_SIZE.y = std::stof(value);
                } else if (key == "pickup_width") {
                    PICKUP_SIZE.x = std::stof(value);
                } else if (key == "pickup_height") {
                    PICKUP_SIZE.y = std::stof(value);
                } else if (key == "arrow_width") {
                    ARROW_SIZE.x = std::stof(value);
                } else if (key == "arrow_height") {
                    ARROW_SIZE.y = std::stof(value);
                } else if (key == "default_scroll_speed") {
                    DEFAULT_SCROLL_SPEED = std::stof(value);
                } else if (key == "arrow_speed") {
                    ARROW_SPEED = std::stof(value);
                } else if (key == "sprint_distance") {
                    SPRINT_DISTANCE = std::stof(value);
                } else if (key == "platform_generate_interval") {
                    PLATFORM_GENERATE_BASE_INTERVAL = std::stof(value);
                } else if (key == "enemy_generate_interval") {
                    ENEMY_GENERATE_BASE_INTERVAL = std::stof(value);
                } else if (key == "pickup_generate_interval") {
                    PICKUP_GENERATE_BASE_INTERVAL = std::stof(value);
                } else if (key == "arrow_skill_cooldown") {
                    ARROW_SKILL_COOLDOWN = std::stof(value);
                } else if (key == "sprint_skill_cooldown") {
                    SPRINT_SKILL_COOLDOWN = std::stof(value);
                }
            }
        }
    }
    
    file.close();
    std::cout << "Config loaded from " << filename << std::endl;
    return true;
}

bool GameConfig::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Error: Could not save config file " << filename << std::endl;
        return false;
    }
    
    file << "# Game Configuration File\n";
    file << "# Entity Sizes\n";
    file << "platform_width=" << PLATFORM_SIZE.x << "\n";
    file << "platform_height=" << PLATFORM_SIZE.y << "\n";
    file << "player_width=" << PLAYER_SIZE.x << "\n";
    file << "player_height=" << PLAYER_SIZE.y << "\n";
    file << "enemy_width=" << ENEMY_SIZE.x << "\n";
    file << "enemy_height=" << ENEMY_SIZE.y << "\n";
    file << "bullet_width=" << BULLET_SIZE.x << "\n";
    file << "bullet_height=" << BULLET_SIZE.y << "\n";
    file << "pickup_width=" << PICKUP_SIZE.x << "\n";
    file << "pickup_height=" << PICKUP_SIZE.y << "\n";
    file << "arrow_width=" << ARROW_SIZE.x << "\n";
    file << "arrow_height=" << ARROW_SIZE.y << "\n";
    file << "\n# Game Speed Settings\n";
    file << "default_scroll_speed=" << DEFAULT_SCROLL_SPEED << "\n";
    file << "arrow_speed=" << ARROW_SPEED << "\n";
    file << "sprint_distance=" << SPRINT_DISTANCE << "\n";
    file << "\n# Generation Intervals\n";
    file << "platform_generate_interval=" << PLATFORM_GENERATE_BASE_INTERVAL << "\n";
    file << "enemy_generate_interval=" << ENEMY_GENERATE_BASE_INTERVAL << "\n";
    file << "pickup_generate_interval=" << PICKUP_GENERATE_BASE_INTERVAL << "\n";
    file << "\n# Skill Cooldowns\n";
    file << "arrow_skill_cooldown=" << ARROW_SKILL_COOLDOWN << "\n";
    file << "sprint_skill_cooldown=" << SPRINT_SKILL_COOLDOWN << "\n";
    
    file.close();
    std::cout << "Config saved to " << filename << std::endl;
    return true;
}

void GameConfig::resetToDefaults() {
    initializeDefaults();
    std::cout << "Config reset to defaults" << std::endl;
}

void GameConfig::adjustForDifficulty(float difficulty_multiplier) {
    // 先实现了一个总体的在这里，如果想具体修改某些参数，可以不必使用这个函数，直接修改
    // 随着难度增加调整参数
    // 例如：生成间隔减少（更频繁），敌人速度增加等
    
    // 减少生成间隔（更频繁生成）
    PLATFORM_GENERATE_BASE_INTERVAL = std::max(0.3f, 1.0f / difficulty_multiplier);
    ENEMY_GENERATE_BASE_INTERVAL = std::max(1.0f, 3.0f / difficulty_multiplier);
    PICKUP_GENERATE_BASE_INTERVAL = std::max(0.3f, 0.8f / difficulty_multiplier);
    
    // 增加游戏速度
    DEFAULT_SCROLL_SPEED = 100.0f * difficulty_multiplier;
    
    // 可以根据需要调整其他参数
    // 例如：敌人尺寸、子弹速度等
    
    std::cout << "Config adjusted for difficulty: " << difficulty_multiplier << std::endl;
}

} // namespace Utils
