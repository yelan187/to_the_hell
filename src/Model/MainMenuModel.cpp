#include <iostream>
#include "Model/MainMenuModel.h"

using Model::MainMenuModel;

// ==================== 构造函数和析构函数 ====================

MainMenuModel::MainMenuModel() : Model() {
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
    background_music.setVolume(70.0f);  // 稍低的音量适合菜单音乐
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
