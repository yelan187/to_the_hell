#include "App/GameApp.h"

using App::GameApp;

// public methods
GameApp::GameApp(std::string game_title, sf::Vector2u window_size, int fps) 
    : game_title(std::move(game_title)), window_size(window_size), fps(fps),
      change_page_command(this) {
    window.create(sf::VideoMode(window_size.x, window_size.y), game_title, sf::Style::Default);
    window.setFramerateLimit(fps);
    changePage(View::PAGE_STATE::MAIN_MENU);
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

void GameApp::changePage(View::PAGE_STATE new_page_state, bool init) {
    // update the page
    current_page_state = new_page_state;
    switch (new_page_state) {
        case View::PAGE_STATE::GAME:
            // change to game view
            break;
        case View::PAGE_STATE::EXIT:
            page->exit();
            break;
        case View::PAGE_STATE::SCORE:
            // change to score view
            break;
        case View::PAGE_STATE::MAIN_MENU:
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

// private methods
void GameApp::initMainMenu() {
    mainmenu_view = std::make_shared<View::MainMenuView>(
        game_title, window_size, fps, window
    );
    mainmenu_view_model = std::make_shared<ViewModel::MainMenuViewModel>(
        window_size
    );
    // properties
    mainmenu_view->setCurrentSelection(mainmenu_view_model->getCurrentSelectionIndex());
    mainmenu_view->setMenuOptions(mainmenu_view_model->getMenuOptions());
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