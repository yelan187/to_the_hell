#include <iostream>
#include "View/GameView.h"
#include "Common/SkillID.h"

using View::GameView;

void GameView::init() {
    // 加载字体
    if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    
    // 预加载所有背景
    preloadBackgrounds();
    
    // 预加载动画纹理
    preloadAnimationTextures();
    
    // 设置默认背景
    switchBackground("assets/images/background/misty_forest_1.png");
    
    total_score_text.setCharacterSize(36);  // 从24增加到36 (1.5倍)
    total_score_text.setFillColor(sf::Color::White);
    total_score_text.setFont(font);
    total_score_text.setPosition(15,15);

    game_time_text.setCharacterSize(36);  // 从24增加到36 (1.5倍)
    game_time_text.setFillColor(sf::Color::White);
    game_time_text.setFont(font);
    game_time_text.setPosition(15,55);  // 调整Y位置适应更大字体

    // 初始化玩家HP文本
    player_hp_text.setCharacterSize(36);  // 从24增加到36 (1.5倍)
    player_hp_text.setFillColor(sf::Color::Red);
    player_hp_text.setFont(font);
    player_hp_text.setPosition(15, 135);  // 调整HP位置，适应更大字体

    platform_info_text.setCharacterSize(36);  // 从24增加到36 (1.5倍)
    platform_info_text.setFillColor(sf::Color::White);
    platform_info_text.setFont(font);
    platform_info_text.setPosition(15, 95);  // 调整平台信息位置，适应更大字体

    player.init();

    choices.clear();
    for (int i = 0; i < Common::Config::GameConfig::MAX_CHOICES; ++i) {
        choices.push_back(
            new View::UI::Choice(window, i)
        );
        choices[i]->init();
    }
}



void GameView::notification_callback(Common::NotificationId id, void* view) {
    if (!view) return;
    GameView* game_view = static_cast<GameView*>(view);
    switch (id) {
        case Common::NotificationId::ChangeGameFrame:
            game_view->updateframe();
            break;
        case Common::NotificationId::GameOver:
            game_view->gameOver();
            break;
        case Common::NotificationId::Choose:
            game_view->choose();
            break;
        case Common::NotificationId::EndChoose:
            game_view->endChoose();
            break;
    }
}

// update
void GameView::endChoose() {
    update_command = dummy_command;
    dummy_command = nullptr;
}

void GameView::choose() {
    dummy_command = update_command;
    update_command = nullptr;
    for (int index = 0; index < Common::Config::GameConfig::MAX_CHOICES; ++index) {
        choices[index]->update((*choices_info)[index]);
    }
}

void GameView::gameOver() {
    Common::GameOverCommandParam param;
    param.value.total_score = *total_score;
    param.value.game_time = *game_time;
    gameover_command->execute(param);
}

void GameView::updateframe() {
    // 检查背景是否需要更新
    if (frame_info->background_info.changed) {
        switchBackground(frame_info->background_info.background_file);
    }
    
    total_score_text.setString(*total_score);
    game_time_text.setString(*game_time);
    
    // 更新HP文本显示
    player_hp_text.setString("HP: " + std::to_string(frame_info->player_info.hp) + "/" + std::to_string(frame_info->player_info.max_hp));
    
    platform_info_text.setString(*platform_info);
    player.update(frame_info->player_info);
    platforms.clear();

    // platform
    for (int id : frame_info->platforms_id) {
        View::UI::Platform platform(id, window);
        platform.update(frame_info->platforms_info[id]);
        platforms.push_back(platform);
    }
    
    // enemy
    enemies.clear();
    for (int id : frame_info->enemies_id) {
        View::UI::Enemy enemy(id, window);
        enemy.update(frame_info->enemies_info.at(id));
        enemies.push_back(enemy);
    }
    
    // bullet
    bullets.clear();
    for (int id : frame_info->bullets_id) {
        View::UI::Bullet bullet(id, window);
        bullet.update(frame_info->bullets_info.at(id));
        bullets.push_back(bullet);
    }
    
    // pickups
    pickups.clear();
    for (int id : frame_info->pickups_id) {
        pickups.emplace_back(id, window);
        pickups.back().update(frame_info->pickups_info.at(id));
    }
    
    // skills
    skill_bar.updateSkills(frame_info->skills_info);
}

