#pragma once

#include "View/Page.h"
#include <iostream>

namespace View {

class MainMenuView : public Page {
public:
    MainMenuView(std::string game_title, sf::Vector2u window_size, int fps, sf::RenderWindow& window) : Page(game_title, window_size, fps, window) {
        if (!font.loadFromFile("assets/fonts/fusion.ttf")) {
            std::cerr << "Error loading font!" << std::endl;
            return;
        }    
        if (!title_texture.loadFromFile("assets/images/title.png")) {
            std::cerr << "Error loading title image!" << std::endl;
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
            text.setCharacterSize(50);
            text.setFillColor(sf::Color::White);
            text.setFont(font);
            text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
            text.setPosition(
                window_size.x / 2,
                window_size.y / 2 + (menu_options.size() * text.getCharacterSize() * 1.5f)
            );
            menu_options.push_back(text);
        }
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
    void updateBackgroundParticles(std::vector<sf::Vector2f>* particles);

    void init() override;
    void render() override;
    void handleInput(const sf::Event& event) override;

private:
    static void notification_callback(Common::NotificationParam* param, void* view);
    // menu info
    sf::Texture title_texture;
    sf::Sprite title_sprite;
    std::vector<sf::Text> menu_options;
    int current_selection;
    sf::Font font;
    sf::CircleShape option_pointer;
    std::vector<sf::CircleShape> background_particles;
    // command
    Common::CommandBase* navigateUp_command;
    Common::CommandBase* navigateDown_command;
    Common::CommandBase* confirmSelection_command;
};

}