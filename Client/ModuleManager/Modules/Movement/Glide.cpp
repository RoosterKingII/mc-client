#include "Glide.h"

Glide::Glide() : Module("Glide", "go down.", Category::MOVEMENT) {
    addSlider<float>("Value", "NULL", ValueType::FLOAT_T, &this->Value, -0.20f, 0.f);
}

void Glide::onNormalTick(Actor* actor) {
    if (mc.getLocalPlayer() == nullptr) return;

    mc.getLocalPlayer()->stateVectorComponent->velocity.y = Value;
}
