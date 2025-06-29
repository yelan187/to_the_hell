// #include <iostream>
// #include <View/UI/Player.h>
// #include <ViewModel/GameViewModel.h>

// using View::UI::Player;

// void Player::init(std::shared_ptr<ViewModel::GameViewModel> view_model) {
//     loadTextures();
//     this->view_model = view_model;
//     size = sf::Vector2f(60,60);
//     rect.setSize(size);
// }

// void Player::update(float delta_time) {
//     setTexture(view_model->getPlayerState());
//     rect.setPosition(view_model->getPlayerPosition(size));
// }

// void Player::render(sf::RenderWindow& window){
//     window.draw(rect);
// }

// void Player::loadTextures() {
//     // std::cout << "Loading player textures..." << std::endl;
//     // Load textures for different player states
//     try {
//         sf::Texture idle_l_texture;
//         idle_l_texture.loadFromFile("assets/images/player/player_idle_l.png");
//         texture_map[PlayerState::IDLE_L] = idle_l_texture;
//     } catch (const std::exception& e) {
//         std::cout << "Error loading player textures: " << e.what() << std::endl;
//     }
//     try {
//         sf::Texture idle_r_texture;
//         idle_r_texture.loadFromFile("assets/images/player/player_idle_r.png");
//         texture_map[PlayerState::IDLE_R] = idle_r_texture;
//     } catch (const std::exception& e) {
//         std::cout << "Error loading player textures: " << e.what() << std::endl;
//     }
//     try {
//         sf::Texture walk_l_texture;
//         walk_l_texture.loadFromFile("assets/images/player/player_walk_l.png");
//         texture_map[PlayerState::WALKING_L] = walk_l_texture;
//     } catch (const std::exception& e) {
//         std::cout << "Error loading player textures: " << e.what() << std::endl;
//     }
//     try {
//         sf::Texture walk_r_texture;
//         walk_r_texture.loadFromFile("assets/images/player/player_walk_r.png");
//         texture_map[PlayerState::WALKING_R] = walk_r_texture;
//     } catch (const std::exception& e) {
//         std::cout << "Error loading player textures: " << e.what() << std::endl;
//     }
//     try {
//         sf::Texture jump_l_texture;
//         jump_l_texture.loadFromFile("assets/images/player/player_jump_l.png");
//         texture_map[PlayerState::JUMPING_L] = jump_l_texture;
//     } catch (const std::exception& e) {
//         std::cout << "Error loading player textures: " << e.what() << std::endl;
//     }
//     try {
//         sf::Texture jump_r_texture;
//         jump_r_texture.loadFromFile("assets/images/player/player_jump_r.png");
//         texture_map[PlayerState::JUMPING_R] = jump_r_texture;
//     } catch (const std::exception& e) {
//         std::cout << "Error loading player textures: " << e.what() << std::endl;
//     }
// }

// void Player::setTexture(PlayerState state) {
//     switch (state) {
//         case PlayerState::IDLE_L:
//             rect.setTexture(&texture_map[PlayerState::IDLE_L]);
//             break;
//         case PlayerState::IDLE_R:
//             rect.setTexture(&texture_map[PlayerState::IDLE_R]);
//             break;
//         case PlayerState::WALKING_L:
//             rect.setTexture(&texture_map[PlayerState::WALKING_L]);
//             break;
//         case PlayerState::WALKING_R:
//             rect.setTexture(&texture_map[PlayerState::WALKING_R]);
//             break;
//         case PlayerState::JUMPING_L:
//             rect.setTexture(&texture_map[PlayerState::JUMPING_L]);
//             break;
//         case PlayerState::JUMPING_R:
//             rect.setTexture(&texture_map[PlayerState::JUMPING_R]);
//             break;
//         default:
//             break;
//     }
// }