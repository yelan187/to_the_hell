#pragma once
#include "Model/Model.h"

namespace Model {
class GameModel : public Model {
public:
    GameModel(Core::Engine &engine) : Model(engine) {}
    void update() override;

};
}