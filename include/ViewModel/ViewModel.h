#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Core/Engine.h"
#include "Model/Model.h"

namespace ViewModel {

class ViewModel {
public:
    ViewModel(Core::Engine &engine, sf::Vector2u window_size) 
    : engine(engine), window_size(window_size) {}

    ViewModel(Core::Engine &engine) : engine(engine) {}

    virtual ~ViewModel() = default;

protected:
    Core::Engine &engine;
    sf::Vector2u window_size;

private:
    std::shared_ptr<Model::Model> model;

};
}