#include <iostream>
#include "ViewModel/GameViewModel.h"


using ViewModel::GameViewModel;

GameViewModel::GameViewModel(sf::Vector2u window_size) : 
    ViewModel(window_size),
    model(nullptr),
    playerLeft_command(this),
    playerRight_command(this),
    playerJump_command(this),
    playerDown_command(this),
    playerStopLeft_command(this),
    playerStopRight_command(this),
    playerStopJump_command(this),
    playerStopDown_command(this),
    update_command(this)
{
    init_keystate();
    loadPlayerTextures();
}

void GameViewModel::changeGameTimeText() {
    std::chrono::seconds duration = model->getDuration();
    int seconds = duration.count() % 60;
    int minutes = (duration.count() / 60) % 60;
    int hours = duration.count() / 3600;
    std::stringstream ss;
    ss << "Time: ";
    if (hours > 0) {
        ss << std::setw(2) << std::setfill('0') << hours << "h";
    }
    if (hours > 0 || minutes > 0) {
        ss << std::setw(2) << std::setfill('0') << minutes << "m";
    } else {
        ss << "00m";
    }
    ss << std::setw(2) << std::setfill('0') << seconds << "s";
    game_time_text = ss.str();
}

void GameViewModel::changeTotalScoreText() {
    total_score_text = "Score: " + std::to_string(model->getTotalScore());
}

void GameViewModel::playerJump() {
    if (!key_state[sf::Keyboard::W]) {
        model->playerJump();
        key_state[sf::Keyboard::W] = true;
    }
}

void GameViewModel::playerDown() {
    if (!key_state[sf::Keyboard::S]) {
        model->playerDown();
        key_state[sf::Keyboard::S] = true;
    }
}

void GameViewModel::playerWalkLeft() {
    if (!key_state[sf::Keyboard::A]) {
        key_state[sf::Keyboard::A] = true;
        if (!key_state[sf::Keyboard::D]) {
            player_towards = towards::LEFT;
        }
        model->playerWalkLeft();
    }
}

void GameViewModel::playerWalkRight() {
    if (!key_state[sf::Keyboard::D]) {
        key_state[sf::Keyboard::D] = true;
        if (!key_state[sf::Keyboard::A]) {
            player_towards = towards::RIGHT;
        }
        model->playerWalkRight();
    }
}

void GameViewModel::playerStopLeft() {
    key_state[sf::Keyboard::A] = false;
    if (key_state[sf::Keyboard::D]) {
        player_towards = towards::RIGHT;
    }
    model->playerStopLeft();
}

void GameViewModel::playerStopRight() {
    key_state[sf::Keyboard::D] = false;
    if (key_state[sf::Keyboard::A]) {
        player_towards = towards::LEFT;
    }
    model->playerStopRight();
}

void GameViewModel::playerStopJump() {
    key_state[sf::Keyboard::W] = false;
}

void GameViewModel::playerStopDown() {
    key_state[sf::Keyboard::S] = false;
}

void GameViewModel::loadPlayerTextures() {
    // std::cout << "Loading player textures..." << std::endl;
    // Load textures for different player states
    try {
        sf::Texture idle_l_texture;
        idle_l_texture.loadFromFile("assets/images/player/player_idle_l.png");
        player_textures[PlayerState::IDLE_L] = idle_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture idle_r_texture;
        idle_r_texture.loadFromFile("assets/images/player/player_idle_r.png");
        player_textures[PlayerState::IDLE_R] = idle_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture walk_l_texture;
        walk_l_texture.loadFromFile("assets/images/player/player_walk_l.png");
        player_textures[PlayerState::WALKING_L] = walk_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture walk_r_texture;
        walk_r_texture.loadFromFile("assets/images/player/player_walk_r.png");
        player_textures[PlayerState::WALKING_R] = walk_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture jump_l_texture;
        jump_l_texture.loadFromFile("assets/images/player/player_jump_l.png");
        player_textures[PlayerState::JUMPING_L] = jump_l_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
    try {
        sf::Texture jump_r_texture;
        jump_r_texture.loadFromFile("assets/images/player/player_jump_r.png");
        player_textures[PlayerState::JUMPING_R] = jump_r_texture;
    } catch (const std::exception& e) {
        std::cout << "Error loading player textures: " << e.what() << std::endl;
    }
}

sf::Texture* GameViewModel::getPlayerTexture(Model::Entities::PlayerState state) {
    switch (state) {
        case Model::Entities::PlayerState::IDLE:
            if (player_towards == towards::LEFT) {
                return &player_textures[PlayerState::IDLE_L];
            } else {
                return &player_textures[PlayerState::IDLE_R];
            }
        case Model::Entities::PlayerState::WALKING:
            if (player_towards == towards::LEFT) {
                return &player_textures[PlayerState::WALKING_L];
            } else {
                return &player_textures[PlayerState::WALKING_R];
            }
        case Model::Entities::PlayerState::JUMPING_IDLE:
        case Model::Entities::PlayerState::JUMPING_WALKING:
            if (player_towards == towards::LEFT) {
                return &player_textures[PlayerState::JUMPING_L];
            } else {
                return &player_textures[PlayerState::JUMPING_R];
            }
    }
    return &player_textures[PlayerState::IDLE_R]; // Default to idle right texture
}

Common::FrameInfo::PlatformInfo GameViewModel::getPlatformInfo(Common::_FrameInfo::PlatformInfo info) {
    Common::FrameInfo::PlatformInfo platform_info;
    platform_info.position = info.position;
    switch (info.type) {
        case Model::Entities::PlatformType::NORMAL:
            platform_info.color = sf::Color::Green; // Default color for normal platforms
            platform_info.size = Utils::PLATFORM_SIZE;
            break;
    }
    return platform_info;
}

void GameViewModel::notification_callback(Common::NotificationParam* param, void* view_model) {
    if (!view_model) return;
    GameViewModel* game_view_model = static_cast<GameViewModel*>(view_model);
    switch (param->id) {
        case Common::NotificationId::_ChangeGameFrame:
            game_view_model->forwarding(dynamic_cast<Common::_ChangeGameFrameParam*>(param)->value);
            break;
        case Common::NotificationId::GameOver: 
            game_view_model->trigger.fire(param);
            break;
    }
}

void GameViewModel::forwarding(const Common::_FrameInfo& frame_info) {
    Common::ChangeGameFrameParam* change_frame_param = new Common::ChangeGameFrameParam();
    change_frame_param->id = Common::NotificationId::ChangeGameFrame;
    changeGameTimeText();
    changeTotalScoreText();
    // change_frame_param->value.total_score_text = getTotalScoreText();
    // change_frame_param->value.game_time_text = getGameTimeText();
    change_frame_param->value.debug_info_text = getDebugInfoText();
    change_frame_param->value.player_info.position = frame_info.player_info.position;
    change_frame_param->value.player_info.size = frame_info.player_info.size;
    change_frame_param->value.player_info.texture = getPlayerTexture(frame_info.player_info.state);
    std::map<int, Common::FrameInfo::PlatformInfo> platforms_info;
    for (const auto& [id, info] : frame_info.platforms_info) {
        platforms_info[id] = getPlatformInfo(info);
    }
    change_frame_param->value.platforms_info = platforms_info;
    change_frame_param->value.platforms_id = frame_info.platforms_id;
    trigger.fire(change_frame_param);
    delete change_frame_param;
}

void GameViewModel::UpdateCommand::execute(Common::CommandParam& delta_time) {
    auto& update_param = dynamic_cast<Common::UpdateParam&>(delta_time);
    view_model->model->update(update_param.value);
}