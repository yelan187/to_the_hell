#pragma once
#include "Model/Model.h"

namespace Model {
class GameModel : public Model {
public:
    GameModel(Core::Engine &engine) : Model(engine) {};
    
    int getTotalScore() const { return total_score; }

private:
    int total_score;

};
}