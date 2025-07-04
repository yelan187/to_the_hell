#include <View/UI/Platform.h>
#include <algorithm>

using View::UI::Platform;

void Platform::render(){
    // 如果没有纹理，设置一个默认颜色作为后备方案
    if (rect.getTexture() == nullptr) {
        rect.setFillColor(sf::Color::Green); // 绿色作为平台的默认颜色
    }
    window.draw(rect);
}

void Platform::update(Common::FrameInfo::PlatformInfo platform_info) {
    rect.setSize(platform_info.size);
    rect.setPosition(platform_info.position);
    // 只有当texture不为空时才设置纹理
    if (platform_info.texture != nullptr) {
        rect.setTexture(platform_info.texture);
    }
}