#pragma once
#include "../Module.h"

class ChunkESP : public Module {
private:
    std::vector<Vec3<int>> chunkPositions;
    int renderMode = 0;
    UIColor color = UIColor(0, 255, 255, 50);  // Color semi-transparente
    UIColor lineColor = UIColor(0, 255, 255, 255); // Color sólido para líneas
    float lineWidth = 0.3f;
    bool fill = true;
    bool outline = true;
    int renderDistance = 2;

public:
    ChunkESP();
    virtual void onRender(MinecraftUIRenderContext* ctx) override;
};