void GameView::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::W:
                playerJumpCommand->execute();
                break;
            case sf::Keyboard::S:
                playerDownCommand->execute();
                break;
            case sf::Keyboard::A:
                playerLeftCommand->execute();
                break;
            case sf::Keyboard::D:
                playerRightCommand->execute();
                break;
            case sf::Keyboard::J: // 箭矢射击技能
                {
                    Common::PlayerSkillParam skill_param;
                    skill_param.value.skill_id = Common::SkillID::ARROW_SHOT;
                    skill_param.value.direction = sf::Vector2f(1.0f, 0.0f);
                    playerSkillCommand->execute(skill_param);
                }
                break;
            case sf::Keyboard::U: // 冲刺技能
                {
                    Common::PlayerSkillParam skill_param;
                    skill_param.value.skill_id = Common::SkillID::SPRINT;
                    skill_param.value.direction = sf::Vector2f(1.0f, 0.0f);
                    playerSkillCommand->execute(skill_param);
                }
                break;
            default:
                break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::A:
                playerStopLeftCommand->execute();
                break;
            case sf::Keyboard::D:
                playerStopRightCommand->execute();
                break;
            case sf::Keyboard::W:
                playerStopJumpCommand->execute();
                break;
            case sf::Keyboard::S:
                playerStopDownCommand->execute();
                break;
            case sf::Keyboard::Num1:
            case sf::Keyboard::Num2:
            case sf::Keyboard::Num3:
                {
                    Common::ChooseParam param;
                    param.value = event.key.code - sf::Keyboard::Num1;
                    chooseCommand->execute(param);
                }
            default:
                break;
        }
    } else if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mouse_pos(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y));
            int clicked_skill = skill_bar.getClickedSkill(mouse_pos);
            
            if (clicked_skill >= 0) {
                // 将点击的技能索引映射到对应的SkillID
                Common::SkillID skill_id;
                switch (clicked_skill) {
                    case 0: skill_id = Common::SkillID::ARROW_SHOT; break;
                    case 1: skill_id = Common::SkillID::SPRINT; break;
                    case 2: skill_id = Common::SkillID::GROUND_PENETRATION; break;
                    default: return; // 无效索引
                }
                
                Common::PlayerSkillParam skill_param;
                skill_param.value.skill_id = skill_id;
                skill_param.value.direction = sf::Vector2f(1.0f, 0.0f); // 默认方向
                playerSkillCommand->execute(skill_param);
            }
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::A:
                playerStopLeftCommand->execute();
                break;
            case sf::Keyboard::D:
                playerStopRightCommand->execute();
                break;
            case sf::Keyboard::W:
                playerStopJumpCommand->execute();
                break;
            case sf::Keyboard::S:
                playerStopDownCommand->execute();
                break;
            default:
                break;
        }
    }
}

