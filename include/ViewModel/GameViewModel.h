#pragma once

#include "Common/CommandBase.h"
#include "Common/NotificationBase.h"
#include "Common/FrameInfo.h"
#include "Common/SkillID.h"

#include "ViewModel/ViewModel.h"
#include "Model/GameModel.h"
#include <functional>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>


namespace ViewModel {

class GameViewModel : public ViewModel {
public:
    enum class towards {
        LEFT,
        RIGHT
    };
    enum class PlayerState {
        IDLE_L,
        IDLE_R,
        WALKING_L,
        WALKING_R,
        JUMPING_L,
        JUMPING_R,
        SPRINTING_L,
        SPRINTING_R
    };
    enum class PlatformType {
        NORMAL
    };
    GameViewModel(sf::Vector2u windowSize);
    virtual ~GameViewModel() = default;

    void setModel(std::shared_ptr<Model::GameModel> model) {
        this->model = model;
        this->model->getTrigger().add(&notification_callback, this);
    }

    // properties
    std::string* getTotalScore() {
        return &total_score;
    }
    std::string* getGameTime() {
        return &game_time;
    }
    std::string* getPlatformInfo() {
        return &platform_info;
    }
    Common::FrameInfo* getFrameInfo() {
        return &frame_info;
    }

    // commands
    Common::CommandBase* getPlayerLeftCommand() {
        return &playerLeft_command;
    }
    Common::CommandBase* getPlayerRightCommand() {
        return &playerRight_command;
    }
    Common::CommandBase* getPlayerJumpCommand() {
        return &playerJump_command;
    }
    Common::CommandBase* getPlayerDownCommand() {
        return &playerDown_command;
    }
    Common::CommandBase* getPlayerStopLeftCommand() {
        return &playerStopLeft_command;
    }
    Common::CommandBase* getPlayerStopRightCommand() {
        return &playerStopRight_command;
    }
    Common::CommandBase* getPlayerStopJumpCommand() {
        return &playerStopJump_command;
    }
    Common::CommandBase* getPlayerStopDownCommand() {
        return &playerStopDown_command;
    }
    Common::CommandBase* getUpdateCommand() {
        return &update_command;
    }
    
    // 技能命令
    Common::CommandBase* getPlayerSkillCommand() {
        return &playerSkill_command;
    }

    void playerJump();
    void playerDown();
    void playerWalkLeft();
    void playerWalkRight();
    void playerStopLeft();
    void playerStopRight();
    void playerStopJump();
    void playerStopDown();

    void forwarding();
    // 技能方法
    void playerUseSkill(Common::SkillID skill_id);

private:
    // model
    std::shared_ptr<Model::GameModel> model;
    // properties
    Common::FrameInfo frame_info;
    std::string total_score;
    std::string game_time;
    std::string platform_info;
    // notification
    static void notification_callback(Common::NotificationId id, void* viewmodel);
    // others
    towards player_towards = towards::RIGHT;
    std::map<sf::Keyboard::Key, bool> key_state;
    void init_keystate() {
        key_state[sf::Keyboard::W] = false;
        key_state[sf::Keyboard::A] = false;
        key_state[sf::Keyboard::S] = false;
        key_state[sf::Keyboard::D] = false;
    }
    std::map<PlayerState, sf::Texture> player_textures;
    void loadPlayerTextures();
    sf::Texture* getPlayerTexture(Model::Entities::PlayerState state);
    void updateTotalScoreText();
    void updateGameTimeText();
    void updatePlatformInfoText() {
        platform_info = model->getPlatformInfo();
    }
    void updatePlatformsInfo(); // 更新FrameInfo中的平台信息

// commands
public:
    class PlayerLeftCommand : public Common::CommandBase {
    public:
        PlayerLeftCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->playerWalkLeft();
        }
    private:
        GameViewModel* view_model;
    };
    class PlayerRightCommand : public Common::CommandBase {
    public:
        PlayerRightCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->playerWalkRight();
        }
    private:
        GameViewModel* view_model;
    };
    class PlayerJumpCommand : public Common::CommandBase {
    public:
        PlayerJumpCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->playerJump();
        }
    private:
        GameViewModel* view_model;
    };
    class PlayerDownCommand : public Common::CommandBase {
    public:
        PlayerDownCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->playerDown();
        }
    private:
        GameViewModel* view_model;
    };
    class PlayerStopLeftCommand : public Common::CommandBase {
    public:
        PlayerStopLeftCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->playerStopLeft();
        }
    private:
        GameViewModel* view_model;
    };
    class PlayerStopRightCommand : public Common::CommandBase {
    public:
        PlayerStopRightCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->playerStopRight();
        }
    private:
        GameViewModel* view_model;
    };
    class PlayerStopJumpCommand : public Common::CommandBase {
    public:
        PlayerStopJumpCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->playerStopJump();
        }
    private:
        GameViewModel* view_model;
    };
    class PlayerStopDownCommand : public Common::CommandBase {
    public:
        PlayerStopDownCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            view_model->playerStopDown();
        }
    private:
        GameViewModel* view_model;
    };

    class UpdateCommand : public Common::CommandBase {
    public:
        UpdateCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute(Common::CommandParam& delta_time) override ;
    private:
        GameViewModel* view_model;
    };
    
    // 技能命令类
    class PlayerSkillCommand : public Common::CommandBase {
    public:
        PlayerSkillCommand(GameViewModel* view_model) : view_model(view_model) {}
        void execute() override {
            // 默认使用技能0 (箭矢射击)，向右发射
            view_model->playerUseSkill(Common::SkillID::ARROW_SHOT);
        }
        void execute(Common::CommandParam& params) override {
            auto& skill_param = dynamic_cast<Common::PlayerSkillParam&>(params);
            view_model->playerUseSkill(static_cast<Common::SkillID>(skill_param.value.skill_id));
        }
    private:
        GameViewModel* view_model;
    };

private:
    PlayerLeftCommand playerLeft_command;
    PlayerRightCommand playerRight_command;
    PlayerJumpCommand playerJump_command;
    PlayerDownCommand playerDown_command;
    PlayerStopLeftCommand playerStopLeft_command;
    PlayerStopRightCommand playerStopRight_command;
    PlayerStopJumpCommand playerStopJump_command;
    PlayerStopDownCommand playerStopDown_command;
    UpdateCommand update_command;
    
    // 技能命令实例
    PlayerSkillCommand playerSkill_command;
};

}