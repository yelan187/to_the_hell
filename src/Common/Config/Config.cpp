#include "Common/Config/Config.h"
#include <iostream>

namespace Common {
namespace Config {

// 配置管理器实现
bool ConfigManager::initialized = false;

void ConfigManager::initialize() {
    if (initialized) return;
    
    std::cout << "Initializing Config System" << std::endl;
    initialized = true;
}

void ConfigManager::resetAllToDefaults() {
    std::cout << "Resetting all configurations to defaults" << std::endl;
    
    // 重置游戏配置
    GameConfig::resetToDefaults();
    
    // 重置全局配置中的可变部分
    GlobalConfig::setDebugMode(true);
    
    std::cout << "All configurations reset to defaults" << std::endl;
}

}
}
