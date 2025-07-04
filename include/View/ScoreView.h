#pragma once

#include "View/Page.h"
#include <iostream>
#include <string>
#include <chrono>
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
    void setCurrentSelection(int* selection) {
        current_selection = selection;
    }
    void setMenuOptions(std::vector<std::string>* options) {
        menu_options_text = options;
        initMenuOptions();
    }
    void setTotalScore(std::string* score) {
        total_score = score;
        initTotalScoreText();
    }
    void setGameTime(std::string* time) {
        game_time = time;
        initTimeText();
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
    void init();
    void render() override;
    void handleInput(const sf::Event& event) override;

private:
    // properties
    int* current_selection;
    std::vector<std::string>* menu_options_text;
    std::string* total_score;
    std::string* game_time;
    // command
    Common::CommandBase* navigateUp_command;
    Common::CommandBase* navigateDown_command;
    Common::CommandBase* confirmSelection_command;
    Common::CommandBase* viewmodel_confirm_command;
    // notification
    static void notification_callback(Common::NotificationId id, void* view);
    // others
    sf::Font font;
    sf::Text score_text;
    sf::Text time_text;
    std::vector<sf::Text> menu_options;
    sf::CircleShape option_pointer;
    void initMenuOptions();
    void initTotalScoreText();
    void initTimeText();
};

}
