#pragma once
#include "../Module.h"
#include "C:\Users\Admin\Desktop\nhackmcbe-master2\nhackmcbe-master\Boost\SDK\Classes\MinecraftUIRenderContext.h"
#include <mutex>
#include <vector>
#include <chrono>

class StorageESP : public Module {
public:
    StorageESP();
    void onRender(MinecraftUIRenderContext* renderCtx) override;
    void getBlocks(std::vector<Vec3<int>>& blocks);

private:
    std::chrono::steady_clock::time_point lastUpdateTime;
    std::vector<Vec3<int>> blocks;
    std::mutex listLock;
    float opacity = 255.f;
    int radius = 50;
    int time = 1000;
    bool line;
    bool tracerEnabled;
};