#pragma once
#include <string>
#include <SFML/Graphics.hpp>

namespace Common {
namespace Config {

// 窗口配置
struct WindowConfig {
    std::string title;
    int width;
    int height;
    int fps;
    bool debug_mode;
};

// 玩家配置
struct PlayerConfig {
    sf::Vector2f size;
    float move_speed;
    float jump_force;
    float gravity;
    float max_fall_speed;
    int max_health;
    int sprint_duration_ms;
    float sprint_speed_multiplier;
};

// 平台配置
struct PlatformConfig {
    sf::Vector2f normal_size;
    sf::Vector2f moving_size;
    sf::Vector2f disappearing_size;
    float moving_speed;
    float disappear_delay_seconds;
    float generation_interval_seconds;
    float normal_platform_probability;     // 普通平台生成概率
    float moving_platform_probability;     // 移动平台生成概率
    float disappearing_platform_probability; // 消失平台生成概率
};

// 敌人配置
struct EnemyConfig {
    sf::Vector2f ghost_size;
    float ghost_move_speed;
    float generation_interval_seconds;
    float spawn_probability;
    int collision_damage;
};

// 子弹/箭矢配置
struct BulletConfig {
    sf::Vector2f arrow_size;
    float arrow_speed;
    float arrow_lifetime_seconds;
    int arrow_damage;
    float cooldown_seconds;
};

// 豆子/拾取物配置
struct PickupConfig {
    sf::Vector2f normal_dot_size;
    sf::Vector2f star_dot_size;
    float generation_interval_seconds;
    float star_dot_probability;      // 五角星豆子生成概率
    int normal_dot_score;           // 普通豆子分数
    int star_dot_score;             // 五角星豆子分数
};

// 技能配置
struct SkillConfig {
    float arrow_skill_cooldown_seconds;
    float sprint_skill_cooldown_seconds;
    float sprint_duration_seconds;
    float sprint_speed_multiplier;
};

// 游戏玩法配置
struct GameplayConfig {
    float difficulty_increase_interval_seconds; // 难度递增间隔
    float enemy_spawn_rate_multiplier;          // 敌人生成速率倍数
    float platform_spawn_rate_multiplier;      // 平台生成速率倍数
    int score_per_second;                       // 每秒基础分数
    int platform_jump_bonus;                   // 跳跃平台奖励分数
    int enemy_kill_bonus;                       // 击杀敌人奖励分数
};

// 物理配置
struct PhysicsConfig {
    float gravity_acceleration;
    float friction_coefficient;
    float air_resistance;
    float terminal_velocity;
    float collision_tolerance;
};

// 视觉效果配置
struct VisualConfig {
    sf::Color normal_platform_color;
    sf::Color moving_platform_color;
    sf::Color disappearing_platform_color;
    sf::Color ghost_enemy_color;
    sf::Color player_color;
    sf::Color arrow_color;
    sf::Color normal_dot_color;
    sf::Color star_dot_color;
    float particle_lifetime_seconds;
};

// 配置管理器类
class ConfigManager {
public:
    static ConfigManager& getInstance();
    
    // 获取各类配置
    const WindowConfig& getWindowConfig() const { return window_config; }
    const PlayerConfig& getPlayerConfig() const { return player_config; }
    const PlatformConfig& getPlatformConfig() const { return platform_config; }
    const EnemyConfig& getEnemyConfig() const { return enemy_config; }
    const BulletConfig& getBulletConfig() const { return bullet_config; }
    const PickupConfig& getPickupConfig() const { return pickup_config; }
    const SkillConfig& getSkillConfig() const { return skill_config; }
    const GameplayConfig& getGameplayConfig() const { return gameplay_config; }
    const PhysicsConfig& getPhysicsConfig() const { return physics_config; }
    const VisualConfig& getVisualConfig() const { return visual_config; }
    
    // 运行时修改配置
    void setWindowConfig(const WindowConfig& config) { window_config = config; }
    void setPlayerConfig(const PlayerConfig& config) { player_config = config; }
    void setPlatformConfig(const PlatformConfig& config) { platform_config = config; }
    void setEnemyConfig(const EnemyConfig& config) { enemy_config = config; }
    void setBulletConfig(const BulletConfig& config) { bullet_config = config; }
    void setPickupConfig(const PickupConfig& config) { pickup_config = config; }
    void setSkillConfig(const SkillConfig& config) { skill_config = config; }
    void setGameplayConfig(const GameplayConfig& config) { gameplay_config = config; }
    void setPhysicsConfig(const PhysicsConfig& config) { physics_config = config; }
    void setVisualConfig(const VisualConfig& config) { visual_config = config; }
    
    // 加载和保存配置
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
    void loadDefaults();

private:
    ConfigManager();
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    WindowConfig window_config;
    PlayerConfig player_config;
    PlatformConfig platform_config;
    EnemyConfig enemy_config;
    BulletConfig bullet_config;
    PickupConfig pickup_config;
    SkillConfig skill_config;
    GameplayConfig gameplay_config;
    PhysicsConfig physics_config;
    VisualConfig visual_config;
};

} // namespace Config
} // namespace Common
