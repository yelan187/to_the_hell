#include <iostream>
#include "Model/ScoreModel.h"
#include "Core/Engine.h"

namespace Model {

ScoreModel::ScoreModel(Core::Engine &engine, int score, std::chrono::seconds time) : Model(engine), score(score), time(time) {
}

int ScoreModel::getScore() const {
    return score;
}

std::chrono::seconds ScoreModel::getTime() const {
    return time;
}

void ScoreModel::restartGame() {
    engine.startGame();
}

void ScoreModel::backToMainMenu() {
    engine.changePage(Core::Engine::PAGE_STATE::MAIN_MENU);
}

void ScoreModel::exitGame() {
    engine.exitGame();
}

}
