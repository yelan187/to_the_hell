#pragma once

#include <memory>
#include "Common/CommandBase.h"
#include "Common/PageState.h"

#include "View/MainMenuView.h"
#include "ViewModel/MainMenuViewModel.h"
#include "Model/MainMenuModel.h"

#include "View/GameView.h"
#include "ViewModel/GameViewModel.h"
#include "Model/GameModel.h"

#include "View/ScoreView.h"
#include "ViewModel/ScoreViewModel.h"
#include "Model/ScoreModel.h"

namespace App {
class GameApp {
public:    
    GameApp(std::string game_title, sf::Vector2u window_size, int fps, bool debug=false);
    void run();
    void changePage(Common::PAGE_STATE new_page_state, bool init = true, void* info = nullptr);

private:
    // Main Menu Page
    std::shared_ptr<View::MainMenuView> mainmenu_view;
    std::shared_ptr<ViewModel::MainMenuViewModel> mainmenu_view_model;
    std::shared_ptr<Model::MainMenuModel> mainmenu_model;
    void initMainMenu();

    // Game Page
    std::shared_ptr<View::GameView> game_view;
    std::shared_ptr<ViewModel::GameViewModel> game_view_model;
    std::shared_ptr<Model::GameModel> game_model;
    void initGame();

    // Score Page
    typedef struct scoreInfo {
        int total_score;
        std::chrono::seconds game_time;
    } ScoreInfo;
    std::shared_ptr<View::ScoreView> score_view;
    std::shared_ptr<ViewModel::ScoreViewModel> score_view_model;
    std::shared_ptr<Model::ScoreModel> score_model;
    void initScore(void* info);

    // current page
    Common::PAGE_STATE current_page_state = Common::PAGE_STATE::MAIN_MENU;
    std::shared_ptr<View::Page> page;

    // game info
    sf::RenderWindow window;
    std::string game_title;
    sf::Vector2u window_size;
    int fps;
    bool debug;

public:
    class ChangePageCommand : public Common::CommandBase {
    public:
        ChangePageCommand(GameApp* app) : app(app) {}
        void execute(Common::CommandParam& params) override;
    private:
        GameApp* app;
    };
    class GameOverCommand : public Common::CommandBase {
    public:
        GameOverCommand(GameApp* app) : app(app) {}
        void execute(Common::CommandParam& params) override;
    private:
        GameApp* app;
    };
private:
    ChangePageCommand change_page_command;
    GameOverCommand gameover_command;
};

}
