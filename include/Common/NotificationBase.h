#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>

namespace Common {

enum class NotificationId {
    // MainMenu
    ChangeCurrentSelection,      
    ChangeBackgroundParticles,   
    
    // Game
    ChangeGameFrame,                    
    GameOver                    
};

// callback function type for notifications
typedef void (* NotificationFunc)(NotificationId id, void* p);

}
