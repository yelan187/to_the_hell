#pragma once
#include "Model/Model.h"

namespace Model {
class MainMenuModel : public Model {
public:
    MainMenuModel(Core::Engine &engine) : Model(engine) {}

    void update() override {};
    
    void startGame();
    void exitGame();
};
}