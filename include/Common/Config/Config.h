#pragma once

// 配置系统主头文件 - 方便其他模块统一包含
#include "Common/Config/GlobalConfig.h"
#include "Common/Config/GameConfig.h"

namespace Common {
namespace Config {

// ====================================
// 配置管理器 - 统一管理所有配置
// ====================================
class ConfigManager {
public:
    // 初始化配置系统
    static void initialize();
    
    // 重置所有配置为默认值
    static void resetAllToDefaults();
    
private:
    ConfigManager() = default;
    static bool initialized;
};

}
}
