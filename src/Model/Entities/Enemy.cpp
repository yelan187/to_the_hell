#include "Model/Entities/Enemy.h"
#include "Model/GameModel.h"
#include <cmath>

using Model::Entities::Enemy;

Enemy::Enemy(int id, EnemyType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model)
    : id(id), type(type), position(position), size(size), game_model(game_model) {
    
    // 初始化敌人的射击和移动参数
    shoot_timer = 0.0f;
    shoot_interval = 2.0f; // 每2秒向玩家射击一次
    move_speed = 50.0f;    // 敌人移动速度：50像素/秒
    velocity = sf::Vector2f(0, 0); // 初始速度为零
}

void Enemy::update(float delta_time) {
    // 每帧更新敌人状态：移动、射击、位置
    updateMovement(delta_time);  // 计算朝向玩家的移动
    updateShooting(delta_time);  // 处理射击逻辑
    position += velocity * delta_time; // 应用速度更新位置
}

void Enemy::updateMovement(float delta_time) {
    // 计算敌人朝向玩家的移动方向和速度
    sf::Vector2f player_pos = game_model->getPlayerPosition();
    sf::Vector2f direction = player_pos - position; // 从敌人指向玩家的向量
    
    // 计算方向向量的长度（距离）
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (length > 0) {
        // 将方向向量标准化为单位向量
        direction.x /= length;
        direction.y /= length;
        
        // 设置敌人朝向玩家移动的速度
        velocity = direction * move_speed;
    }
}

void Enemy::updateShooting(float delta_time) {
    // 更新射击计时器，用于控制射击频率
    shoot_timer += delta_time;
}

bool Enemy::canShoot() const {
    // 检查是否可以射击（基于射击间隔时间）
    return shoot_timer >= shoot_interval;
}

void Enemy::shoot() {
    if (canShoot()) {
        // 计算朝向玩家的射击方向
        sf::Vector2f player_pos = game_model->getPlayerPosition();
        sf::Vector2f direction = player_pos - position; // 从敌人指向玩家
        
        // 标准化射击方向向量
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length > 0) {
            direction.x /= length;
            direction.y /= length;
            
            // 设置子弹速度：方向 × 速度大小
            sf::Vector2f bullet_velocity = direction * 200.0f; // 子弹速度200像素/秒
            sf::Vector2f bullet_pos = position + size / 2.0f;  // 从敌人中心位置发射子弹
            
            // 通过游戏模型创建子弹
            game_model->createBullet(bullet_pos, bullet_velocity);
        }
        
        // 重置射击计时器，准备下次射击
        shoot_timer = 0.0f;
    }
}

bool Enemy::outOfWindow(sf::Vector2u window_size) const {
    // 检查敌人是否完全移出游戏窗口边界
    return position.x + size.x < 0 ||      // 完全移出左边界
           position.x > window_size.x ||   // 完全移出右边界
           position.y + size.y < 0 ||      // 完全移出上边界
           position.y > window_size.y;     // 完全移出下边界
}

bool Enemy::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    // 使用轴对齐边界框(AABB)检测敌人与目标对象的碰撞
    sf::Vector2f enemy_lt = position;                  // 敌人左上角
    sf::Vector2f enemy_rb = enemy_lt + size;           // 敌人右下角
    
    sf::Vector2f target_lt = target_pos;               // 目标对象左上角
    sf::Vector2f target_rb = target_lt + target_size;  // 目标对象右下角
    
    // 使用分离轴定理检测碰撞
    return !(enemy_rb.x <= target_lt.x ||   // 敌人在目标左侧
             enemy_lt.x >= target_rb.x ||   // 敌人在目标右侧
             enemy_rb.y <= target_lt.y ||   // 敌人在目标上方
             enemy_lt.y >= target_rb.y);    // 敌人在目标下方
}
