#include "Model/Entities/Effect.h"
#include "Model/GameModel.h"

using Model::Entities::JumpStrength;
using Model::Entities::SpeedBoost;
using Model::Entities::HealthBoost;
using Model::GameModel;

void JumpStrength::apply() {
    game_model->getPlayer()->addMaxJumpCount(1);
}

void SpeedBoost::apply() {
    // 增加玩家移动速度
    game_model->getPlayer()->addWalkSpeed(0.3f);
}

void HealthBoost::apply() {
    // 先增加最大生命值，再恢复生命值
    game_model->getPlayer()->addMaxHP(2);
    game_model->getPlayer()->heal(2);
}
