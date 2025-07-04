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
        description = "Increases max jump count by 1";
    }
    void apply() override;
};

class SpeedBoost : public Effect {
public:
    SpeedBoost(GameModel* game_model):Effect(game_model) {
        name = "Speed Boost";
        description = "Increases movement speed by 30%";
    }
    void apply() override;
};

class HealthBoost : public Effect {
public:
    HealthBoost(GameModel* game_model):Effect(game_model) {
        name = "Health Boost";
        description = "Increases max HP by 2 and restores 2 HP";
    }
    void apply() override;
};

}
}
