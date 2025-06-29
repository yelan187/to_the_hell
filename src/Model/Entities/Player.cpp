#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Model/GameModel.h" // Include the definition of GameModel
#include "Utils/Config.h"

using Model::Entities::Player;

void Player::jump(float scroll_speed) {
    if (state == PlayerState::IDLE) {
        state = PlayerState::JUMPING_IDLE;
        velocity.y = -jumping_speed;
        
    } else if (state == PlayerState::WALKING) {
        state = PlayerState::JUMPING_WALKING;
        if (on_platform) {
            velocity.y = -jumping_speed;
        } else {
            velocity.y = -jumping_speed - scroll_speed;
        }
    }
}
void Player::fall() {
    if (!on_platform) {
        velocity.y += 300.0f;
    }
}
void Player::walkLeft() {
    if (state == PlayerState::IDLE) {
        state = PlayerState::WALKING;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x = 0;
    } else if (state == PlayerState::JUMPING_IDLE) {
        state = PlayerState::JUMPING_WALKING;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::JUMPING_WALKING) {
        state = PlayerState::JUMPING_IDLE;
        velocity.x = 0;
    }
}

void Player::walkRight() {
    if (state == PlayerState::IDLE) {
        state = PlayerState::WALKING;
        velocity.x += walking_speed;
    }else if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x = 0;
    } else if (state == PlayerState::JUMPING_IDLE) {
        state = PlayerState::JUMPING_WALKING;
        velocity.x += walking_speed;
    } else if (state == PlayerState::JUMPING_WALKING) {
        state = PlayerState::JUMPING_IDLE;
        velocity.x = 0;
    }
}
void Player::stopLeft() {
    if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x = 0;
    } else if (state == PlayerState::IDLE){
        state = PlayerState::WALKING;
        velocity.x += walking_speed;
    } else if (state == PlayerState::JUMPING_WALKING) {
        state = PlayerState::JUMPING_IDLE;
        velocity.x = 0;
    } else if (state == PlayerState::JUMPING_IDLE) {
        state = PlayerState::JUMPING_WALKING;
        velocity.x += walking_speed;
    }
}

void Player::stopRight() {
    if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x = 0;
    } else if (state == PlayerState::IDLE){
        state = PlayerState::WALKING;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::JUMPING_WALKING) {
        state = PlayerState::JUMPING_IDLE;
        velocity.x = 0;
    } else if (state == PlayerState::JUMPING_IDLE) {
        state = PlayerState::JUMPING_WALKING;
        velocity.x -= walking_speed;
    }
}

void Player::updatePosition(float delta_time) {
    sf::Vector2f prev_position = position;
    position += velocity * delta_time + 0.5f * acceleration * delta_time * delta_time;
    // std::cout << "Updating position: " << position.x << ", " << position.y << std::endl;

    int prev_on_platform_id = on_platform_id;
    on_platform = false;
    on_platform_id = -1;
    for (auto& platform_pair : game_model->getPlatforms()) {
        Platform* platform = platform_pair.second;
        bool res = collisionDetection(platform);
        if (res) {
            // std::cout << "Collision detected with platform ID: " << platform->id << std::endl;

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
    velocity += acceleration * delta_time;
    
    if (on_platform) {
        velocity.y = game_model->getPlatformById(on_platform_id)->getVelocity().y;
    } else {
        if (collision_direction == CollisionDirection::UP) {
            velocity.y = 0;
        } else if (collision_direction == CollisionDirection::LEFT) {
            velocity.x = 0;
        } else if (collision_direction == CollisionDirection::RIGHT) {
            velocity.x = 0;
        } else if (collision_direction == CollisionDirection::NONE) {
            if ((state == PlayerState::JUMPING_WALKING || state == PlayerState::WALKING)
                && velocity.x == 0){
                if (prev_collision_direction == CollisionDirection::LEFT) {
                    velocity.x = -walking_speed;
                } else if (prev_collision_direction == CollisionDirection::RIGHT) {
                    velocity.x = walking_speed;
                }
            }
        }
    }
    prev_collision_direction = collision_direction;
    collision_direction = CollisionDirection::NONE;
}

void Player::updateAcceleration(float delta_time) {
    if (!on_platform) {
        acceleration = gravity;
    } else {
        acceleration = sf::Vector2f(0, 0);
    }
}

void Player::update(float delta_time) {
    // std::cout << sf::Keyboard::isKeyPressed(sf::Keyboard::A) << std::endl;
    auto platforms = game_model->getPlatforms();
    // cout velocity and acceleration
    // std::cout << "Velocity: " << velocity.x << ", " << velocity.y << std::endl;
    // std::cout << "position: " << position.x << ", " << position.y << std::endl;
    // std::cout << "on_platform_id: " << on_platform_id << std::endl;
    
    updatePosition(delta_time);
    updateVelocity(delta_time);
    updateAcceleration(delta_time);
}

bool Player::collisionDetection(Platform* platform) {
    return collisionDetection(platform, getPosition());
}

bool Player::collisionDetection(Platform* platform, sf::Vector2f p) {
    sf::Vector2f player_lt = p;
    sf::Vector2f player_rb = player_lt + getSize();

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
    // std::cout << "Handling collision with platform ID: " << platform->id << std::endl;
    sf::Vector2f p = findCollisionPosition(platform, prev_position, delta_time);
    if ((p.y + getSize().y) <= (platform->getPosition().y + 2)) {
        /*
         * player is above the platform
         */
        // std::cout << "Player is above the platform" << std::endl;
        // std::cout << "Player is above the platform" << std::endl;
        setPosition(p);
        on_platform = true;
        on_platform_id = platform->id;
        if (state == PlayerState::JUMPING_WALKING) {
            // std::cout << "Player landed on platform" << std::endl;
            state = PlayerState::WALKING;
        } else if (state == PlayerState::JUMPING_IDLE) {
            // std::cout << "Player landed on platform" << std::endl;
            state = PlayerState::IDLE;
        }
    } else if (p.y >= (platform->getPosition().y + platform->getSize().y - 1)) {
        /*
         * player is below the platform
         */
        // std::cout << "Player is below the platform" << std::endl;
        setPosition(p);
        on_platform = false;
        on_platform_id = -1;
        collision_direction = CollisionDirection::UP;
    } else if ((p.x + getSize().x) <= (platform->getPosition().x + 1)) {
        /*
         * player is to the left of the platform
         */
        // std::cout << "Player is to the left of the platform" << std::endl;
        setPosition(p.x, getPosition().y);
        on_platform = false;
        on_platform_id = -1;
        collision_direction = CollisionDirection::RIGHT;
    } else if (p.x >= (platform->getPosition().x + platform->getSize().x - 1)) {
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