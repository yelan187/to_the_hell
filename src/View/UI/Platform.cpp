#include <View/UI/Platform.h>
#include <algorithm>

using View::UI::Platform;

void Platform::render(){
    // 根据平台类型使用不同的渲染方式
    if (platform_type == static_cast<int>(Model::Entities::PlatformType::FRAGILE)) {
        renderFragilePlatform();
    } else if (platform_type == static_cast<int>(Model::Entities::PlatformType::ROLLING)) {
        renderRollingPlatform();
    } else if (platform_type == static_cast<int>(Model::Entities::PlatformType::BOUNCY)) {
        renderBouncyPlatform();
    } else {
        // 普通平台和带刺平台使用标准矩形渲染
        window.draw(rect);
    }
}

void Platform::update(Common::FrameInfo::PlatformInfo platform_info) {
    rect.setSize(platform_info.size);
    rect.setFillColor(platform_info.color);
    rect.setPosition(platform_info.position);
    
    // 存储平台信息用于特殊渲染
    this->platform_info = platform_info;
    this->platform_type = platform_info.platform_type;
    
    // 为弹跳平台设置椭圆
    if (platform_type == static_cast<int>(Model::Entities::PlatformType::BOUNCY)) {
        setupEllipse(platform_info);
    }
}

void Platform::renderFragilePlatform() {
    // 绘制绿色虚线边框
    sf::Vector2f pos = platform_info.position;
    sf::Vector2f size = platform_info.size;
    
    // 虚线参数
    float dash_length = 8.0f;
    float gap_length = 4.0f;
    float thickness = 2.0f;
    
    // 绘制上边
    for (float x = pos.x; x < pos.x + size.x; x += dash_length + gap_length) {
        sf::RectangleShape dash;
        dash.setFillColor(sf::Color::Green);
        dash.setPosition(x, pos.y);
        dash.setSize(sf::Vector2f(std::min(dash_length, pos.x + size.x - x), thickness));
        window.draw(dash);
    }
    
    // 绘制下边
    for (float x = pos.x; x < pos.x + size.x; x += dash_length + gap_length) {
        sf::RectangleShape dash;
        dash.setFillColor(sf::Color::Green);
        dash.setPosition(x, pos.y + size.y - thickness);
        dash.setSize(sf::Vector2f(std::min(dash_length, pos.x + size.x - x), thickness));
        window.draw(dash);
    }
    
    // 绘制左边
    for (float y = pos.y; y < pos.y + size.y; y += dash_length + gap_length) {
        sf::RectangleShape dash;
        dash.setFillColor(sf::Color::Green);
        dash.setPosition(pos.x, y);
        dash.setSize(sf::Vector2f(thickness, std::min(dash_length, pos.y + size.y - y)));
        window.draw(dash);
    }
    
    // 绘制右边
    for (float y = pos.y; y < pos.y + size.y; y += dash_length + gap_length) {
        sf::RectangleShape dash;
        dash.setFillColor(sf::Color::Green);
        dash.setPosition(pos.x + size.x - thickness, y);
        dash.setSize(sf::Vector2f(thickness, std::min(dash_length, pos.y + size.y - y)));
        window.draw(dash);
    }
}

void Platform::renderRollingPlatform() {
    // 先绘制绿色平台基础
    window.draw(rect);
    
    // 然后绘制箭头指示滚动方向
    sf::Vector2f pos = platform_info.position;
    sf::Vector2f size = platform_info.size;
    bool rolling_right = platform_info.rolling_direction;
    
    sf::Color arrow_color(0, 0, 0); // 黑色箭头，在绿色背景上更醒目
    
    // 在平台中央绘制多个箭头
    float center_y = pos.y + size.y / 2.0f;
    int num_arrows = 3; // 绘制3个箭头
    float arrow_spacing = size.x / (num_arrows + 1);
    
    for (int i = 1; i <= num_arrows; i++) {
        float arrow_x = pos.x + arrow_spacing * i;
        
        if (rolling_right) {
            // 向右的箭头: 使用三角形
            sf::CircleShape arrow_triangle(6, 3); // 半径6，3个顶点形成三角形
            arrow_triangle.setFillColor(arrow_color);
            arrow_triangle.setOrigin(6, 6);
            arrow_triangle.setPosition(arrow_x, center_y);
            arrow_triangle.setRotation(90); // 旋转90度使箭头指向右边
            window.draw(arrow_triangle);
        } else {
            // 向左的箭头: 使用三角形
            sf::CircleShape arrow_triangle(6, 3); // 半径6，3个顶点形成三角形
            arrow_triangle.setFillColor(arrow_color);
            arrow_triangle.setOrigin(6, 6);
            arrow_triangle.setPosition(arrow_x, center_y);
            arrow_triangle.setRotation(270); // 旋转270度使箭头指向左边
            window.draw(arrow_triangle);
        }
    }
}

void Platform::renderBouncyPlatform() {
    // 绘制椭圆形状
    window.draw(ellipse);
}

void Platform::setupEllipse(const Common::FrameInfo::PlatformInfo& platform_info) {
    // 设置椭圆形状用于弹跳平台（严格按照内层master实现）
    ellipse.setRadius(platform_info.size.x / 2.0f); // 使用平台宽度的一半作为半径
    ellipse.setScale(1.0f, 0.2f); // 压扁成椭圆（高度压缩到20%，更扁）
    ellipse.setFillColor(sf::Color::Green);
    ellipse.setPosition(platform_info.position);
}