#include "Model/Entities/Pickup.h"
#include "Model/GameModel.h"

using Model::Entities::Pickup;

Pickup::Pickup(int id, PickupType type, sf::Vector2f position, sf::Vector2f size, GameModel* game_model, int platform_id)
    : id(id), type(type), position(position), size(size), game_model(game_model), platform_id(platform_id) {
    
    // 设置豆子的默认移动速度，与世界滚动速度同步
    scroll_speed = 100.0f; 
    velocity = sf::Vector2f(0, -scroll_speed); // 默认向上移动（补偿世界向下滚动）
    
    // 如果是普通豆子且指定了平台ID，将豆子精确定位到平台上方
    if (platform_id != -1 && type == PickupType::NORMAL_DOT) {
        auto platforms = game_model->getPlatforms();
        if (platforms.find(platform_id) != platforms.end()) {
            sf::Vector2f platform_pos = platforms[platform_id]->getPosition();
            // 确保豆子恰好位于平台上方，不会嵌入平台内部
            this->position.y = platform_pos.y - size.y;
        }
    }
}

void Pickup::update(float delta_time) {
    // 更新豆子的运动状态和位置
    updateMovement(delta_time);
    position += velocity * delta_time;
}

void Pickup::updateMovement(float delta_time) {
    if (isOnPlatform()) {
        // 普通豆子附着在平台上，需要跟随平台一起移动
        auto platforms = game_model->getPlatforms();
        if (platforms.find(platform_id) != platforms.end()) {
            // 平台仍然存在，更新豆子位置以保持在平台上方
            sf::Vector2f platform_pos = platforms[platform_id]->getPosition();
            position.y = platform_pos.y - size.y; // 保持在平台顶部
            velocity = sf::Vector2f(0, -scroll_speed); // 补偿世界滚动
        } else {
            // 平台已被销毁，豆子脱离平台继续向上移动
            velocity = sf::Vector2f(0, -scroll_speed);
            platform_id = -1; // 清除平台关联
        }
    } else {
        // 自由移动的豆子（五角星豆子或脱离平台的普通豆子）
        velocity = sf::Vector2f(0, -scroll_speed); // 向上移动以补偿世界滚动
        
        // 只有普通豆子才能附着到平台上
        if (type == PickupType::NORMAL_DOT) {
            auto platforms = game_model->getPlatforms();
            
            // 检查是否有平台可以附着
            for (const auto& platform_pair : platforms) {
                sf::Vector2f platform_pos = platform_pair.second->getPosition();
                sf::Vector2f platform_size = platform_pair.second->getSize();
                
                // 碰撞检测：豆子是否接触到平台顶部区域
                bool x_overlap = (position.x + size.x > platform_pos.x && 
                                position.x < platform_pos.x + platform_size.x);
                bool y_contact = (position.y + size.y >= platform_pos.y - 5 && 
                                position.y <= platform_pos.y + 10);
                
                if (x_overlap && y_contact) {
                    // 豆子成功附着到平台上
                    platform_id = platform_pair.first; // 记录附着的平台ID
                    position.y = platform_pos.y - size.y; // 精确定位到平台顶部
                    velocity = sf::Vector2f(0, -scroll_speed); // 跟随平台移动
                    break; // 找到第一个可附着的平台即可
                }
            }
        }
    }
}

bool Pickup::outOfWindow(sf::Vector2u window_size) const {
    // 检查豆子是否移出游戏窗口边界
    // 豆子从底部生成向上移动，主要检查左右边界和顶部边界
    return position.x + size.x < 0 ||      // 完全移出左边界
           position.x > window_size.x ||   // 完全移出右边界  
           position.y + size.y < 0;        // 完全移出顶部边界
}

bool Pickup::collidesWith(sf::Vector2f target_pos, sf::Vector2f target_size) const {
    // 使用轴对齐边界框(AABB)检测豆子与目标对象的碰撞
    sf::Vector2f pickup_lt = position;                 // 豆子左上角
    sf::Vector2f pickup_rb = pickup_lt + size;         // 豆子右下角
    
    sf::Vector2f target_lt = target_pos;               // 目标对象左上角
    sf::Vector2f target_rb = target_lt + target_size;  // 目标对象右下角
    
    // 使用分离轴定理：如果两个矩形在任一轴上分离，则不碰撞
    // 返回true表示发生碰撞，false表示未碰撞
    return !(pickup_rb.x <= target_lt.x ||   // 豆子在目标左侧
             pickup_lt.x >= target_rb.x ||   // 豆子在目标右侧
             pickup_rb.y <= target_lt.y ||   // 豆子在目标上方
             pickup_lt.y >= target_rb.y);    // 豆子在目标下方
}

int Pickup::getScore() const {
    // 根据豆子类型返回对应的分数值
    switch (type) {
        case PickupType::NORMAL_DOT:
            return 2;  // 普通豆子价值2分
        case PickupType::STAR_DOT:
            return 5;  // 五角星豆子价值5分
        default:
            return 0;  // 未知类型不给分
    }
}
