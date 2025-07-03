#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <memory> 
#include "Common/Config/Config.h"
#include "Common/NotificationBase.h"
#include "Common/FrameInfo.h"

namespace View{
namespace UI {

/**
 * Platform类负责渲染不同类型的平台
 * 支持五种平台类型：
 * - NORMAL (0): 绿色矩形平台
 * - SPIKED (1): 红色矩形平台（危险）
 * - ROLLING (2): 绿色平台+黑色方向箭头
 * - FRAGILE (3): 绿色虚线边框平台
 * - BOUNCY (4): 绿色椭圆形平台
 */
class Platform {
public:
    Platform(int id, sf::RenderWindow& window) : id(id), window(window) {}
    void update(Common::FrameInfo::PlatformInfo platform_info);
    void render();

private:
    // 平台类型常量定义（避免依赖Model层枚举）
    
    int id;
    sf::RenderWindow& window;
    sf::RectangleShape rect;
    sf::Texture* texture;
};

}
}