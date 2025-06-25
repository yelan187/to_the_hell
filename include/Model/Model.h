#pragma once
#include "Core/Engine.h"

namespace Model {
class Model {
public:
    Model(Core::Engine &engine) : engine(engine) {};
protected:
    Core::Engine &engine;

};

};