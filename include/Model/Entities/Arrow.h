#pragma once

#include <SFML/Graphics.hpp>

namespace Model {
    class GameModel;
    
    namespace Entities {
        class Arrow {
        private:
            int id;
            sf::Vector2f position;
            sf::Vector2f velocity;
            sf::Vector2f size;
            GameModel* game_model;
            float lifetime;
            static constexpr float MAX_LIFETIME = 3.0f; // 箭矢存在3秒

        public:
            Arrow(int id, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, GameModel* game_model);
            
            void update(float delta_time);
            bool outOfWindow(sf::Vector2u window_size) const;
            bool isExpired() const;
            bool collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const;
            
            // Getters
            int getId() const { return id; }
            sf::Vector2f getPosition() const { return position; }
            sf::Vector2f getSize() const { return size; }
            sf::Vector2f getVelocity() const { return velocity; }
        };
    }
}
