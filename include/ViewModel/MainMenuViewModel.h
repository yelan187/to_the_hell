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
    
    void setModel(std::shared_ptr<Model::MainMenuModel> model) {
        this->model = model;
    }

    std::vector<sf::Vector2f> getBackgroundParticles() const;
    std::vector<std::string> getMenuOptions() const;
    Common::CommandBase* getNavigateUpCommand() {
        return &navigateUp_command;
    }
    Common::CommandBase* getNavigateDownCommand() {
        return &navigateDown_command;
    }
    Common::CommandBase* getUpdateCommand() {
        return &update_command;
    }
    Common::Trigger& getTrigger() {
        return trigger;
    }
    int getCurrentSelectionIndex() const {
        return current_selection_index;
    }

    // notification
    Common::NotificationFunc getNotificationCallback() {
        return &notification_callback;
    }
private:
    void navigateUp();
    void navigateDown();
    void updateAnimationState(float delta_time);
    void updateAvailableOptions();
    // notification
	static void notification_callback(Common::NotificationParam* param, void* view_model) {};

private:
    // model
    std::shared_ptr<Model::MainMenuModel> model;

    // props
	float animation_time;

    std::vector<sf::Vector2f> background_particles;

    std::vector<MenuOption> available_options;
    int current_selection_index;
public:
    class NavigateUpCommand : public Common::CommandBase {
    public:
        NavigateUpCommand(MainMenuViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->navigateUp();
            Common::ChangeCurrentSelectionParam* param = new Common::ChangeCurrentSelectionParam();
            param->id = Common::NotificationId::ChangeCurrentSelection;
            param->value = view_model->getCurrentSelectionIndex();
            view_model->trigger.fire(param);
        }
    private:
        MainMenuViewModel* view_model;
    };

    class NavigateDownCommand : public Common::CommandBase {
    public:
        NavigateDownCommand(MainMenuViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->navigateDown();
            Common::ChangeCurrentSelectionParam* param = new Common::ChangeCurrentSelectionParam();
            param->id = Common::NotificationId::ChangeCurrentSelection;
            param->value = view_model->getCurrentSelectionIndex();
            view_model->trigger.fire(param);
        }
    private:
        MainMenuViewModel* view_model;
    };

    class UpdateCommand : public Common::CommandBase {
    public:
        UpdateCommand(MainMenuViewModel* view_model) : view_model(view_model) {}
        void execute(Common::CommandParam& delta_time) override;
    private:
        MainMenuViewModel* view_model;
    };

private:
    NavigateUpCommand navigateUp_command;
    NavigateDownCommand navigateDown_command;
    UpdateCommand update_command;
};
}