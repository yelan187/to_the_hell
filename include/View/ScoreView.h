#pragma once

#include "View/Page.h"
#include <iostream>
#include <string>
namespace View {

class ScoreView : public Page {
public:
    ScoreView(std::string game_title, sf::Vector2u window_size, int fps, sf::RenderWindow& window) : Page(game_title, window_size, fps, window){
        if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
            return;
        }
    }
    // properties
    void setCurrentSelection(int selection) {
        current_selection = selection;
    }
    void setMenuOptions(const std::vector<std::string>& options) {
        menu_options.clear();
        for (const auto& option : options) {
            sf::Text text;
            text.setString(option);
            text.setFont(font);
            text.setCharacterSize(40);
            text.setFillColor(sf::Color::White);
            auto textRect = text.getLocalBounds();
            text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
            text.setPosition(
                window_size.x / 2.0f, 
                window_size.y / 2.f + 50.f + menu_options.size() * 50.f
            );
            menu_options.push_back(text);
        }
    }
    void setTotalScoreText(int score) {
        score_text.setFont(font);
        score_text.setString("Score: " + std::to_string(score));
        score_text.setCharacterSize(50);
        score_text.setFillColor(sf::Color::White);
        sf::FloatRect textRect = score_text.getLocalBounds();
        score_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        score_text.setPosition(window_size.x / 2.0f, window_size.y / 2.0f - 150.f);
    }
    void setTimeText(std::chrono::seconds time) {
        time_text.setFont(font);
        time_text.setString("Time: " + std::to_string(time.count()) + "s");
        time_text.setCharacterSize(50);
        time_text.setFillColor(sf::Color::White);
        sf::FloatRect textRect = time_text.getLocalBounds();
        time_text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
        time_text.setPosition(window_size.x / 2.0f, window_size.y / 2.0f - 50.f);
    }
    // commands
    void setNavigateUpCommand(Common::CommandBase* command) {
        navigateUp_command = command;
    }
    void setNavigateDownCommand(Common::CommandBase* command) {
        navigateDown_command = command;
    }
    void setConfirmSelectionCommand(Common::CommandBase* command) {
        confirmSelection_command = command;
    }
    // notification
    Common::NotificationFunc getNotificationCallback() {
        return &notification_callback;
    }
    // update
    void updateCurrentSelection();
    void init();
    void render() override;
    void handleInput(const sf::Event& event) override;

private:
    static void notification_callback(Common::NotificationParam* param, void* view);

    // menu info
    int current_selection;
    sf::Font font;
    sf::Text score_text;
    sf::Text time_text;
    std::vector<sf::Text> menu_options;

    sf::CircleShape option_pointer;
    // command
    Common::CommandBase* navigateUp_command;
    Common::CommandBase* navigateDown_command;
    Common::CommandBase* confirmSelection_command;
};

}
