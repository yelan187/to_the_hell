#include <iostream>
#include "View/GameView.h"

using View::GameView;

void GameView::init() {
    total_score_text.setCharacterSize(24);
    total_score_text.setFillColor(sf::Color::White);
    total_score_text.setFont(font);
    total_score_text.setPosition(15,15);

    game_time_text.setCharacterSize(24);
    game_time_text.setFillColor(sf::Color::White);
    game_time_text.setFont(font);
    game_time_text.setPosition(15,40);

    if (debug) {
        debug_info_text.setCharacterSize(24);
        debug_info_text.setFillColor(sf::Color::White);
        debug_info_text.setFont(font);
        debug_info_text.setPosition(15,65);
    }

    player.init();
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
    }
}

// update
void GameView::gameOver() {
    Common::GameOverCommandParam param;
    param.value.total_score = *total_score;
    param.value.game_time = *game_time;
    gameover_command->execute(param);
}

void GameView::updateframe() {
    total_score_text.setString(*total_score);
    game_time_text.setString(*game_time);
    if (debug) {
        debug_info_text.setString(*debug_info);
    }
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
            case sf::Keyboard::J:
                {
                    Common::PlayerSkillParam skill_param;
                    skill_param.value.skill_id = Common::SkillID::ARROW_SHOT;
                    skill_param.value.direction = sf::Vector2f(1.0f, 0.0f);
                    playerSkillCommand->execute(skill_param);
                }
                break;
            case sf::Keyboard::U:
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
            default:
                break;
        }
    }
}

// render
void GameView::render() {
    window.clear(sf::Color::Black);
    
    window.draw(game_time_text);
    window.draw(total_score_text);

    if (debug) {
        window.draw(debug_info_text);
    }

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
        
    // 渲染技能栏
    skill_bar.render(window);
    
    window.display();
}