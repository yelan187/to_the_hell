#include <iostream>
#include "Model/MainMenuModel.h"

using Model::MainMenuModel;

// ==================== 构造函数和析构函数 ====================

MainMenuModel::MainMenuModel() : Model() {
    // 初始化菜单点击音效
    if (!menu_click_sound_buffer.loadFromFile("assets/sounds/Menu Selection Click.wav")) {
        std::cerr << "Failed to load menu click sound: assets/sounds/Menu Selection Click.wav" << std::endl;
    } else {
        menu_click_sound.setBuffer(menu_click_sound_buffer);
        menu_click_sound.setVolume(50.0f); // 设置音量
    }
    
    startBackgroundMusic();
}

MainMenuModel::~MainMenuModel() {
    stopBackgroundMusic();
}

// ==================== 音频系统方法 ====================

void MainMenuModel::startBackgroundMusic() {
    stopBackgroundMusic();
    
    if (!background_music.openFromFile("assets/music/rings_music.mp3")) {
        std::cerr << "Failed to load main menu music: assets/music/rings_music.mp3" << std::endl;
        return;
    }
    
    background_music.setLoop(true);
    background_music.setVolume(60.0f);  // 稍低的音量适合菜单音乐
    background_music.play();
}

void MainMenuModel::stopBackgroundMusic() {
    if (background_music.getStatus() == sf::Music::Playing) {
        background_music.stop();
    }
}

bool MainMenuModel::isBackgroundMusicPlaying() const {
    return background_music.getStatus() == sf::Music::Playing;
}

// ==================== 菜单音效方法 ====================

void MainMenuModel::playMenuClickSound() {
    if (menu_click_sound.getBuffer() != nullptr) {
        menu_click_sound.play();
    }
}
