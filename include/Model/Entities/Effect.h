#pragma once
#include <string>
#include <functional>

namespace Model {
    class GameModel;
}

namespace Model {
namespace Entities {

class Effect {
public:
    Effect(GameModel* game_model):game_model(game_model) {}
    virtual ~Effect() {}
    virtual void apply() {};
    std::string getName() const {return name;}
    std::string getDescription() const{return description;}
protected:
    GameModel* game_model;
    std::string name;
    std::string description;
};

class JumpStrength : public Effect {
public:
    JumpStrength(GameModel* game_model):Effect(game_model) {
        name = "Jump Strength";
        description = "Increases jump height by 50%";
    }
    void apply() override;
};

}
}
