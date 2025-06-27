#pragma once
#include "Core/Engine.h"

namespace Model {
class Model {
public:
    Model(Core::Engine &engine) : engine(engine) {};
    Model(Core::Engine &engine,sf::Vector2f window_size) : engine(engine), window_size(window_size){};

    virtual void update(float delta_time) {};
    
    virtual ~Model() {}; 
    
protected:
    Core::Engine &engine;
    sf::Vector2f window_size;
};

};