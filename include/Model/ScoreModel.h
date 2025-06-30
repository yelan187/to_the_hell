#pragma once
#include "Model/Model.h"
#include <chrono>

namespace Model {
class ScoreModel : public Model {
public:
    ScoreModel(sf::Vector2u window_size,int score, std::chrono::seconds time) : Model(window_size), score(score), time(time) {};
    
    int getScore() const;
    std::chrono::seconds getTime() const;

private:
    int score;
    std::chrono::seconds time;
};
}
