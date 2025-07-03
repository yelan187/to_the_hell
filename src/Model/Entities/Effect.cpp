#include "Model/Entities/Effect.h"
#include "Model/GameModel.h"

using Model::Entities::JumpStrength;
using Model::GameModel;

void JumpStrength::apply() {
    game_model->getPlayer()->addMaxJumpCount(1);
}
