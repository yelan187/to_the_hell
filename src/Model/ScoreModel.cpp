#include <iostream>
#include "Model/ScoreModel.h"

using Model::ScoreModel;

std::string* ScoreModel::getScore() {
    return &score;
}

std::string* ScoreModel::getTime() {
    return &time;
}
