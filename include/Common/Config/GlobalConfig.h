#pragma once
#include <string>
#include <SFML/Graphics.hpp>

namespace Common {
namespace Config {

// ====================================
// 全局配置 - 基础不易改变的配置
// ====================================
class GlobalConfig {
public:
    // 窗口配置
    static const std::string WINDOW_TITLE;
    static const int WINDOW_WIDTH;
    static const int WINDOW_HEIGHT;
    static const int FPS;
    
    // 调试配置
    static bool DEBUG_MODE;
    
    // 资源路径配置
    static const std::string ASSETS_PATH;
    static const std::string FONTS_PATH;
    static const std::string IMAGES_PATH;
    static const std::string SOUNDS_PATH;
    
    // 基础UI配置
    static const sf::Color DEFAULT_TEXT_COLOR;
    static const sf::Color DEFAULT_BACKGROUND_COLOR;
    
    // 获取窗口大小
    static sf::Vector2u getWindowSize();
    
    // 运行时修改方法
    static void setDebugMode(bool debug);
    
private:
    GlobalConfig() = default; // 静态类，禁止实例化
};

}
}
