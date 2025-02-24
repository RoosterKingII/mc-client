#pragma once
#include "../Module.h"
# define M_PI 3.14159265358979323846 /* pi */
#include <chrono>

class PacketMine : public Module {
private:
    bool isMining = false; // Flag to indicate if mining is in progress
    Vec2<float> GetRotations(Vec3<float> playerEyePos, Vec3<float> targetPos) {
        Vec3<float> delta = targetPos.sub(playerEyePos);
        float yaw = atan2(delta.z, delta.x) * 180.0f / M_PI;
        float pitch = atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / M_PI;
        return { -pitch, yaw - 90 };
    }
    float range = 6.f;
    bool silenSwitch = true;
    bool silentBack = true;
    bool message = false;
    int breakMode = 0;
    int breakKeybind = 0x0;
    Vec3<int> breakPos = Vec3<int>(0, 0, 0);
    uint8_t face = -1;

    std::pair<int, float> getBestPickaxeSlot(Block* block);
    int lastSlot = -1;
    int shouldSwitchBack = false;

    // New members for auto-eating
    bool isEating = false;
    int lastFoodSlot = -1;
    int eatInterval = 20; // Interval in seconds
    std::chrono::steady_clock::time_point lastEatTime;

public:
    void setIsMining(bool mining) {
        isMining = mining;
    }

    // Getter for the mining flag
    bool getIsMining() const {
        return isMining;
    }
    bool isbeingmine = false;
    PacketMine();
    void setbreakPos(const Vec3<int>& bPos, uint8_t f);
    void setBreakPos(const Vec3<int>& bPos, uint8_t f);
    void Reset();
    virtual void onSendPacket(Packet* packet, bool& shouldCancel);
    virtual void onNormalTick(Actor* actor) override;
    virtual void onRender(MinecraftUIRenderContext* ctx) override;
    virtual void onImGuiRender(ImDrawList* drawlist) override;

    // New method for auto-eating
    void autoEat(LocalPlayer* localPlayer);
};
