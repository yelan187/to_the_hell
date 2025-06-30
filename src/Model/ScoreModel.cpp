#include <iostream>
#include "Model/ScoreModel.h"

using Model::ScoreModel;

int ScoreModel::getScore() const {
    return score;
}

std::chrono::seconds ScoreModel::getTime() const {
    return time;
}
