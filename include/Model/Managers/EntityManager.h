#pragma once
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

namespace Model {
class GameModel; // 前向声明

namespace Entities {
    class Platform;
    class Enemy;
    class Bullet;
    class Pickup;
    class Arrow;
    class Skill;
    class Player;
    enum class PlatformType;
    enum class EnemyType;
    enum class PickupType;
}

namespace Managers {

class EntityManager {
public:
    EntityManager(sf::Vector2u window_size);
    ~EntityManager();
    
    // 更新所有实体
    void updateAll(float delta_time);
    
    // 平台管理
    void addPlatform(Entities::PlatformType type, sf::Vector2f position, sf::Vector2f size, float scroll_speed);
    std::map<int, Entities::Platform*>& getPlatforms() { return platforms; }
    Entities::Platform* getPlatformById(int id) const;
    
    // 敌人管理
    void addEnemy(Entities::EnemyType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model);
    std::map<int, Entities::Enemy*>& getEnemies() { return enemies; }
    
    // 子弹管理
    void addBullet(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size);
    std::map<int, Entities::Bullet*>& getBullets() { return bullets; }
    
    // 豆子管理
    void addPickup(Entities::PickupType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model, int target_platform_id = -1);
    std::map<int, Entities::Pickup*>& getPickups() { return pickups; }
    
    // 箭矢管理
    void addArrow(sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, GameModel* game_model);
    std::map<int, Entities::Arrow*>& getArrows() { return arrows; }
    
    // 技能管理
    void initSkills();
    std::vector<Entities::Skill*>& getSkills() { return skills; }
    
    // 清理所有实体
    void clearAll();
    
private:
    sf::Vector2u window_size;
    
    // 平台相关
    std::map<int, Entities::Platform*> platforms;
    int next_platform_id;
    
    // 敌人相关
    std::map<int, Entities::Enemy*> enemies;
    int next_enemy_id;
    
    // 子弹相关
    std::map<int, Entities::Bullet*> bullets;
    int next_bullet_id;
    
    // 豆子相关
    std::map<int, Entities::Pickup*> pickups;
    int next_pickup_id;
    
    // 箭矢相关
    std::map<int, Entities::Arrow*> arrows;
    int next_arrow_id;
    
    // 技能相关
    std::vector<Entities::Skill*> skills;
    
    // 清理超出窗口的实体
    void cleanupOutOfBounds();
};

} // namespace Managers
} // namespace Model
