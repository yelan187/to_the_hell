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
    void setModel(std::shared_ptr<Model::ScoreModel> model) {
        this->model = model;
    }
    
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
    int getTotalScore() const {
        return model->getScore();
    }
    std::chrono::seconds getGameTime() const {
        return model->getTime();
    }
    // notification
    Common::NotificationFunc getNotificationCallback() {
        return &notification_callback;
    }
    int getScore() const;
    std::chrono::seconds getTime() const;

private:
    void navigateUp();
    void navigateDown();
    static void notification_callback(Common::NotificationParam* param, void* view_model) {};
private:
    std::shared_ptr<Model::ScoreModel> model;
    int current_selection_index;
    std::vector<std::string> menu_options;
public:
    class NavigateUpCommand : public Common::CommandBase {
    public:
        NavigateUpCommand(ScoreViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->navigateUp();
            Common::ChangeCurrentSelectionParam* param = new Common::ChangeCurrentSelectionParam();
            param->id = Common::NotificationId::ChangeCurrentSelection;
            param->value = view_model->getCurrentSelectionIndex();
            view_model->trigger.fire(param);
            delete param;
        }
    private:
        ScoreViewModel* view_model;
    };

    class NavigateDownCommand : public Common::CommandBase {
    public:
        NavigateDownCommand(ScoreViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->navigateDown();
            Common::ChangeCurrentSelectionParam* param = new Common::ChangeCurrentSelectionParam();
            param->id = Common::NotificationId::ChangeCurrentSelection;
            param->value = view_model->getCurrentSelectionIndex();
            view_model->trigger.fire(param);
            delete param;
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

private:
    NavigateUpCommand navigateUp_command;
    NavigateDownCommand navigateDown_command;
    UpdateCommand update_command;
};

}
