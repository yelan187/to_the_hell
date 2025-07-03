#include "App/GameApp.h"

using App::GameApp;

// public methods
GameApp::GameApp(std::string game_title, sf::Vector2u window_size, int fps, bool debug) 
    : game_title(std::move(game_title)), window_size(window_size), fps(fps), debug(debug),
      change_page_command(this),
      gameover_command(this) {
    window.create(sf::VideoMode(window_size.x, window_size.y), game_title, sf::Style::Default);
    window.setFramerateLimit(fps);
    changePage(Common::PAGE_STATE::MAIN_MENU);
}

void GameApp::run() {
    while (window.isOpen()) {
        Common::UpdateParam param;
        param.value = 1.0f / fps;
        page->update_command->execute(param);
        page->render();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
                page->handleInput(event);
            }
        }
    }
}

void GameApp::changePage(Common::PAGE_STATE new_page_state, bool init, void* info) {
    // update the page
    current_page_state = new_page_state;
    switch (new_page_state) {
        case Common::PAGE_STATE::GAME:
            if (init) {
                initGame();
            }
            page = game_view;
            break;
        case Common::PAGE_STATE::EXIT:
            page->exit();
            break;
        case Common::PAGE_STATE::SCORE:
            if (init) {
                initScore(info);
            }
            page = score_view;
            break;
        case Common::PAGE_STATE::MAIN_MENU:
            if (init) {
                initMainMenu();
            }
            page = mainmenu_view;
            break;
    }
}

void GameApp::ChangePageCommand::execute(Common::CommandParam& params) {
    Common::ChangePageParam& change_page_param = dynamic_cast<Common::ChangePageParam&>(params);
    app->changePage(
        change_page_param.value.new_page_state,
        change_page_param.value.init
    );
}

void GameApp::GameOverCommand::execute(Common::CommandParam& params) {
    Common::GameOverCommandParam& game_over_param = dynamic_cast<Common::GameOverCommandParam&>(params);
    ScoreInfo score_info = {
        game_over_param.value.total_score,
        game_over_param.value.game_time
    };
    app->changePage(
        Common::PAGE_STATE::SCORE,
        true,
        static_cast<void*>(&score_info)  
    );
}

// private methods
void GameApp::initMainMenu() {
    mainmenu_view = std::make_shared<View::MainMenuView>(
        game_title, window_size, fps, window
    );
    mainmenu_view_model = std::make_shared<ViewModel::MainMenuViewModel>(
        window_size
    );
    mainmenu_model = std::make_shared<Model::MainMenuModel>();
    // set model
    mainmenu_view_model->setModel(mainmenu_model);
    // properties
    mainmenu_view->setCurrentSelection(mainmenu_view_model->getCurrentSelectionIndex());
    mainmenu_view->setMenuOptions(mainmenu_view_model->getMenuOptions());
    mainmenu_view->setBackgroundParticles(mainmenu_view_model->getBackgroundParticles());
    // commands
    mainmenu_view->setUpdateCommand(mainmenu_view_model->getUpdateCommand());
    mainmenu_view->setNavigateUpCommand(mainmenu_view_model->getNavigateUpCommand());
    mainmenu_view->setNavigateDownCommand(mainmenu_view_model->getNavigateDownCommand());
    mainmenu_view->setConfirmSelectionCommand(&change_page_command);
    // notification
    mainmenu_view_model->getTrigger().add(
        mainmenu_view->getNotificationCallback(),
        mainmenu_view.get()
    );

    mainmenu_view->init();
}

void GameApp::initGame() {
    game_view = std::make_shared<View::GameView>(
        game_title, window_size, fps, window, debug
    );
    game_view_model = std::make_shared<ViewModel::GameViewModel>(window_size);
    game_model = std::make_shared<Model::GameModel>(window_size);
    // set model
    game_view_model->setModel(game_model);
    // properties
    game_view->setTotalScore(game_view_model->getTotalScore());
    game_view->setGameTime(game_view_model->getGameTime());
    game_view->setPlatformInfo(game_view_model->getPlatformInfo());
    game_view->setFrameInfo(game_view_model->getFrameInfo());
    // commands
    game_view->setPlayerLeftCommand(game_view_model->getPlayerLeftCommand());
    game_view->setPlayerRightCommand(game_view_model->getPlayerRightCommand());
    game_view->setPlayerJumpCommand(game_view_model->getPlayerJumpCommand());
    game_view->setPlayerDownCommand(game_view_model->getPlayerDownCommand());
    game_view->setPlayerStopLeftCommand(game_view_model->getPlayerStopLeftCommand());
    game_view->setPlayerStopRightCommand(game_view_model->getPlayerStopRightCommand());
    game_view->setPlayerStopJumpCommand(game_view_model->getPlayerStopJumpCommand());
    game_view->setPlayerStopDownCommand(game_view_model->getPlayerStopDownCommand());
    game_view->setUpdateCommand(game_view_model->getUpdateCommand());
    game_view->setGameOverCommand(&gameover_command);
    
    // commands 技能命令绑定
    game_view->setPlayerSkillCommand(game_view_model->getPlayerSkillCommand());
    
    // notification
    game_view_model->getTrigger().add(
        game_view->getNotificationCallback(),
        game_view.get()
    );
    
    game_view->init();
}

void GameApp::initScore(void* info) {
    ScoreInfo* score_info = static_cast<ScoreInfo*>(info);
    score_view = std::make_shared<View::ScoreView>(
        game_title, window_size, fps, window
    );
    score_view_model = std::make_shared<ViewModel::ScoreViewModel>(window_size);
    score_model = std::make_shared<Model::ScoreModel>(window_size, score_info->total_score, score_info->game_time);
    // set model
    score_view_model->setModel(score_model);
    // properties
    score_view->setCurrentSelection(score_view_model->getCurrentSelectionIndex());
    score_view->setMenuOptions(score_view_model->getMenuOptions());
    score_view->setTotalScore(score_view_model->getTotalScore());
    score_view->setGameTime(score_view_model->getGameTime());
    // commands
    score_view->setNavigateUpCommand(score_view_model->getNavigateUpCommand());
    score_view->setNavigateDownCommand(score_view_model->getNavigateDownCommand());
    score_view->setUpdateCommand(score_view_model->getUpdateCommand());
    score_view->setConfirmSelectionCommand(&change_page_command);
    // notification
    score_view_model->getTrigger().add(
        score_view->getNotificationCallback(),
        score_view.get()
    );

    score_view->init();
}