// render
void GameView::render() {
    window.clear(sf::Color::Black);
    
    // 更新背景过渡效果
    static sf::Clock delta_clock;
    float delta_time = delta_clock.restart().asSeconds();
    updateBackgroundTransition(delta_time);
    
    // 绘制背景（支持淡入淡出）
    if (is_transitioning) {
        // 过渡期间：先绘制当前背景，再绘制下一个背景（带透明度）
        window.draw(current_background_sprite);
        sf::Uint8 alpha = static_cast<sf::Uint8>(255 * transition_progress);
        next_background_sprite.setColor(sf::Color(255, 255, 255, alpha));
        window.draw(next_background_sprite);
    } else {
        // 正常状态：只绘制当前背景
        window.draw(current_background_sprite);
    }
    
    window.draw(game_time_text);
    window.draw(total_score_text);
    window.draw(player_hp_text);

    window.draw(platform_info_text);

    player.render();

    for (auto &p: platforms)
        p.render();
        
    // 渲染敌人
    for (auto &e: enemies)
        e.render();
        
    // 渲染子弹
    for (auto &b: bullets)
        b.render();
        
    // 渲染豆子
    for (auto &pickup: pickups)
        pickup.render();
        
    // 渲染动画
    if (frame_info) {
        for (const auto& animation_pair : frame_info->animations) {
            auto animation_data = animation_pair.second;
            
            // 渲染当前帧（纹理已在初始化时预加载，直接指定纹理路径）
            animation_renderer.render(
                animation_data.position,
                animation_data.size,
                animation_data.current_frame_rect,
                255.0f,
                animation_data.texture_path
            );
        }
    }
        
    // 渲染技能栏
    skill_bar.render(window);
    
    if (update_command == nullptr) {
        sf::RectangleShape overlay;
        overlay.setSize(sf::Vector2f(window_size.x, window_size.y));
        overlay.setPosition(0, 0);
        overlay.setFillColor(sf::Color(0, 0, 0, 128));
        window.draw(overlay);
        for(auto& choice : choices) {
            choice->render();
        }
    }

    window.display();
}

void GameView::preloadBackgrounds() {
    // 预加载所有背景图片
    std::vector<std::string> background_files = {
        "assets/images/background/misty_forest_1.png",
        "assets/images/background/misty_forest_2.png"
        // 可以在这里添加更多背景文件
    };
    
    for (const auto& file : background_files) {
        sf::Texture texture;
        if (texture.loadFromFile(file)) {
            background_textures[file] = texture;
            // std::cout << "Preloaded background: " << file << std::endl;
        } else {
            std::cerr << "Failed to preload background: " << file << std::endl;
        }
    }
}

void GameView::preloadAnimationTextures() {
    // 预加载所有动画纹理
    std::vector<std::string> animation_files = {
        "assets/images/others/Hit-Yellow.png"
        // 可以在这里添加更多动画文件
    };
    
    for (const auto& file : animation_files) {
        animation_renderer.loadTexture(file);
        // std::cout << "Preloaded animation texture: " << file << std::endl;
    }
}

void GameView::setBackgroundScale(sf::Sprite& sprite, const sf::Texture& texture) {
    // 缩放背景以覆盖整个窗口
    sf::Vector2u texture_size = texture.getSize();
    float scale_x = static_cast<float>(window_size.x) / texture_size.x;
    float scale_y = static_cast<float>(window_size.y) / texture_size.y;
    sprite.setScale(scale_x, scale_y);
}

void GameView::switchBackground(const std::string& background_file) {
    // 如果已经是当前背景，则不切换
    if (current_background_name == background_file) {
        return;
    }
    
    // 检查背景是否已预加载
    auto it = background_textures.find(background_file);
    if (it == background_textures.end()) {
        std::cerr << "Background not preloaded: " << background_file << std::endl;
        return;
    }
    
    // 如果是第一次设置背景
    if (current_background_name.empty()) {
        current_background_sprite.setTexture(it->second);
        setBackgroundScale(current_background_sprite, it->second);
        current_background_name = background_file;
        return;
    }
    
    // 开始过渡
    next_background_sprite.setTexture(it->second);
    setBackgroundScale(next_background_sprite, it->second);
    is_transitioning = true;
    transition_progress = 0.0f;
    
    // std::cout << "Starting background transition to: " << background_file << std::endl;
}

void GameView::updateBackgroundTransition(float delta_time) {
    if (!is_transitioning) {
        return;
    }
    
    transition_progress += delta_time * Common::Config::GameConfig::BACKGROUND_TRANSITION_SPEED;
    
    if (transition_progress >= 1.0f) {
        // 过渡完成
        current_background_sprite = next_background_sprite;
        current_background_sprite.setColor(sf::Color::White); // 恢复完全不透明
        current_background_name = frame_info->background_info.background_file;
        is_transitioning = false;
        transition_progress = 0.0f;
        
        // std::cout << "Background transition completed" << std::endl;
    }
}