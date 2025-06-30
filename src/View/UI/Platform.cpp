#include <View/UI/Platform.h>

using View::UI::Platform;

void Platform::render(){
    window.draw(rect);
}

void Platform::update(Common::FrameInfo::PlatformInfo platform_info) {
    rect.setSize(platform_info.size);
    rect.setFillColor(platform_info.color);
    rect.setPosition(platform_info.position);
}