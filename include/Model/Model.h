#pragma once

#include "Common/Trigger.h"

namespace Model {
class Model {
public:
    Model() = default;
    Model(sf::Vector2u window_size) : window_size(window_size){};

    virtual void update(float delta_time) {};
    
    virtual ~Model() {}; 
    
protected:
    sf::Vector2u window_size;
};

};