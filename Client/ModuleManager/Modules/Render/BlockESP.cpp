#include "BlockESP.h"
#include "../../../../Utils/RenderUtils.h"
#include <chrono>

BlockESP::BlockESP() : Module("BlockESP", "ESP for but blocks.", Category::RENDER),
lastUpdateTime(std::chrono::steady_clock::now()) {
	addBoolCheck("Diamond Ore", "Highlight diamond ore blocks.", &diamondToggle);
	addBoolCheck("Emerald Ore", "Highlight emerald ore blocks.", &emeraldToggle);
	addBoolCheck("Redstone Ore", "Highlight redstone ore blocks.", &redstoneToggle);
	addBoolCheck("Iron Ore", "Highlight iron ore blocks.", &ironToggle);
	addBoolCheck("Gold Ore", "Highlight gold ore blocks.", &goldToggle);
	addBoolCheck("Coal Ore", "Highlight coal ore blocks.", &coalToggle);
	addBoolCheck("Nether Wart", "Highlight nether wart blocks.", &netherWartToggle);
	addSlider<int>("Radius", "NULL", ValueType::INT_T, &radius, 1, 120);
	addSlider<int>("BlockID", "NULL", ValueType::INT_T, &customid, 1, 252);
	addSlider<int>("Update Time", "NULL", ValueType::INT_T, &time2, 1, 10000);
	addColorPicker("Block Color", "NULL", &colsex);
}

void BlockESP::getBlocks(std::vector<Vec3<int>>& blocks) {
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

                        if ((diamondToggle && block->blockLegacy->blockId == 56) ||    // Diamond ore
                            (emeraldToggle && block->blockLegacy->blockId == 129) ||   // Emerald ore
                            (redstoneToggle && (block->blockLegacy->blockId == 73 || block->blockLegacy->blockId == 74)) ||   // Redstone ore
                            (ironToggle && block->blockLegacy->blockId == 15) ||       // Iron ore
                            (goldToggle && block->blockLegacy->blockId == 14) ||       // Gold ore
                            (coalToggle && block->blockLegacy->blockId == 16) ||       // Coal ore
                            (netherWartToggle && block->blockLegacy->blockId == 115) || // Nether wart
                            (customToggle && block->blockLegacy->blockId == customid)) { // Custom block
                            blocks.push_back(blockPos);
                            
                        }
                    }
                }
            }
        }
    }
}

void BlockESP::onRender(MinecraftUIRenderContext* renderCtx) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;
    if (!mc.canUseMoveKeys()) return;
    Level* level = localPlayer->getLevel();
    if (level == nullptr) return;

    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdateTime).count();

    if (duration > time2) { // Actualizar cada segundo
        getBlocks(blocks);
        lastUpdateTime = now;
    }

    if (blocks.empty()) return;

    for (const Vec3<int>& blockPos : blocks) {
        Block* block = mc.getLocalPlayer()->dimension->blockSource->getBlock(blockPos);
        AABB aabb = mc.getLocalPlayer()->makeAABB(&blockPos);
        UIColor flushColor = UIColor();

        if (block->blockLegacy->blockId == 56) flushColor = UIColor(0, 0, 255, 255);         // Diamond
        if (block->blockLegacy->blockId == 129) flushColor = UIColor(0, 255, 0, 255);  // Emerald
        if (block->blockLegacy->blockId == 73 || block->blockLegacy->blockId == 74) flushColor = UIColor(255, 0, 0, 255);   // Redstone
        if (block->blockLegacy->blockId == 15) flushColor = UIColor(169, 169, 169, 255); // Iron
        if (block->blockLegacy->blockId == 14) flushColor = UIColor(255, 215, 0, 255); // Gold
        if (block->blockLegacy->blockId == 115) flushColor = UIColor(138, 43, 226, 255); // Nether Wart
        if (block->blockLegacy->blockId == 16) flushColor = UIColor(0, 0, 0, 255); // Coal Ore
//if (block->blockLegacy->blockId == customid) flushColor = UIColor(75, 45, 10, 100); // Coal Ore

        RenderUtils::setColor(flushColor.r, flushColor.g, flushColor.b, flushColor.a);
        Vec3<float> blockPosFloat(
            static_cast<float>(blockPos.x),
            static_cast<float>(blockPos.y),
            static_cast<float>(blockPos.z)
        );
        RenderUtils::drawBox(blockPosFloat, flushColor, flushColor, .3f, false, true);
    }
}
