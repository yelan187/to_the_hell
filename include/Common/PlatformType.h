
#pragma once
namespace Common {

enum class PlatformType {
    WALL,       // 墙壁平台
    NORMAL,     // 普通平台
    SPIKED,     // 带刺平台（踩上即死）
    ROLLING,    // 滚动平台（左右滚动，在平台上时人物会以一定速度水平位移）
    ROLLING_L,  // for game viewmodel
    FRAGILE,    // 脆弱平台（踩到后几秒破碎）
    BOUNCY      // 弹跳平台（踩到后给人物施加向上速度）
};

}