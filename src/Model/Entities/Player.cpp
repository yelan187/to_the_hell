#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Model/Entities/Skill.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Bullet.h"
#include "Model/Entities/Pickup.h"
#include "Model/GameModel.h"
#include <cmath>

using Model::Entities::Player;

void Player::jump() {
    if (jump_counter >= max_jump_count) {
        return;
    }
    jump_counter++;
    if (state == PlayerState::IDLE) {
        state = PlayerState::JUMPING_IDLE;        
    } else if (state == PlayerState::WALKING) {
        state = PlayerState::JUMPING_WALKING;
    }
    velocity.y = -jumping_speed;
}
void Player::fall() {
    if (!on_platform) {
        velocity.y += Common::Config::GameConfig::PLAYER_FALL_ACCELERATION;
    }
}

void Player::groundPenetration(){
    // 只有在平台上时才能使用地面穿透
    if (!on_platform || on_platform_id == -1) {
        return;
    }
    
    Platform* current_platform = game_model->getPlatformById(on_platform_id);
    if (!current_platform) {
        return; // 当前平台不存在
    }
    
    // 计算穿透后的位置（在当前平台下方一点）
    sf::Vector2f target_position = sf::Vector2f(
        position.x, // 保持X坐标不变
        position.y + size.y + current_platform->getSize().y + 5.0f // Y坐标向下穿透
    );
    
    // 检查目标位置是否会与其他平台碰撞
    for (auto& platform_pair : game_model->getPlatforms()) {
        Platform* platform = platform_pair.second;
        if (platform == current_platform) continue; // 跳过当前平台
        
        bool would_collide = collisionDetection(platform, target_position);
        if (would_collide) {
            return; // 如果会碰撞其他平台，则不执行穿透
        }
    }
    
    // 执行穿透：更新位置和状态
    position = target_position;
    velocity.y = 0.0f; // 重置垂直速度
    on_platform = false;
    on_platform_id = -1;
}

void Player::walkLeft() {
    facing_direction = sf::Vector2f(-1.0f, 0.0f);  // 设置面向左侧
    if (state == PlayerState::IDLE) {
        state = PlayerState::WALKING;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::JUMPING_IDLE) {
        state = PlayerState::JUMPING_WALKING;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::JUMPING_WALKING) {
        state = PlayerState::JUMPING_IDLE;
        velocity.x-= walking_speed;
    }
}

void Player::walkRight() {
    facing_direction = sf::Vector2f(1.0f, 0.0f);  // 设置面向右侧
    if (state == PlayerState::IDLE) {
        state = PlayerState::WALKING;
        velocity.x += walking_speed;
    }else if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x += walking_speed;
    } else if (state == PlayerState::JUMPING_IDLE) {
        state = PlayerState::JUMPING_WALKING;
        velocity.x += walking_speed;
    } else if (state == PlayerState::JUMPING_WALKING) {
        state = PlayerState::JUMPING_IDLE;
        velocity.x += walking_speed;
    }
}
void Player::stopLeft() {
    if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x += walking_speed;
    } else if (state == PlayerState::IDLE){
        state = PlayerState::WALKING;
        velocity.x += walking_speed;
    } else if (state == PlayerState::JUMPING_WALKING) {
        state = PlayerState::JUMPING_IDLE;
        velocity.x += walking_speed;
    } else if (state == PlayerState::JUMPING_IDLE) {
        state = PlayerState::JUMPING_WALKING;
        velocity.x += walking_speed;
    }
}

void Player::stopRight() {
    if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::IDLE){
        state = PlayerState::WALKING;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::JUMPING_WALKING) {
        state = PlayerState::JUMPING_IDLE;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::JUMPING_IDLE) {
        state = PlayerState::JUMPING_WALKING;
        velocity.x -= walking_speed;
    }
}

void Player::updatePosition(float delta_time,sf::Vector2f additional_replacement) {
    sf::Vector2f prev_position = position;
    position += velocity * delta_time + 0.5f * acceleration * delta_time * delta_time;
    position += additional_replacement;

    int prev_on_platform_id = on_platform_id;
    on_platform = false;
    on_platform_id = -1;
    for (auto& platform_pair : game_model->getPlatforms()) {
        Platform* platform = platform_pair.second;
        bool res = collisionDetection(platform);
        if (res) {
            if (prev_on_platform_id == platform->id) {
                on_platform = true;
                on_platform_id = platform->id;
            } else {
                handleCollision(platform,prev_position,delta_time);
            }

            break;
        }
    }
}

