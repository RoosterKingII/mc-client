#include "Noclip.h"

NoClip::NoClip() : Module("NoClip", "NoClip", Category::MOVEMENT) {
    addSlider<float>("Horizontal Speed", "NULL", ValueType::FLOAT_T, &speed, 0.03, 10.f);
    addSlider<float>("Vertical Speed", "NULL", ValueType::FLOAT_T, &this->upanddown, 0.03, 10.f);
 //   this->addBoolCheck("Save Old POS", "NULL", &this->NoPacket);
    addSlider<float>("Glide", "How fast you want to glide.", ValueType::FLOAT_T, &glideMod, -0.2f, 0.f);
    addSlider<float>("phasevalue", ".", ValueType::FLOAT_T, &phase, 0.f, 1.f);
    addBoolCheck("SM Border", "NULL", &smborder);
    this->addBoolCheck("Enable Phase", "Enable wall phase functionality", &enablePhase); // Nueva opción
}

NoClip::~NoClip() {}

void NoClip::onNormalTick(Actor* actor) {
    auto gm = mc.getGameMode();
    gm->player->stateVectorComponent->velocity = Vec3<float>(0, 0, 0);
    glideModEffective = glideMod;

    if (mc.canUseMoveKeys()) {
        if (mc.isKeyDown(VK_SPACE))
            glideModEffective += upanddown;
        if (mc.isKeyDown(VK_SHIFT))
            glideModEffective -= upanddown;
    }

    if (smborder) {
        auto* pos = mc.getLocalPlayer()->getPosition();

        if (pos->x > 1090)
            mc.getLocalPlayer()->setPos(Vec3(pos->x = 1090, pos->y, pos->z));

        if (pos->z > 1090)
            mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y, pos->z = 1090));

        if (pos->x < 60)
            mc.getLocalPlayer()->setPos(Vec3(pos->x = 60, pos->y, pos->z));

        if (pos->z < 60)
            mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y, pos->z = 60));
        if (pos->y > 155)
            mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y = 155, pos->z));
    }

    gm->player->stateVectorComponent->velocity.y = glideModEffective;

    auto player = mc.getLocalPlayer();
    if (player == nullptr) return;

    bool w = mc.isKeyDown('W');
    bool a = mc.isKeyDown('A');
    bool s = mc.isKeyDown('S');
    bool d = mc.isKeyDown('D');

    float yaw = player->rotationComponent->Get().y;

    bool pressed = w || a || s || d;

    if (w && s)
        return;

    if (d) {
        yaw += 90.f;
        if (w)
            yaw -= 45.f;
        else if (s)
            yaw += 45.f;
    }
    if (a) {
        yaw -= 90.f;
        if (w)
            yaw += 45.f;
        else if (s)
            yaw -= 45.f;
    }

    if (s && !a && !d)
        yaw += 180.f;

    float calcYaw = (yaw + 90) * (PI / 180);
    Vec3<float> moveVec;
    moveVec.x = cos(calcYaw) * speed;
    moveVec.y = player->stateVectorComponent->velocity.y;
    moveVec.z = sin(calcYaw) * speed;
    if (pressed) player->lerpMotion(moveVec);

    // Nueva implementación segura de Phase
    if (enablePhase && mc.getLocalPlayer() != nullptr) {
        auto aabb = mc.getLocalPlayer()->getAABB();
        float reduction = phase; 
        if (aabb->upper.y - aabb->lower.y > 0.1f) {
            aabb->lower.x += reduction; 
            aabb->lower.y += reduction; 
            aabb->lower.z += reduction; 

            aabb->upper.x -= reduction; 
            aabb->upper.y -= reduction; 
            aabb->upper.z -= reduction; 
        }
    }
}
void NoClip::onEnable() {}
void NoClip::onDisable() {
    if (mc.getLocalPlayer() == nullptr) return;
    auto aabb = mc.getLocalPlayer()->getAABB();
    aabb->lower.x = mc.getLocalPlayer()->getPosition()->x - 0.3f;
    aabb->lower.y = mc.getLocalPlayer()->getPosition()->y;
    aabb->lower.z = mc.getLocalPlayer()->getPosition()->z - 0.3f;

    aabb->upper.x = mc.getLocalPlayer()->getPosition()->x + 0.3f;
    aabb->upper.y = mc.getLocalPlayer()->getPosition()->y + 1.6f;
    aabb->upper.z = mc.getLocalPlayer()->getPosition()->z + 0.3f;
}