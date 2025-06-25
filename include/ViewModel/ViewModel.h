#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "Core/Engine.h"

namespace ViewModel {

class ViewModel {
public:
    ViewModel(Core::Engine &engine) : engine(engine) {};
    virtual ~ViewModel() = default;
    using Handler = std::function<void(const std::string&)>;
    
    void Subscribe(const std::string& property, Handler handler) {
        handlers[property].push_back(handler);
    }
    
protected:
    void Notify(const std::string& property) {
        auto it = handlers.find(property);
        if (it != handlers.end()) {
            for (auto& handler : it->second) {
                handler(property);
            }
        }
    }
    std::unordered_map<std::string, std::vector<Handler>> handlers;
    Core::Engine &engine;

};
}