void Player::updateVelocity(float delta_time) {
    // std::cout << "velocity: " << velocity.x << ", " << velocity.y << std::endl;
    velocity += acceleration * delta_time;
    if (!(on_platform && game_model->getPlatformById(on_platform_id)->type == PlatformType::ROLLING)) {
        if (prev_rolling_associated_velocity.x != 0){
            velocity -= prev_rolling_associated_velocity;
        }
        prev_rolling_associated_velocity.x = 0;
    }

    if (on_platform) {
        Platform* current_platform = game_model->getPlatformById(on_platform_id);
        // 检查脆弱平台是否已经破碎
        if (current_platform->isBroken()) {
            // 平台已破碎，玩家开始下落
            on_platform = false;
            on_platform_id = -1;
            velocity.y = 0; // 开始下落
        } else {
            velocity.y = current_platform->getVelocity().y;
            if (current_platform->type == PlatformType::ROLLING) { 
                if (prev_rolling_associated_velocity.x == 0) {
                    prev_rolling_associated_velocity.x = current_platform->getRollingSpeed() * (current_platform->getRollingDirection() ? 1 : -1);
                    velocity += prev_rolling_associated_velocity;
                }
            }
        }
    } else {
        if (collision_direction == CollisionDirection::NONE) {
            if (prev_collision_correction_velocity.x != 0) {
                velocity -= prev_collision_correction_velocity;
            }
            prev_collision_correction_velocity.x = 0;
        }

        if (collision_direction == CollisionDirection::UP) {
            velocity.y = 0;
        } else if (collision_direction == CollisionDirection::LEFT || 
                   collision_direction == CollisionDirection::RIGHT) {
            if (prev_collision_correction_velocity.x == 0) {
                prev_collision_correction_velocity.x = -velocity.x;
                velocity += prev_collision_correction_velocity;
            }
        }
    }

    prev_collision_direction = collision_direction;
    collision_direction = CollisionDirection::NONE;
    // if(Common::Config::GlobalConfig::DEBUG_MODE){
    //     std::cout << "prev_rolling_associated_velocity: " << prev_rolling_associated_velocity.x << std::endl;
    //     std::cout << "Velocity: " << velocity.x << ", " << velocity.y << std::endl;
    //     std::cout << "on_platform_id: "<< on_platform_id << std::endl;
    // }

}

void Player::updateAcceleration(float delta_time) {
    if (!on_platform) {
        acceleration = gravity;
    } else {
        acceleration = sf::Vector2f(0, 0);
    }
}

void Player::update(float delta_time) {
    updateSkills(delta_time);
    // 处理滚动平台效果 - 移动到updateVelocity中处理
    updatePosition(delta_time);
    updateVelocity(delta_time);
    updateAcceleration(delta_time);
}

bool Player::collisionDetection(Platform* platform) {
    return collisionDetection(platform, getPosition());
}

bool Player::collisionDetection(Platform* platform, sf::Vector2f p) {
    sf::Vector2f player_lt = p;
    sf::Vector2f player_rb = p + getSize();

    // 只在平台检测时缩小水平碰撞框，垂直方向保持完整
    float shrink_amount = getSize().x * Common::Config::GameConfig::PLAYER_COLLISION_SHRINK_RATIO;
    player_lt.x += shrink_amount; // 玩家碰撞框左上角向右偏移
    player_rb.x -= shrink_amount; // 玩家碰撞框右下角向左偏移

    sf::Vector2f platform_lt = platform->getPosition();
    sf::Vector2f platform_rb = platform_lt + platform->getSize();

    return !(player_rb.x <= (platform_lt.x) || player_lt.x >= (platform_rb.x) ||
             player_rb.y <= (platform_lt.y) || player_lt.y >= (platform_rb.y));
}

