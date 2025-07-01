#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory> 
#include "Utils/Config.h"
#include "Common/NotificationBase.h"
#include "Model/Entities/Platform.h" // 用于PlatformType枚举

namespace View{
namespace UI {

/**
 * Platform类负责渲染不同类型的平台
 * 支持五种平台类型：
 * - NORMAL: 绿色矩形平台
 * - SPIKED: 红色矩形平台（危险）
 * - ROLLING: 绿色平台+黑色方向箭头
 * - FRAGILE: 绿色虚线边框平台
 * - BOUNCY: 绿色椭圆形平台
 */
class Platform {
public:
    Platform(int id, sf::RenderWindow& window) : id(id), window(window) {}
    void update(Common::FrameInfo::PlatformInfo platform_info);
    void render();

private:
    int id;
    sf::RenderWindow& window;
    sf::RectangleShape rect;
    sf::CircleShape ellipse; // 用于弹跳平台的椭圆形状
    
    // 存储平台信息用于特殊渲染
    Common::FrameInfo::PlatformInfo platform_info;
    int platform_type;
    
    // 特殊平台的渲染方法
    void renderFragilePlatform();
    void renderRollingPlatform();
    void renderBouncyPlatform();
    void setupEllipse(const Common::FrameInfo::PlatformInfo& platform_info);
};

}
}