#include <iostream>
#include "View/GameView.h"

using View::GameView;

GameView::GameView(Core::Engine &engine) : View::Page(engine), debug(true) {
    if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }
    view_model = std::make_shared<ViewModel::GameViewModel>(engine,window_size);
    
    total_score_text.setString(view_model->getTotalScore());
    total_score_text.setCharacterSize(24);
    total_score_text.setFillColor(sf::Color::White);
    total_score_text.setFont(font);
    total_score_text.setPosition(15,15);

    game_time_text.setString(view_model->getGameTime());
    game_time_text.setCharacterSize(24);
    game_time_text.setFillColor(sf::Color::White);
    game_time_text.setFont(font);
    game_time_text.setPosition(15,40);

    if (debug) {
        std::cout << "Debug mode is ON" << std::endl;
        debug_info_text.setString(view_model->getDebugInfo());
        debug_info_text.setCharacterSize(24);
        debug_info_text.setFillColor(sf::Color::White);
        debug_info_text.setFont(font);
        debug_info_text.setPosition(15,65);
    }

    player.init(view_model);
    for (int id : view_model->getPlatformsId()){
        View::UI::Platform p;
        p.init(view_model,id);
        platforms.push_back(p);
    }
}

void GameView::update(float deltaTime) {
    view_model->update(deltaTime);

    total_score_text.setString(view_model->getTotalScore());

    game_time_text.setString(view_model->getGameTime());

    if (debug) {
        debug_info_text.setString(view_model->getDebugInfo());
    }

    player.update(deltaTime);
    
    // 更新平台
    platforms.clear();
    for (int id : view_model->getPlatformsId()){
        View::UI::Platform p;
        p.init(view_model,id);
        platforms.push_back(p);
    }
    
    // 更新敌人 - 只更新现有敌人，添加新敌人，移除不存在的敌人
    std::vector<int> current_enemy_ids = view_model->getEnemiesId();
    
    // 移除不存在的敌人
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), 
        [&current_enemy_ids](const std::pair<int, View::UI::Enemy>& enemy_pair) {
            return std::find(current_enemy_ids.begin(), current_enemy_ids.end(), enemy_pair.first) == current_enemy_ids.end();
        }), enemies.end());
    
    // 添加新敌人并更新现有敌人
    for (int id : current_enemy_ids) {
        auto it = std::find_if(enemies.begin(), enemies.end(), 
            [id](const std::pair<int, View::UI::Enemy>& enemy_pair) {
                return enemy_pair.first == id;
            });
        
        if (it == enemies.end()) {
            // 新敌人，添加到列表
            View::UI::Enemy new_enemy;
            new_enemy.init(view_model, id);
            enemies.push_back(std::make_pair(id, new_enemy));
        } else {
            // 现有敌人，只更新
            it->second.update(deltaTime);
        }
    }
    
    // 更新子弹 - 同样的逻辑
    std::vector<int> current_bullet_ids = view_model->getBulletsId();
    
    // 移除不存在的子弹
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), 
        [&current_bullet_ids](const std::pair<int, View::UI::Bullet>& bullet_pair) {
            return std::find(current_bullet_ids.begin(), current_bullet_ids.end(), bullet_pair.first) == current_bullet_ids.end();
        }), bullets.end());
    
    // 添加新子弹并更新现有子弹
    for (int id : current_bullet_ids) {
        auto it = std::find_if(bullets.begin(), bullets.end(), 
            [id](const std::pair<int, View::UI::Bullet>& bullet_pair) {
                return bullet_pair.first == id;
            });
        
        if (it == bullets.end()) {
            // 新子弹，添加到列表
            View::UI::Bullet new_bullet;
            new_bullet.init(view_model, id);
            bullets.push_back(std::make_pair(id, new_bullet));
        } else {
            // 现有子弹，只更新
            it->second.update(deltaTime);
        }
    }
}

void GameView::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    window.draw(game_time_text);
    window.draw(total_score_text);

    if (debug) {
        window.draw(debug_info_text);
    }

    player.render(window);

    for (auto &p: platforms)
        p.render(window);
        
    for (auto &enemy_pair: enemies)
        enemy_pair.second.render(window);
        
    for (auto &bullet_pair: bullets)
        bullet_pair.second.render(window);

    window.display();
}

void GameView::handleInput(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::W:
                view_model->playerJump();
                break;
            case sf::Keyboard::S:
                view_model->playerDown();
                break;
            case sf::Keyboard::A:
                view_model->playerWalkLeft();
                break;
            case sf::Keyboard::D:
                view_model->playerWalkRight();
                break;
            default:
                break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
            case sf::Keyboard::A:
                view_model->playerStopLeft();
                break;
            case sf::Keyboard::D:
                view_model->playerStopRight();
                break;
            case sf::Keyboard::W:
                view_model->playerStopJump();
                break;
            case sf::Keyboard::S:
                view_model->playerStopDown();
                break;
            default:
                break;
        }
    }
}