sf::Vector2f Player::findCollisionPosition(Platform* platform, 
                                            sf::Vector2f prev_position, 
                                            float delta_time, 
                                            bool by_time) {
    if (collisionDetection(platform, prev_position)) {
        return prev_position;
    }
    const int MAX_ITERATIONS = 10;
    
    if (by_time) {
        float t_lowerbound = 0.0f;
        float t_upperbound = delta_time;
        for (int i = 0; i < MAX_ITERATIONS; ++i) {
            float t = (t_lowerbound + t_upperbound) / 2.0f;
            sf::Vector2f new_position = prev_position + velocity * t + 0.5f * acceleration * t * t;
            if (collisionDetection(platform, new_position)) {
                t_upperbound = t;
            } else {
                t_lowerbound = t;
            }
        }
        return prev_position + velocity * t_upperbound + 0.5f * acceleration * t_upperbound * t_upperbound;
    } else {
        sf::Vector2f position_lowerbound = prev_position;
        sf::Vector2f position_upperbound = getPosition();
        for (int i = 0; i < MAX_ITERATIONS; ++i) {
            sf::Vector2f new_position = (position_lowerbound + position_upperbound) / 2.0f;
            if (collisionDetection(platform, new_position)) {
                position_upperbound = new_position;
            } else {
                position_lowerbound = new_position;
            }
        }
        return position_upperbound;
    }
}

void Player::handleCollision(Platform* platform, sf::Vector2f prev_position, float delta_time) {
    sf::Vector2f p = findCollisionPosition(platform, prev_position, delta_time);
    
    // 计算缩小后的水平碰撞框
    float shrink_amount = getSize().x * Common::Config::GameConfig::PLAYER_COLLISION_SHRINK_RATIO;
    sf::Vector2f effective_lt = sf::Vector2f(p.x + shrink_amount, p.y);
    sf::Vector2f effective_rb = sf::Vector2f(p.x + getSize().x - shrink_amount, p.y + getSize().y);
    
    if ((p.y + getSize().y) <= (platform->getPosition().y + 2)) {
        /*
         * player is above the platform
         */
        // std::cout << "Player is above the platform" << std::endl;
        setPosition(p);
        on_platform = true;
        on_platform_id = platform->id;
        // 处理平台特殊效果
        handlePlatformEffects(platform);
        if (state == PlayerState::JUMPING_WALKING) {
            state = PlayerState::WALKING;
        } else if (state == PlayerState::JUMPING_IDLE) {
            state = PlayerState::IDLE;
        }
        jump_counter = 0;
    } else if (p.y >= (platform->getPosition().y + platform->getSize().y - 1)) {
        /*
         * player is below the platform
         */
        // std::cout << "Player is below the platform" << std::endl;
        setPosition(p);
        on_platform = false;
        on_platform_id = -1;
        collision_direction = CollisionDirection::UP;
    } else if ((effective_rb.x) <= (platform->getPosition().x + 1)) {
        /*
         * player is to the left of the platform
         */
        // std::cout << "Player is to the left of the platform" << std::endl;
        setPosition(p.x, getPosition().y);
        on_platform = false;
        on_platform_id = -1;
        collision_direction = CollisionDirection::RIGHT;
    } else if (effective_lt.x >= (platform->getPosition().x + platform->getSize().x - 1)) {
        /*
         * player is to the right of the platform
         */
        // std::cout << "Player is to the right of the platform" << std::endl;
        setPosition(p.x, getPosition().y);
        on_platform = false;
        on_platform_id = -1;
        collision_direction = CollisionDirection::LEFT;
    }
}

void Player::handlePlatformEffects(Platform* platform) {
    // 通知平台玩家已经踩到
    platform->onPlayerLanded();
    
    switch (platform->type) {
        case Entities::PlatformType::SPIKED:
            // 带刺平台：玩家死亡
            kill();
            break;
            
        case Entities::PlatformType::BOUNCY:
            // 弹跳平台：给玩家施加向上的力
            bounce(platform->getBounceForce());
            // 弹跳平台需要让玩家立即离开平台，不能停留在上面
            on_platform = false;
            on_platform_id = -1;
            break;

        case Entities::PlatformType::ROLLING:
        case Entities::PlatformType::FRAGILE:
        case Entities::PlatformType::NORMAL:
        default:
            // 普通平台、滚动平台、脆弱平台不需要立即效果
            // 滚动平台的效果在updateVelocity中处理
            break;
    }
}

void Player::bounce(float bounce_force) {
    velocity.y = -bounce_force; // 向上弹跳
    
    // 设置跳跃状态
    if (state == PlayerState::IDLE) {
        state = PlayerState::JUMPING_IDLE;
    } else if (state == PlayerState::WALKING) {
        state = PlayerState::JUMPING_WALKING;
    }
}

// ==================== HP系统方法 ====================

void Player::takeDamage(int damage) {
    hp -= damage;
    if (hp <= 0) {
        hp = 0;
        is_dead = true;
    }
}

