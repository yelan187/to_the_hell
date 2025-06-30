#pragma once
#include <SFML/Graphics.hpp>

namespace Model {
class GameModel; // 前向声明

namespace Entities {
    class Platform;
    class Enemy;  
    class Pickup;
    enum class PlatformType;
    enum class EnemyType;
    enum class PickupType;
}

namespace Managers {

class EntityManager; // 前向声明

class GeneratorManager {
public:
    GeneratorManager(sf::Vector2u window_size, EntityManager* entity_manager, GameModel* game_model);
    
    // 更新生成间隔并生成实体
    void update(float delta_time);
    
    // 初始化生成器
    void initialize();
    
    // 设置生成参数
    void setScrollSpeed(float speed) { scroll_speed = speed; }
    
    // 生成初始平台
    void generateInitialPlatforms();
    
private:
    sf::Vector2u window_size;
    EntityManager* entity_manager;
    GameModel* game_model;
    float scroll_speed;
    
    // 生成间隔
    float platform_generate_interval;
    float enemy_generate_interval;
    float pickup_generate_interval;
    
    // 生成方法
    void generatePlatform();
    void generateEnemy();
    void generatePickup();
    
    // 重置间隔方法
    void resetPlatformGenerateInterval();
    void resetEnemyGenerateInterval();
    void resetPickupGenerateInterval();
    
    // 随机类型选择
    Entities::PlatformType getPlatformTypeRand();
};

} // namespace Managers
} // namespace Model
