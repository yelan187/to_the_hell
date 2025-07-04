#pragma once

#include "Common/CommandBase.h"
#include "Common/NotificationBase.h"

#include "ViewModel/ViewModel.h"
#include "Model/ScoreModel.h"
#include <functional>
#include <vector>
#include <string>

namespace ViewModel {

class ScoreViewModel : public ViewModel {
public:
    ScoreViewModel(sf::Vector2u window_size);  
    // model  
    void setModel(std::shared_ptr<Model::ScoreModel> model) {
        this->model = model;
    }
    // properties
    std::vector<std::string>* getMenuOptions() {
        return &menu_options;
    }
    int* getCurrentSelectionIndex() {
        return &current_selection_index;
    }
    std::string* getTotalScore() const {
        return model->getScore();
    }
    std::string* getGameTime() const {
        return model->getTime();
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
    Common::CommandBase* getConfirmSelectionCommand() {
        return &confirmSelection_command;
    }
    // notification
    Common::NotificationFunc getNotificationCallback() {
        return &notification_callback;
    }

private:
    // model
    std::shared_ptr<Model::ScoreModel> model;
    // notification
    static void notification_callback(Common::NotificationId id, void* view_model) {}
    // others
    int current_selection_index;
    std::vector<std::string> menu_options;
    void navigateUp();
    void navigateDown();
    void confirmSelection();

// commands
public:
    class NavigateUpCommand : public Common::CommandBase {
    public:
        NavigateUpCommand(ScoreViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->navigateUp();
            view_model->trigger.fire(Common::NotificationId::ChangeCurrentSelection);
        }
    private:
        ScoreViewModel* view_model;
    };

    class NavigateDownCommand : public Common::CommandBase {
    public:
        NavigateDownCommand(ScoreViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->navigateDown();
            view_model->trigger.fire(Common::NotificationId::ChangeCurrentSelection);
        }
    private:
        ScoreViewModel* view_model;
    };

    class UpdateCommand : public Common::CommandBase {
    public:
        UpdateCommand(ScoreViewModel* view_model) : view_model(view_model) {}
        void execute(Common::CommandParam& delta_time) override {}
    private:
        ScoreViewModel* view_model;
    };

    class ConfirmSelectionCommand : public Common::CommandBase {
    public:
        ConfirmSelectionCommand(ScoreViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->confirmSelection();
        }
    private:
        ScoreViewModel* view_model;
    };

private:
    NavigateUpCommand navigateUp_command;
    NavigateDownCommand navigateDown_command;
    UpdateCommand update_command;
    ConfirmSelectionCommand confirmSelection_command;
};

}