void Player::heal(int amount) {
    hp += amount;
    if (hp > max_hp) {
        hp = max_hp;
    }
}

// ==================== 技能系统方法 ====================

void Player::initSkills() {
    skills.clear();
    skills[Common::SkillID::ARROW_SHOT] = std::make_shared<ArrowShot>(this);
    skills[Common::SkillID::SPRINT] = std::make_shared<Sprint>(this);
    skills[Common::SkillID::GROUND_PENETRATION] = std::make_shared<GroundPenetration>(this);
}

void Player::updateSkills(float delta_time) {
    for (auto& skill_pair : skills) {
        skill_pair.second->update(delta_time);
    }
}

// ==================== 碰撞检测系统 ====================

int Player::checkBulletCollisions() {
    // 获取玩家碰撞框（应用水平缩小）
    sf::Vector2f shrunk_size = sf::Vector2f(
        size.x * (1.0f - Common::Config::GameConfig::PLAYER_COLLISION_SHRINK_RATIO),
        size.y
    );
    sf::Vector2f shrunk_position = sf::Vector2f(
        position.x + (size.x - shrunk_size.x) / 2.0f,
        position.y
    );

    for (auto& bullet_pair : game_model->getBullets()) {
        Bullet* bullet = bullet_pair.second;
        if (!bullet || bullet->isPlayerBullet()) continue; // 跳过玩家子弹

        sf::Vector2f bullet_pos = bullet->getPosition();
        sf::Vector2f bullet_size = bullet->getSize();

        // 检测碰撞
        bool collision = (shrunk_position.x < bullet_pos.x + bullet_size.x &&
                         shrunk_position.x + shrunk_size.x > bullet_pos.x &&
                         shrunk_position.y < bullet_pos.y + bullet_size.y &&
                         shrunk_position.y + shrunk_size.y > bullet_pos.y);

        if (collision) {
            return bullet_pair.first; // 返回子弹ID
        }
    }
    return -1; // 无碰撞
}

int Player::checkEnemyCollisions() {
    for (auto& enemy_pair : game_model->getEnemies()) {
        Enemy* enemy = enemy_pair.second;
        if (!enemy) continue;

        sf::Vector2f enemy_pos = enemy->getPosition();
        sf::Vector2f enemy_size = enemy->getSize();

        // 检测碰撞
        bool collision = (position.x < enemy_pos.x + enemy_size.x &&
                         position.x + size.x > enemy_pos.x &&
                         position.y < enemy_pos.y + enemy_size.y &&
                         position.y + size.y > enemy_pos.y);

        if (collision) {
            return enemy_pair.first; // 返回敌人ID
        }
    }
    return -1; // 无碰撞
}

int Player::checkPickupCollisions() {
    for (auto& pickup_pair : game_model->getPickups()) {
        Pickup* pickup = pickup_pair.second;
        if (!pickup) continue;

        sf::Vector2f pickup_pos = pickup->getPosition();
        sf::Vector2f pickup_size = pickup->getSize();

        // 检测碰撞
        bool collision = (position.x < pickup_pos.x + pickup_size.x &&
                         position.x + size.x > pickup_pos.x &&
                         position.y < pickup_pos.y + pickup_size.y &&
                         position.y + size.y > pickup_pos.y);

        if (collision) {
            return pickup_pair.first; // 返回拾取物ID
        }
    }
    return -1; // 无碰撞
}

void Player::pickup(int pickup_id) {
    Pickup* pickup = game_model->getPickupById(pickup_id);
    if (!pickup) return;

    // 根据拾取物类型处理
    pickup->applyEffect();
    game_model->removePickup(pickup_id); // 从游戏模型中移除拾取物
}

void Player::beDamagedByEnemy(int enemy_id) {
    Enemy* enemy = game_model->getEnemyById(enemy_id);
    if (!enemy) return;

    // 玩家受到敌人伤害
    takeDamage(Common::Config::GameConfig::ENEMY_CONTACT_DAMAGE); // 使用配置中的敌人接触伤害
    
    // 可以添加击退效果等
}

void Player::beDamagedByBullet(int bullet_id) {
    Bullet* bullet = game_model->getBulletById(bullet_id);
    if (!bullet || bullet->isPlayerBullet()) return;

    // 玩家受到子弹伤害
    takeDamage(bullet->getDamage());
    
    // 移除子弹
    game_model->removeBullet(bullet_id);
}