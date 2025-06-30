#include <iostream>
#include "View/GameView.h"

using View::GameView;

void GameView::init() {
    // total_score_text.setString(view_model->getTotalScore());
    total_score_text.setCharacterSize(24);
    total_score_text.setFillColor(sf::Color::White);
    total_score_text.setFont(font);
    total_score_text.setPosition(15,15);

    // game_time_text.setString(view_model->getGameTime());
    game_time_text.setCharacterSize(24);
    game_time_text.setFillColor(sf::Color::White);
    game_time_text.setFont(font);
    game_time_text.setPosition(15,40);

    if (debug) {
        std::cout << "Debug mode is ON" << std::endl;
        // debug_info_text.setString(view_model->getDebugInfo());
        debug_info_text.setCharacterSize(24);
        debug_info_text.setFillColor(sf::Color::White);
        debug_info_text.setFont(font);
        debug_info_text.setPosition(15,65);
    }

    player.init();

}

void GameView::notification_callback(Common::NotificationParam* param, void* view) {
    if (!view) return;
    GameView* game_view = static_cast<GameView*>(view);
    switch (param->id) {
        case Common::NotificationId::ChangePlatformsId:
            game_view->platforms_id = dynamic_cast<Common::ChangePlatformsIdParam*>(param)->value;
            break;
        case Common::NotificationId::ChangeGameFrame:
            Common::FrameInfo frame_info = dynamic_cast<Common::ChangeGameFrameParam*>(param)->value;
            game_view->update(frame_info);
            break;
    }
}

void GameView::update(Common::FrameInfo frame_info) {

    total_score_text.setString(frame_info.total_score_text);

    game_time_text.setString(frame_info.game_time_text);

    if (debug) {
        debug_info_text.setString(frame_info.debug_info_text);
    }

    player.update(frame_info.player_info);
    platforms.clear();

    for (int id : platforms_id) {
        View::UI::Platform platform(id,window);
        platform.init();
        platform.update(frame_info.platforms_info[id]);
        platforms.push_back(platform);
    }
}

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

    window.display();
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