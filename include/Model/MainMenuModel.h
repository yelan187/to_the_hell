#pragma once
#include "Model/Model.h"
#include <SFML/Audio.hpp>

namespace Model {
class MainMenuModel : public Model {
public:
    MainMenuModel();
    ~MainMenuModel();
    
    // 音频控制方法
    void startBackgroundMusic();
    void stopBackgroundMusic();
    bool isBackgroundMusicPlaying() const;

private:
    sf::Music background_music;
};
}