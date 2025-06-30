#pragma once
#include <SFML/Graphics.hpp>

namespace Model {
namespace Entities {
    class Player;
    class Bullet;
    class Pickup;
    class Arrow;
    class Enemy;
}

namespace Managers {

class EntityManager; // 前向声明

class CollisionManager {
public:
    CollisionManager(EntityManager* entity_manager);
    
    // 检查所有碰撞
    int checkAllCollisions(Entities::Player* player);
    
private:
    EntityManager* entity_manager;
    
    // 具体的碰撞检查方法
    bool checkBulletPlayerCollisions(Entities::Player* player);
    int checkPickupPlayerCollisions(Entities::Player* player);
    int checkArrowEnemyCollisions();
};

} // namespace Managers
} // namespace Model
