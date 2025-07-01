#pragma once
#include "Common/Config/Config.h"

// 为了向后兼容，保留Utils命名空间的别名
namespace Utils {
    // 引用新的配置系统
    using namespace Common::Config;
    
    // 向后兼容的常量别名
    const auto& WINDOW_TITLE = GlobalConfig::WINDOW_TITLE;
    const auto& WINDOW_WIDTH = GlobalConfig::WINDOW_WIDTH;
    const auto& WINDOW_HEIGHT = GlobalConfig::WINDOW_HEIGHT;
    const auto& FPS = GlobalConfig::FPS;
    const auto& DEBUG_MODE = GlobalConfig::DEBUG_MODE;
    
    const auto& PLAYER_SIZE = GameConfig::PLAYER_SIZE;
    const auto& PLATFORM_SIZE = GameConfig::PLATFORM_SIZE;
}
