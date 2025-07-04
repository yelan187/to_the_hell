#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <chrono>
#include "Common/SkillID.h"

namespace Common {
// accessible for view/viewmodel layers to bind properties
typedef struct choiceInfo{
    std::string name;
    std::string description;
    std::string icon_path;  // 图标图片路径
} ChoiceInfo;
typedef std::vector<ChoiceInfo> ChoicesInfo;
}