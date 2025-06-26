#pragma once
#include "Model/Model.h"

namespace Model {
class GameModel : public Model {
public:
    GameModel(Core::Engine &engine) : Model(engine), total_score(0) {}
    void update() override;
    
    int getTotalScore() const { return total_score; }

private:
    int total_score;

};
}