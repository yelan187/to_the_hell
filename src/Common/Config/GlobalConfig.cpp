#include "Common/Config/GlobalConfig.h"

namespace Common {
namespace Config {

// ====================================
// 全局配置实现
// ====================================

// 窗口配置
const std::string GlobalConfig::WINDOW_TITLE = "To The Hell";   // 游戏窗口标题
const int GlobalConfig::WINDOW_WIDTH = 800;                     // 窗口宽度
const int GlobalConfig::WINDOW_HEIGHT = 600;                    // 窗口高度
const int GlobalConfig::FPS = 60;                               // 目标帧率

// 调试配置
bool GlobalConfig::DEBUG_MODE = true;                           // 调试模式开关



}
}
