#pragma once
#include <string>

namespace Utils {
    const std::string WINDOW_TITLE = "To The Hell";
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 600;
    constexpr int FPS = 60;
    enum class GameViewModelPlayerState {
        IDLE_L,
        IDLE_R,
        WALKING_L,
        WALKING_R,
        JUMPING_L,
        JUMPING_R
    };


    enum class PlatformType {
        NORMAL
    };
}
