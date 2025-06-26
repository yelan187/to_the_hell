#pragma once
#include "Core/Engine.h"

namespace Model {
class Model {
public:
    Model(Core::Engine &engine) : engine(engine) {};
    
    virtual void update() {};

    virtual ~Model() {}; 
    
protected:
    Core::Engine &engine;

};

};