#include "StorageESP.h"
#include "../../../../Utils/RenderUtils.h"
#include <chrono>

StorageESP::StorageESP() : Module("StorageESP", "ESP for storage blocks.", Category::RENDER),
lastUpdateTime(std::chrono::steady_clock::now()) {
    addSlider<float>("Opacity", "NULL", ValueType::FLOAT_T, &opacity, 0.1f, 255.f);
    addSlider<int>("Radius", "NULL", ValueType::INT_T, &radius, 1, 200);
    addSlider<int>("time", "NULL", ValueType::INT_T, &time, 1, 10000);
}

void StorageESP::getBlocks(std::vector<Vec3<int>>& blocks) {
    std::lock_guard<std::mutex> guard(listLock);
    blocks.clear();
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;

    int playerChunkX = static_cast<int>(floor(localPlayer->getPosition()->x)) >> 4;
    int playerChunkZ = static_cast<int>(floor(localPlayer->getPosition()->z)) >> 4;
    int playerY = static_cast<int>(localPlayer->getPosition()->y);

    int chunkRadius = (radius + 15) >> 4;

    for (int cx = -chunkRadius; cx <= chunkRadius; cx++) {
        for (int cz = -chunkRadius; cz <= chunkRadius; cz++) {

            int baseX = (playerChunkX + cx) << 4;
            int baseZ = (playerChunkZ + cz) << 4;

            for (int x = 0; x < 16; x++) {
                for (int y = std::max(0, playerY - radius);
                    y < std::min(256, playerY + radius); y++) {
                    for (int z = 0; z < 16; z++) {
                        Vec3<int> blockPos(baseX + x, y, baseZ + z);

                        // Verificar distancia al jugador
                        float dx = blockPos.x - localPlayer->getPosition()->x;
                        float dy = blockPos.y - localPlayer->getPosition()->y;
                        float dz = blockPos.z - localPlayer->getPosition()->z;
                        if (dx * dx + dy * dy + dz * dz > radius * radius) continue;

                        Block* block = localPlayer->dimension->blockSource->getBlock(blockPos);
                        if (!block) continue;

                        if (block->blockLegacy->blockId == 54 ||
                            block->blockLegacy->blockId == 146 ||
                            block->blockLegacy->blockId == 130 ||
                            block->blockLegacy->blockId == 458 ||
                            block->blockLegacy->blockId == 205 ||
                            block->blockLegacy->blockId == 218 ||
                            block->blockLegacy->blockId == 154 ||
                            block->blockLegacy->blockId == 342 ||
                            block->blockLegacy->blockId == 199 ||
                            block->blockLegacy->blockId == 219 ||
                            block->blockLegacy->blockId == 220 ||
                            block->blockLegacy->blockId == 221 ||
                            block->blockLegacy->blockId == 222 ||
                            block->blockLegacy->blockId == 223 ||
                            block->blockLegacy->blockId == 224 ||
                            block->blockLegacy->blockId == 225 ||
                            block->blockLegacy->blockId == 226 ||
                            block->blockLegacy->blockId == 227 ||
                            block->blockLegacy->blockId == 228 ||
                            block->blockLegacy->blockId == 229 ||
                            block->blockLegacy->blockId == 230 ||
                            block->blockLegacy->blockId == 231 ||
                            block->blockLegacy->blockId == 232 ||
                            block->blockLegacy->blockId == 233 ||
                            block->blockLegacy->blockId == 389 ||
                            block->blockLegacy->blockId == 234) {
                            blocks.push_back(blockPos);
                        }
                    }
                }
            }
        }
    }
}

void StorageESP::onRender(MinecraftUIRenderContext* renderCtx) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;
    if (!mc.canUseMoveKeys()) return;
    Level* level = localPlayer->getLevel();
    if (level == nullptr) return;

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();

    if (duration > time) { // Actualizar cada segundo
        getBlocks(blocks);
        lastUpdateTime = now;
    }

    if (blocks.empty()) return;

    for (const Vec3<int>& blockPos : blocks) {
        Block* block = mc.getLocalPlayer()->dimension->blockSource->getBlock(blockPos);
        AABB aabb = mc.getLocalPlayer()->makeAABB(&blockPos);
        UIColor flushColor = UIColor();

        if (block->blockLegacy->blockId == 54) flushColor = UIColor(73.f, 125.f, 98.f, opacity);
        if (block->blockLegacy->blockId == 146) flushColor = UIColor(87.f, 35.f, 100.f, opacity);
        if (block->blockLegacy->blockId == 130) flushColor = UIColor(255.f, 1.f, 1.f, opacity);
        if (block->blockLegacy->blockId == 458) flushColor = UIColor(204.f, 255.f, 0.04f, opacity);
        if (block->blockLegacy->blockId == 205) flushColor = UIColor(255.f, 203.f, 219.f, opacity);
        if (block->blockLegacy->blockId == 218) flushColor = UIColor(255.f, 203.f, 219.f, opacity);
        if (block->blockLegacy->blockId == 154) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 342) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 199) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 219) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 220) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 221) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 222) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 223) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 224) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 225) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 226) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 227) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 228) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 229) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 230) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 231) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 232) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 233) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 234) flushColor = UIColor(1.f, 5.f, 255.f, opacity);
        if (block->blockLegacy->blockId == 389) flushColor = UIColor(1.f, 5.f, 255.f, opacity);

        RenderUtils::setColor(flushColor.r, flushColor.g, flushColor.b, flushColor.a);
        Vec3<float> blockPosFloat(
            static_cast<float>(blockPos.x),
            static_cast<float>(blockPos.y),
            static_cast<float>(blockPos.z)
        );
        RenderUtils::drawBox(blockPosFloat, flushColor, flushColor, .3f, false, true);
    }
}
