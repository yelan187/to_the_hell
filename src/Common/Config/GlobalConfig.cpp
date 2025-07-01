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

// 调试配置
bool GlobalConfig::DEBUG_MODE = true;



}
}
