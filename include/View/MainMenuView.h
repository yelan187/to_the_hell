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
    void setCurrentSelection(int* selection) {
        current_selection = selection;
    }
    void setMenuOptions(std::vector<std::string>* options) {
        menu_options_text = options;
        initMenuOptions();
    }
    void setBackgroundParticles(std::vector<sf::Vector2f>* particles) {
        background_particles_pos = particles;
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
    void setViewModelConfirmCommand(Common::CommandBase* command) {
        viewmodel_confirm_command = command;
    }
    // notification
    Common::NotificationFunc getNotificationCallback() {
        return &notification_callback;
    }
    // update
    void updateCurrentSelection();
    void updateBackgroundParticles();

    void init();
    void render() override;
    void handleInput(const sf::Event& event) override;

private:
    // properties
    int* current_selection;
    std::vector<std::string>* menu_options_text;
    std::vector<sf::Vector2f>* background_particles_pos;
    // command
    Common::CommandBase* navigateUp_command;
    Common::CommandBase* navigateDown_command;
    Common::CommandBase* confirmSelection_command;
    Common::CommandBase* viewmodel_confirm_command;
    // notification
    static void notification_callback(Common::NotificationId id, void* view);
    // others
    sf::Texture title_texture;
    sf::Sprite title_sprite;
    std::vector<sf::Text> menu_options;
    sf::Font font;
    sf::CircleShape option_pointer;
    std::vector<sf::CircleShape> background_particles;
    void initMenuOptions();
};

}