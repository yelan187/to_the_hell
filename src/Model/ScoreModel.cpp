#include <iostream>
#include "Model/ScoreModel.h"

using Model::ScoreModel;

// ==================== 构造函数和析构函数 ====================

ScoreModel::ScoreModel(sf::Vector2u window_size, std::string score, std::string time) 
    : Model(window_size), score(score), time(time) {
    // 初始化菜单点击音效
    if (!menu_click_sound_buffer.loadFromFile("assets/sounds/Menu Selection Click.wav")) {
        std::cerr << "Failed to load menu click sound: assets/sounds/Menu Selection Click.wav" << std::endl;
    } else {
        menu_click_sound.setBuffer(menu_click_sound_buffer);
        menu_click_sound.setVolume(50.0f); // 设置音量
    }
    
    startBackgroundMusic();
}

ScoreModel::~ScoreModel() {
    stopBackgroundMusic();
}

// ==================== 分数和时间获取方法 ====================

std::string* ScoreModel::getScore() {
    return &score;
}

std::string* ScoreModel::getTime() {
    return &time;
}

// ==================== 音频系统方法 ====================

void ScoreModel::startBackgroundMusic() {
    stopBackgroundMusic();
    
    if (!background_music.openFromFile("assets/music/rings_music.mp3")) {
        std::cerr << "Failed to load score music: assets/music/rings_music.mp3" << std::endl;
        return;
    }
    
    background_music.setLoop(true);
    background_music.setVolume(60.0f);  // 稍低的音量适合结算页面音乐
    background_music.play();
}

void ScoreModel::stopBackgroundMusic() {
    if (background_music.getStatus() == sf::Music::Playing) {
        background_music.stop();
    }
}

bool ScoreModel::isBackgroundMusicPlaying() const {
    return background_music.getStatus() == sf::Music::Playing;
}

// ==================== 菜单音效方法 ====================

void ScoreModel::playMenuClickSound() {
    if (menu_click_sound.getBuffer() != nullptr) {
        menu_click_sound.play();
    }
}
