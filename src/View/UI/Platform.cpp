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
            rect.setFillColor(sf::Color::Blue); // 蓝色表示滚动
            break;
        case View::UI::PlatformType::FRAGILE:
            rect.setFillColor(sf::Color::Transparent); // 透明填充
            rect.setOutlineColor(sf::Color::Green); // 绿色轮廓
            rect.setOutlineThickness(2.0f); // 设置轮廓厚度
            break;
        case View::UI::PlatformType::BOUNCY:
            rect.setFillColor(sf::Color::Magenta); // 紫色表示弹跳
            break;
    }

    rect.setPosition(view_model->getPlatformPositionById(id));
    rect.setSize(size);
}

void Platform::render(sf::RenderWindow& window){
    // 检查是否是脆弱平台，需要特殊的虚线渲染
    if (view_model->getPlatformTypeById(id) == View::UI::PlatformType::FRAGILE) {
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
    } else {
        // 其他平台类型使用正常的实心矩形渲染
        window.draw(rect);
    }
}
