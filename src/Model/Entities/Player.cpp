#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Model/GameModel.h"
#include <cmath>

using Model::Entities::Player;

void Player::jump(float scroll_speed) {
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
    Platform* current_platform = game_model->getPlatformById(on_platform_id);
    sf::Vector2f p = position + sf::Vector2f(0, size.y + current_platform->getSize().y + 2);
    for (auto& platform_pair : game_model->getPlatforms()) {
        Platform* platform = platform_pair.second;
        bool res = collisionDetection(platform, p);
        if (res) {
            return;
        }
    }
    position = p;
    velocity.y = current_platform->getVelocity().y;
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
    std::cout << "prev_rolling_associated_velocity: " << prev_rolling_associated_velocity.x << std::endl;
    std::cout << "Velocity: " << velocity.x << ", " << velocity.y << std::endl;
    std::cout << "on_platform_id: "<< on_platform_id << std::endl;
}

void Player::updateAcceleration(float delta_time) {
    if (!on_platform) {
        acceleration = gravity;
    } else {
        acceleration = sf::Vector2f(0, 0);
    }
}

void Player::update(float delta_time) {

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