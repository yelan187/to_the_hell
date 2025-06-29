#include "Model/Entities/Arrow.h"
#include "Model/GameModel.h"

using Model::Entities::Arrow;

Arrow::Arrow(int id, sf::Vector2f position, sf::Vector2f velocity, sf::Vector2f size, GameModel* game_model)
    : id(id), position(position), velocity(velocity), size(size), game_model(game_model), lifetime(0.0f) {
    // 确保箭矢只进行水平移动，消除任何垂直速度分量
    this->velocity.y = 0.0f;
}

void Arrow::update(float delta_time) {
    // 箭矢水平移动：根据发射方向（左或右）移动
    position.x += velocity.x * delta_time;
    
    // 补偿游戏世界的向下滚动效果，使箭矢在世界坐标系中保持水平飞行
    // 世界向下滚动时，箭矢需要向上移动相同距离来保持相对位置
    position.y -= game_model->getScrollSpeed() * delta_time;
    
    // 更新箭矢的生存时间
    lifetime += delta_time;
}

bool Arrow::outOfWindow(sf::Vector2u window_size) const {
    // 检查箭矢是否飞出游戏窗口边界
    return position.x + size.x < 0 ||      // 完全飞出左边界
           position.x > window_size.x ||   // 完全飞出右边界
           position.y + size.y < 0 ||      // 完全飞出上边界
           position.y > window_size.y;     // 完全飞出下边界
}

bool Arrow::isExpired() const {
    // 检查箭矢是否超过最大生存时间（防止箭矢永久存在）
    return lifetime >= MAX_LIFETIME;
}

bool Arrow::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    // 使用轴对齐边界框(AABB)检测箭矢与目标对象的碰撞
    sf::Vector2f arrow_lt = position;                  // 箭矢左上角
    sf::Vector2f arrow_rb = arrow_lt + size;           // 箭矢右下角
    
    sf::Vector2f target_lt = target_pos;               // 目标对象左上角
    sf::Vector2f target_rb = target_lt + target_size;  // 目标对象右下角
    
    // 使用分离轴定理：如果两个矩形在任一轴上分离，则不碰撞
    // 返回true表示发生碰撞，false表示未碰撞
    return !(arrow_rb.x <= target_lt.x ||   // 箭矢在目标左侧
             arrow_lt.x >= target_rb.x ||   // 箭矢在目标右侧
             arrow_rb.y <= target_lt.y ||   // 箭矢在目标上方
             arrow_lt.y >= target_rb.y);    // 箭矢在目标下方
}
