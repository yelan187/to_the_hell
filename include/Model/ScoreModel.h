#pragma once
#include "Model/Model.h"
#include <chrono>

namespace Model {
class ScoreModel : public Model {
public:
    ScoreModel(sf::Vector2u window_size,std::string score, std::string time) : Model(window_size), score(score), time(time) {};
    std::string* getScore();
    std::string* getTime();

    void setScore(std::string total_score) {
        score = total_score;
    }
    void setTime(std::string game_time) {
        time = game_time;
    }

private:
    std::string score;
    std::string time;
};
}
