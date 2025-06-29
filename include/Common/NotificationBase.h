#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Common/ParamBase.h"

namespace Common {

enum class NotificationId {
    // mainmenu
    ChangeCurrentSelection,
    ChangeBackgroundParticles
};

class NotificationParam : public Param {
public:
    NotificationId id;
    virtual ~NotificationParam() = default;
};

typedef void (* NotificationFunc)(NotificationParam*, void* p);

template <typename T>
class NotificationTypeParam : public NotificationParam {
public:
    T value;
    virtual ~NotificationTypeParam() = default;
};

// Add notification param (match with NotificationId)
typedef NotificationTypeParam<int> ChangeCurrentSelectionParam;
typedef NotificationTypeParam<std::vector<sf::Vector2f>*> ChangeBackgroundParticlesParam;

}
