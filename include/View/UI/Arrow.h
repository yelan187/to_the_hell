#pragma once

#include <SFML/Graphics.hpp>
#include "Model/Entities/Arrow.h"

namespace View {
    namespace UI {
        class Arrow {
        private:
            sf::ConvexShape arrow_shape;
            sf::Vector2f position;
            sf::Vector2f size;
            bool facing_right;
            
        public:
            Arrow();
            
            void setPosition(sf::Vector2f position);
            void setSize(sf::Vector2f size);
            void setFacingRight(bool facing_right);
            void draw(sf::RenderWindow& window) const;
            
        private:
            void updateArrowShape();
        };
    }
}
