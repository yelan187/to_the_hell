#include "Common/Config/GlobalConfig.h"

namespace Common {
namespace Config {

// ====================================
// 全局配置实现
// ====================================

// 窗口配置
const std::string GlobalConfig::WINDOW_TITLE = "To The Hell";
const int GlobalConfig::WINDOW_WIDTH = 800;
const int GlobalConfig::WINDOW_HEIGHT = 600;
const int GlobalConfig::FPS = 60;

// 调试配置（可运行时修改）
bool GlobalConfig::DEBUG_MODE = true;

// 资源路径配置
const std::string GlobalConfig::ASSETS_PATH = "assets/";
const std::string GlobalConfig::FONTS_PATH = "assets/fonts/";
const std::string GlobalConfig::IMAGES_PATH = "assets/images/";
const std::string GlobalConfig::SOUNDS_PATH = "assets/sounds/";

// 基础UI配置
const sf::Color GlobalConfig::DEFAULT_TEXT_COLOR = sf::Color::White;
const sf::Color GlobalConfig::DEFAULT_BACKGROUND_COLOR = sf::Color::Black;

// 获取窗口大小
sf::Vector2u GlobalConfig::getWindowSize() {
    return sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT);
}

// 运行时修改方法
void GlobalConfig::setDebugMode(bool debug) {
    DEBUG_MODE = debug;
}

}
}
