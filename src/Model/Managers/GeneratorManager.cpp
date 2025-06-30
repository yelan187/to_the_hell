#include "Model/Managers/GeneratorManager.h"
#include "Model/Managers/EntityManager.h"
#include "Model/Entities/Platform.h"
#include "Model/Entities/Enemy.h"
#include "Model/Entities/Pickup.h"
#include "Model/GameConfig.h"
#include <cstdlib>
#include <algorithm>

using namespace Model::Managers;
using namespace Model::Entities;

GeneratorManager::GeneratorManager(sf::Vector2u window_size, EntityManager* entity_manager, Model::GameModel* game_model)
    : window_size(window_size), entity_manager(entity_manager), game_model(game_model), scroll_speed(Model::GameConfig::DEFAULT_SCROLL_SPEED) {
}

void GeneratorManager::update(float delta_time) {
    // 更新平台生成
    platform_generate_interval -= delta_time;
    if (platform_generate_interval <= 0.0f) {
        resetPlatformGenerateInterval();
        generatePlatform();
    }

    // 更新敌人生成
    enemy_generate_interval -= delta_time;
    if (enemy_generate_interval <= 0.0f) {
        resetEnemyGenerateInterval();
        generateEnemy();
    }

    // 更新豆子生成
    pickup_generate_interval -= delta_time;
    if (pickup_generate_interval <= 0.0f) {
        resetPickupGenerateInterval();
        generatePickup();
    }
}

void GeneratorManager::initialize() {
    resetPlatformGenerateInterval();
    resetEnemyGenerateInterval();
    resetPickupGenerateInterval();
}

void GeneratorManager::generateInitialPlatforms() {
    const int initial_platforms = 3;
    for (int i = 0; i < initial_platforms; ++i) {
        sf::Vector2f position(
            static_cast<float>(rand() % static_cast<int>(window_size.x - Model::GameConfig::PLATFORM_SIZE.x)),
            static_cast<float>(window_size.y / 3 + window_size.y / 2 / initial_platforms * i)
        );
        
        // 初始平台都使用普通平台
        entity_manager->addPlatform(PlatformType::NORMAL, position, Model::GameConfig::PLATFORM_SIZE, scroll_speed);
    }
}

void GeneratorManager::generatePlatform() {
    sf::Vector2f position(
        static_cast<float>(rand() % static_cast<int>(window_size.x - Model::GameConfig::PLATFORM_SIZE.x)),
        window_size.y
    );
    
    PlatformType type = getPlatformTypeRand();
    entity_manager->addPlatform(type, position, Model::GameConfig::PLATFORM_SIZE, scroll_speed);
}

void GeneratorManager::generateEnemy() {
    sf::Vector2f position;
    int side = rand() % 4; // 0:上, 1:右, 2:下, 3:左
    
    switch (side) {
        case 0: // 上边
            position = sf::Vector2f(rand() % window_size.x, -Model::GameConfig::ENEMY_SIZE.y);
            break;
        case 1: // 右边
            position = sf::Vector2f(window_size.x, rand() % window_size.y);
            break;
        case 2: // 下边
            position = sf::Vector2f(rand() % window_size.x, window_size.y);
            break;
        case 3: // 左边
            position = sf::Vector2f(-Model::GameConfig::ENEMY_SIZE.x, rand() % window_size.y);
            break;
    }
    
    entity_manager->addEnemy(EnemyType::GHOST, position, Model::GameConfig::ENEMY_SIZE, game_model);
}

void GeneratorManager::generatePickup() {
    PickupType type;
    sf::Vector2f position;
    int target_platform_id = -1;
    
    if (rand() % 100 < 75) {
        // 普通豆子 - 优先选择ID值高的平台
        type = PickupType::NORMAL_DOT;
        
        auto& platforms = entity_manager->getPlatforms();
        if (!platforms.empty()) {
            // 收集所有平台ID并按降序排序
            std::vector<int> platform_ids;
            for (const auto& platform_pair : platforms) {
                platform_ids.push_back(platform_pair.first);
            }
            
            std::sort(platform_ids.begin(), platform_ids.end(), 
                     [](int a, int b) { return a > b; });
            
            // 从前30%的高ID平台中随机选择一个
            int top_count = std::max(1, static_cast<int>(platform_ids.size() * 0.3));
            int selected_index = rand() % top_count;
            target_platform_id = platform_ids[selected_index];
            
            // 在选中的平台上方生成豆子
            auto* platform = entity_manager->getPlatformById(target_platform_id);
            if (platform) {
                sf::Vector2f platform_pos = platform->getPosition();
                sf::Vector2f platform_size_actual = platform->getSize();
                
                position = sf::Vector2f(
                    platform_pos.x + rand() % static_cast<int>(platform_size_actual.x - Model::GameConfig::PICKUP_SIZE.x),
                    platform_pos.y - Model::GameConfig::PICKUP_SIZE.y - 10
                );
            }
        } else {
            position = sf::Vector2f(
                rand() % static_cast<int>(window_size.x - Model::GameConfig::PICKUP_SIZE.x),
                window_size.y + Model::GameConfig::PICKUP_SIZE.y
            );
        }
    } else {
        // 五角星豆子 - 从底部空中生成
        type = PickupType::STAR_DOT;
        position = sf::Vector2f(
            rand() % static_cast<int>(window_size.x - Model::GameConfig::PICKUP_SIZE.x),
            window_size.y + Model::GameConfig::PICKUP_SIZE.y
        );
    }
    
    entity_manager->addPickup(type, position, Model::GameConfig::PICKUP_SIZE, game_model, target_platform_id);
}

void GeneratorManager::resetPlatformGenerateInterval() {
    platform_generate_interval = Model::GameConfig::PLATFORM_GENERATE_BASE_INTERVAL + static_cast<float>(rand() % 1 - 0.5) / 2;
}

void GeneratorManager::resetEnemyGenerateInterval() {
    enemy_generate_interval = Model::GameConfig::ENEMY_GENERATE_BASE_INTERVAL + static_cast<float>(rand() % 2); // 3-5秒
}

void GeneratorManager::resetPickupGenerateInterval() {
    pickup_generate_interval = Model::GameConfig::PICKUP_GENERATE_BASE_INTERVAL + static_cast<float>(rand() % 10) / 10.0f; // 0.8-1.8秒
}

PlatformType GeneratorManager::getPlatformTypeRand() {
    int random = rand() % 100;
    
    if (random < 50) {
        return PlatformType::NORMAL;      // 50% 普通平台
    } else if (random < 65) {
        return PlatformType::ROLLING;     // 15% 滚动平台
    } else if (random < 80) {
        return PlatformType::BOUNCY;      // 15% 弹跳平台
    } else if (random < 95) {
        return PlatformType::FRAGILE;     // 15% 脆弱平台
    } else {
        return PlatformType::SPIKED;      // 5% 带刺平台（最危险，概率最低）
    }
}
