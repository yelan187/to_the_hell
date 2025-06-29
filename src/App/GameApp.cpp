#include "App/GameApp.h"

using App::GameApp;

// public methods
GameApp::GameApp(std::string game_title, sf::Vector2u window_size, int fps) 
    : game_title(std::move(game_title)), window_size(window_size), fps(fps),
      change_page_command(this) {
    initMainMenu();
}

void GameApp::run() {
    page->run();
}

void GameApp::changePage(View::PAGE_STATE new_page_state, bool deferred) {
    // if not deferred, then renew the current page
    if (!deferred) {
        switch (current_page_state) {
            case View::PAGE_STATE::MAIN_MENU:
                initMainMenu();
                break;
            case View::PAGE_STATE::GAME:
                // Handle game view input if necessary
                break;
            case View::PAGE_STATE::SCORE:
                // Handle score view input if necessary
                break;
        }
    }
    // change the page state and update the page
    current_page_state = new_page_state;
    switch (new_page_state) {
        case View::PAGE_STATE::MAIN_MENU:
            page = mainmenu_view;
            break;
        case View::PAGE_STATE::GAME:
            // change to game view
            break;
        case View::PAGE_STATE::SCORE:
            // change to score view
            break;
    }
}

void GameApp::ChangePageCommand::execute(Common::CommandParam& params) {
    Common::ChangePageParam& change_page_param = dynamic_cast<Common::ChangePageParam&>(params);
    app->changePage(
        change_page_param.value.new_page_state,
        change_page_param.value.deferred
    );
}

// private methods
void GameApp::initMainMenu() {
    mainmenu_view = std::make_shared<View::MainMenuView>(
        game_title, window_size, fps
    );
    mainmenu_view_model = std::make_shared<ViewModel::MainMenuViewModel>(
        window_size
    );
    mainmenu_model = std::make_shared<Model::MainMenuModel>();
    // properties
    mainmenu_view->setCurrentSelection(mainmenu_view_model->getCurrentSelectionIndex());
    mainmenu_view->setMenuOptions(mainmenu_view_model->getMenuOptions());
    mainmenu_view_model->setModel(mainmenu_model);
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
}