#include <View/UI/Platform.h>

using View::UI::Platform;

void Platform::init() {
    size = sf::Vector2f(100, 20);
    rect.setSize(size);
}

void Platform::render(){
    window.draw(rect);
}

void Platform::update(Common::FrameInfo::PlatformInfo platform_info) {
    switch (platform_info.type) {
        case Utils::PlatformType::NORMAL:
            rect.setFillColor(sf::Color::Green);
            break;
    }
    rect.setPosition(platform_info.position);
}