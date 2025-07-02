#pragma once

#include "Common/CommandBase.h"
#include "Common/NotificationBase.h"

#include "ViewModel/ViewModel.h"
#include "Model/MainMenuModel.h"
#include <functional>
#include <vector>
#include <string>

namespace ViewModel {
class MainMenuViewModel : public ViewModel {
public:
    enum class MenuOption {
        START_GAME,
        EXIT
    };
    MainMenuViewModel(sf::Vector2u window_size);
    virtual ~MainMenuViewModel() = default;
    // model
    void setModel(std::shared_ptr<Model::MainMenuModel> model) {
        this->model = model;
    }
    // properties
    std::vector<sf::Vector2f>* getBackgroundParticles() {
        return &background_particles;
    }
    std::vector<std::string>* getMenuOptions() {
        return &menu_options;
    }
    int* getCurrentSelectionIndex() {
        return &current_selection_index;
    }
    // commands
    Common::CommandBase* getNavigateUpCommand() {
        return &navigateUp_command;
    }
    Common::CommandBase* getNavigateDownCommand() {
        return &navigateDown_command;
    }
    Common::CommandBase* getUpdateCommand() {
        return &update_command;
    }
    // notification
    Common::NotificationFunc getNotificationCallback() {
        return &notification_callback;
    }
private:
    // model
    std::shared_ptr<Model::MainMenuModel> model;
    // properties
    std::vector<std::string> menu_options;
    std::vector<sf::Vector2f> background_particles;
    int current_selection_index;
    // notification
	static void notification_callback(Common::NotificationId id, void* view_model) {}
    // others
    float animation_time;
    void initMenuOptions();
    void initAvailableOptions();
    void navigateUp();
    void navigateDown();
    void updateAnimationState(float delta_time);

// commands
public:
    class NavigateUpCommand : public Common::CommandBase {
    public:
        NavigateUpCommand(MainMenuViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->navigateUp();
            view_model->trigger.fire(Common::NotificationId::ChangeCurrentSelection);
        }
    private:
        MainMenuViewModel* view_model;
    };

    class NavigateDownCommand : public Common::CommandBase {
    public:
        NavigateDownCommand(MainMenuViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->navigateDown();
            view_model->trigger.fire(Common::NotificationId::ChangeCurrentSelection);
        }
    private:
        MainMenuViewModel* view_model;
    };

    class UpdateCommand : public Common::CommandBase {
    public:
        UpdateCommand(MainMenuViewModel* view_model) : view_model(view_model) {}
        void execute(Common::CommandParam& delta_time) override {
            auto& update_param = dynamic_cast<Common::UpdateParam&>(delta_time);
            view_model->updateAnimationState(update_param.value);
            view_model->trigger.fire(Common::NotificationId::ChangeBackgroundParticles);
        }
    private:
        MainMenuViewModel* view_model;
    };

private:
    NavigateUpCommand navigateUp_command;
    NavigateDownCommand navigateDown_command;
    UpdateCommand update_command;
};
}