#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Core/Engine.h"
#include "Model/Model.h"

namespace ViewModel {

class ViewModel {
public:
    ViewModel(Core::Engine &engine) : engine(engine) {};
    virtual ~ViewModel() = default;
    
protected:
    Core::Engine &engine;
private:
    std::shared_ptr<Model::Model> model;
};
}