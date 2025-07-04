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
    
    // 菜单音效方法
    void playMenuClickSound();

private:
    sf::Music background_music;
    sf::SoundBuffer menu_click_sound_buffer;
    sf::Sound menu_click_sound;
};
}