#include "Model/Entities/Player.h"
#include "Model/Entities/Platform.h"
#include "Utils/Config.h"

using Model::Entities::Player;

void Player::jump(float scroll_speed) {
    if (state==PlayerState::IDLE || state == PlayerState::WALKING) {
        state = PlayerState::JUMPING;
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
        velocity.x -= walking_speed;
    } else if (state == PlayerState::JUMPING) {
        state = PlayerState::JUMPING;
        velocity.x -= walking_speed;
    }

}
void Player::walkRight() {
    if (state == PlayerState::IDLE) {
        state = PlayerState::WALKING;
        velocity.x += walking_speed;
    } else if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x += walking_speed;
    } else if (state == PlayerState::JUMPING) {
        state = PlayerState::JUMPING;
        velocity.x += walking_speed;
    }
}
void Player::stopLeft() {
    if (state == PlayerState::IDLE) {
        state = PlayerState::WALKING;
        velocity.x += walking_speed;
    } else if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x += walking_speed;
    } else if (state == PlayerState::JUMPING) {
        state = PlayerState::JUMPING;
        velocity.x += walking_speed;
    }
}
void Player::stopRight() {
    if (state == PlayerState::IDLE) {
        state = PlayerState::WALKING;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::WALKING) {
        state = PlayerState::IDLE;
        velocity.x -= walking_speed;
    } else if (state == PlayerState::JUMPING) {
        state = PlayerState::JUMPING;
        velocity.x -= walking_speed;
    }
}
void Player::updatePosition(float delta_time) {
    position += velocity * delta_time + 0.5f * acceleration * delta_time * delta_time;
}
void Player::updateVelocity(float delta_time) {
    velocity += acceleration * delta_time;
    // Reset acceleration after applying it
    acceleration = sf::Vector2f(0, 0);
}
void Player::update(float delta_time, std::map<int, Platform*>& platforms) {
    // print velocity and acceleration
    // std::cout << "Velocity: " << velocity.x << ", " << velocity.y << std::endl;
    // Check if on_platform_id is valid
    if (on_platform && platforms.count(on_platform_id) > 0 && platforms[on_platform_id]) {
        assocatedVelocity(platforms[on_platform_id]->getVelocity());
    } else {
        addAcceleration(gravity);
    }
    updatePosition(delta_time);
    // // Only restrict the player from flying out of the top window boundary when on a platform
    // if (on_platform && position.y < 0) {
    //     position.y = 0;
    //     velocity.y = 0;
    // }
    // // Reset player position if falling out of the bottom of the window
    // if (position.y > Utils::WINDOW_HEIGHT) {
    //     position.y = 0;
    //     velocity.y = 0;
    // }
    updateVelocity(delta_time);
    on_platform = false;
    for (auto& platform_pair : platforms) {
        Model::Entities::Platform* platform = platform_pair.second;
        adjustPosition(platform);
    }
    if (on_platform) {
        velocity.y = 0; // reset vertical velocity when landing
        if (state == PlayerState::JUMPING) {
            state = PlayerState::IDLE;
        }
    }
}
void Player::adjustPosition(Platform* &platform) {
    sf::Vector2f player_lt = position;
    sf::Vector2f player_rb = player_lt + size;

    sf::Vector2f platform_lt = platform->getPosition();
    sf::Vector2f platform_rb = platform_lt + platform->getSize();

    // which corner of the player is in the platform
    int lt = player_lt.x < platform_rb.x && player_lt.x > platform_lt.x &&
            player_lt.y < platform_rb.y && player_lt.y > platform_lt.y;

    int rb = player_rb.x < platform_rb.x && player_rb.x > platform_lt.x &&
            player_rb.y < platform_rb.y && player_rb.y > platform_lt.y;

    int lb = player_lt.x < platform_rb.x && player_lt.x > platform_lt.x &&
            player_rb.y < platform_rb.y && player_rb.y > platform_lt.y;
    
    int rt = player_rb.x < platform_rb.x && player_rb.x > platform_lt.x &&
            player_lt.y < platform_rb.y && player_lt.y > platform_lt.y;

    switch (lt*8 + lb*4 + rb*2 + rt) {
        case 0b1000: // left top
            setPosition(sf::Vector2f(platform_rb.x, player_lt.y));
            break;
        case 0b0100: // left bottom
            setPosition(sf::Vector2f(player_lt.x, platform_lt.y - size.y));
            on_platform = true;
            on_platform_id = platform->id;
            break;
        case 0b0010: // right bottom
            setPosition(sf::Vector2f(player_lt.x, platform_lt.y - size.y));
            on_platform = true;
            on_platform_id = platform->id;
            break;
        case 0b0001: // right top
            setPosition(sf::Vector2f(platform_lt.x - size.x, player_lt.y));
            break;
        case 0b0110: // right bottom and left bottom
            setPosition(sf::Vector2f(player_lt.x, platform_lt.y - size.y));
            on_platform = true;
            on_platform_id = platform->id;
            break;
        case 0b1001: // left top and right top
            setPosition(sf::Vector2f(player_lt.x, platform_rb.y));
            velocity.y = 0; // reset vertical velocity when landing
            break;
    }
}