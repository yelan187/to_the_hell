#include <View/UI/Platform.h>
#include <algorithm>

using View::UI::Platform;

void Platform::render(){
    window.draw(rect);
}

void Platform::update(Common::FrameInfo::PlatformInfo platform_info) {
    rect.setSize(platform_info.size);
    rect.setPosition(platform_info.position);
    rect.setTexture(platform_info.texture);
}