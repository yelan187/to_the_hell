#pragma once
#include "Model/Model.h"
#include <chrono>

namespace Model {
class ScoreModel : public Model {
public:
    ScoreModel(sf::Vector2u window_size,int score, std::chrono::seconds time) : Model(window_size), score(score), time(time) {};
    
    int getScore() const;
    std::chrono::seconds getTime() const;

    void setScore(int total_score) {
        score = total_score;
    }
    void setTime(std::chrono::seconds game_time) {
        time = game_time;
    }

private:
    int score;
    std::chrono::seconds time;
};
}
