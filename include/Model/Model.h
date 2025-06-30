#pragma once

#include "Common/Trigger.h"

namespace Model {
class Model {
public:
    Model() = default;
    Model(sf::Vector2u window_size) : window_size(window_size){};
    Common::Trigger& getTrigger() {
        return trigger;
    }
    
    virtual ~Model() {}; 
    
protected:
    sf::Vector2u window_size;
    Common::Trigger trigger;
};

};