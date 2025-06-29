#pragma once

// include View/ViewModel/Model
#include <memory>
#include "Common/CommandBase.h"
#include "View/MainMenuView.h"
#include "ViewModel/MainMenuViewModel.h"

namespace App {
class GameApp {
public:    
    GameApp(std::string game_title, sf::Vector2u window_size, int fps);
    void run();
    void changePage(View::PAGE_STATE new_page_state, bool init = true);

private:
    // Main Menu Page
    std::shared_ptr<View::MainMenuView> mainmenu_view;
    std::shared_ptr<ViewModel::MainMenuViewModel> mainmenu_view_model;
    void initMainMenu();

    // current page
    View::PAGE_STATE current_page_state = View::PAGE_STATE::MAIN_MENU;
    std::shared_ptr<View::Page> page;

    // game info
    sf::RenderWindow window;
    std::string game_title;
    sf::Vector2u window_size;
    int fps;

public:
    class ChangePageCommand : public Common::CommandBase {
    public:
        ChangePageCommand(GameApp* app) : app(app) {}
        void execute(Common::CommandParam& params) override;
    private:
        GameApp* app;
    };
private:
    ChangePageCommand change_page_command;
};

}
