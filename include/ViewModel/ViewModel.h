#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Model/Model.h"

namespace ViewModel {

class ViewModel {
public:
    ViewModel(sf::Vector2u window_size) : window_size(window_size) {}

protected:
    // trigger
	Common::Trigger trigger;
    sf::Vector2u window_size;
    
private:
    std::shared_ptr<Model::Model> model;

};
}