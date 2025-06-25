#pragma once
#include "Model/Model.h"

namespace Model {
class MainMenuModel : public Model {
public:
    MainMenuModel(Core::Engine &engine) : Model(engine) {}
    void startGame();
    void exitGame();
};
}