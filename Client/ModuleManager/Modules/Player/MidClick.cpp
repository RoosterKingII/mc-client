#include "MidClick.h"
#include "PacketMine.h"

Vec3<float> getLookDirection(const Vec2<float>& rotation) {
    float yaw = rotation.y * (M_PI / 180.0f);
    float pitch = rotation.x * (M_PI / 180.0f);
    float x = -cos(pitch) * sin(yaw);
    float y = -sin(pitch);
    float z = cos(pitch) * cos(yaw);
    return Vec3<float>(x, y, z);
}

bool isWithinFieldOfView(const Vec3<float>& lookDir, const Vec3<float>& toTarget, float fov) {
    float dotProduct = lookDir.dot(toTarget.normalized());
    float angle = acos(dotProduct) * (180.0f / M_PI);
    return angle <= fov / 2.0f;
}

void MidClick::onNormalTick(Actor* actor) {
    if (mc.isMiddleClickDown() && !hasClicked) {
        auto player = mc.getLocalPlayer();
        auto level = player->getLevel();
        auto list = level->getRuntimeActorList();
        hasClicked = true;

        Vec3<float> eyePos = player->getEyePos();
        Vec2<float> rotation = *player->getRotation();
        Vec3<float> lookDir = getLookDirection(rotation);

        float maxDistance = 100.0f;
        float fieldOfView = 10.0f; // Narrow field of view
        Actor* closestActor = nullptr;
        float closestDistance = maxDistance;
        std::string closestName = "";

        for (auto& otherActor : list) {
            if (otherActor == player) continue;

            AABB* aabb = otherActor->getAABB();
            if (!aabb) continue;

            Vec3<float> toTarget = otherActor->getEyePos() - eyePos;
            float distance = toTarget.length();

            if (distance < closestDistance && isWithinFieldOfView(lookDir, toTarget, fieldOfView)) {
                closestActor = otherActor;
                closestDistance = distance;
                closestName = closestActor->getNameTag()->c_str();
            }
        }

        if (closestActor) {
            if (std::find(TargetUtils::Friend.begin(), TargetUtils::Friend.end(), closestName) == TargetUtils::Friend.end()) {
                TargetUtils::Friend.push_back(closestName);
                mc.DisplayClientMessage(std::string("[Rooster] " + closestName + " added to friends list!").c_str());
            }
            else {
                auto it = std::find(TargetUtils::Friend.begin(), TargetUtils::Friend.end(), closestName);
                if (it != TargetUtils::Friend.end()) {
                    TargetUtils::Friend.erase(it);
                    mc.DisplayClientMessage(std::string("[Rooster] " + closestName + " removed from friends list!").c_str());
                }
            }
        }
    }
    else if (!mc.isMiddleClickDown()) {
        hasClicked = false;
    }
}