#pragma once
#include "Model/Model.h"
#include <SFML/Audio.hpp>
#include <chrono>

namespace Model {
class ScoreModel : public Model {
public:
    ScoreModel(sf::Vector2u window_size,std::string score, std::string time);
    ~ScoreModel();
    
    std::string* getScore();
    std::string* getTime();

    void setScore(std::string total_score) {
        score = total_score;
    }
    void setTime(std::string game_time) {
        time = game_time;
    }
    
    // 音频控制方法
    void startBackgroundMusic();
    void stopBackgroundMusic();
    bool isBackgroundMusicPlaying() const;
    
    // 菜单音效方法
    void playMenuClickSound();

private:
    std::string score;
    std::string time;
    sf::Music background_music;
    sf::SoundBuffer menu_click_sound_buffer;
    sf::Sound menu_click_sound;
};
}
