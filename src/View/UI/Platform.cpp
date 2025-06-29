#include <View/UI/Platform.h>
#include <ViewModel/GameViewModel.h>
#include <algorithm>

using View::UI::Platform;

void Platform::init(std::shared_ptr<ViewModel::GameViewModel> view_model,int id) {
    this->id = id;
    this->view_model = view_model;
    size = view_model->getPlatformSize();

    switch (view_model->getPlatformTypeById(id)) {
        case View::UI::PlatformType::NORMAL:
            rect.setFillColor(sf::Color::Green);
            break;
        case View::UI::PlatformType::SPIKED:
            rect.setFillColor(sf::Color::Red); // 红色表示危险
            break;
        case View::UI::PlatformType::ROLLING:
            rect.setFillColor(sf::Color::Green); // 绿色填充
            break;
        case View::UI::PlatformType::FRAGILE:
            rect.setFillColor(sf::Color::Transparent); // 透明填充
            rect.setOutlineColor(sf::Color::Green); // 绿色轮廓
            rect.setOutlineThickness(2.0f); // 设置轮廓厚度
            break;
        case View::UI::PlatformType::BOUNCY:
            // 设置椭圆形状用于弹跳平台
            ellipse.setRadius(size.x / 2.0f); // 使用平台宽度的一半作为半径
            ellipse.setScale(1.0f, 0.2f); // 压扁成椭圆（高度压缩到20%，更扁）
            ellipse.setFillColor(sf::Color::Green); // 绿色填充
            ellipse.setPosition(view_model->getPlatformPositionById(id));
            break;
    }

    rect.setPosition(view_model->getPlatformPositionById(id));
    rect.setSize(size);
}

void Platform::render(sf::RenderWindow& window){
    // 检查平台类型并使用相应的渲染方式
    View::UI::PlatformType platformType = view_model->getPlatformTypeById(id);
    
    if (platformType == View::UI::PlatformType::FRAGILE) {
        // 绘制绿色虚线边框
        sf::Vector2f pos = view_model->getPlatformPositionById(id);
        sf::Vector2f size = view_model->getPlatformSize();
        
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
    } else if (platformType == View::UI::PlatformType::ROLLING) {
        // 先绘制绿色平台基础
        window.draw(rect);
        
        // 然后绘制箭头指示滚动方向
        sf::Vector2f pos = view_model->getPlatformPositionById(id);
        sf::Vector2f size = view_model->getPlatformSize();
        bool rolling_right = view_model->getPlatformRollingDirection(id);
        
        sf::Color arrow_color(0, 0, 0); // 黑色箭头，在绿色背景上更醒目
        float arrow_thickness = 3.0f;
        
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
                // 设置原点为三角形中心，确保居中
                arrow_triangle.setOrigin(6, 6);
                arrow_triangle.setPosition(arrow_x, center_y);
                arrow_triangle.setRotation(90); // 旋转90度使箭头指向右边
                window.draw(arrow_triangle);
            } else {
                // 向左的箭头: 使用三角形
                sf::CircleShape arrow_triangle(6, 3); // 半径6，3个顶点形成三角形
                arrow_triangle.setFillColor(arrow_color);
                // 设置原点为三角形中心，确保居中
                arrow_triangle.setOrigin(6, 6);
                arrow_triangle.setPosition(arrow_x, center_y);
                arrow_triangle.setRotation(270); // 旋转270度使箭头指向左边
                window.draw(arrow_triangle);
            }
        }
    } else if (platformType == View::UI::PlatformType::BOUNCY) {
        // 更新椭圆位置并绘制
        ellipse.setPosition(view_model->getPlatformPositionById(id));
        window.draw(ellipse);
    } else {
        // 其他平台类型使用正常的实心矩形渲染
        window.draw(rect);
    }
}
