#include "StorageESP.h"
#include "../../../../Utils/RenderUtils.h"

StorageESP::StorageESP() : Module("StorageESP", "ESP for but storage blocks.", Category::RENDER) {
    addSlider<float>("Opacity", "NULL", ValueType::FLOAT_T, &opacity, 0.1f, 255.f);
    addSlider<int>("Radius", "NULL", ValueType::INT_T, &esese, 1, 40);
}

void StorageESP::getBlocks(std::vector<Vec3<int>>& blocks) {
    blocks.clear();
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return;

    // Convertir posición del jugador a coordenadas de chunk
    int playerChunkX = static_cast<int>(floor(localPlayer->getPosition()->x)) >> 4;
    int playerChunkZ = static_cast<int>(floor(localPlayer->getPosition()->z)) >> 4;
    int playerY = static_cast<int>(localPlayer->getPosition()->y);
    
    // Convertir radio a chunks
    int chunkRadius = (esese + 15) >> 4;
    
    // Iterar por chunks
    for (int cx = -chunkRadius; cx <= chunkRadius; cx++) {
        for (int cz = -chunkRadius; cz <= chunkRadius; cz++) {
            // Coordenadas base del chunk actual
            int baseX = (playerChunkX + cx) << 4;
            int baseZ = (playerChunkZ + cz) << 4;
            
            // Iterar bloques dentro del chunk
            for (int x = 0; x < 16; x++) {
                for (int y = std::max(0, playerY - esese); 
                     y < std::min(256, playerY + esese); y++) {
                    for (int z = 0; z < 16; z++) {
                        Vec3<int> blockPos(baseX + x, y, baseZ + z);
                        
                        // Verificar distancia al jugador
                        float dx = blockPos.x - localPlayer->getPosition()->x;
                        float dy = blockPos.y - localPlayer->getPosition()->y;
                        float dz = blockPos.z - localPlayer->getPosition()->z;
                        if (dx*dx + dy*dy + dz*dz > esese*esese) continue;
                        
                        Block* block = localPlayer->dimension->blockSource->getBlock(blockPos);
                        if (!block) continue;

                        // Verificar si es bloque de almacenamiento
                        if (block->blockLegacy->blockId == 54 ||   // Normal Chest
                            block->blockLegacy->blockId == 146 ||  // Trapped Chest
                            block->blockLegacy->blockId == 130 ||  // Ender Chest
                            block->blockLegacy->blockId == 458 ||  // Barrel
                            block->blockLegacy->blockId == 205 ||  // Undyed Shulker Box
                            block->blockLegacy->blockId == 218 ||  // Shulker Box
                            block->blockLegacy->blockId == 154) {  // Hopper
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

    getBlocks(blocks);
    if (blocks.empty()) return;

    for (const Vec3<int>& blockPos : blocks) {
        Block* block = mc.getLocalPlayer()->dimension->blockSource->getBlock(blockPos);
        AABB aabb = mc.getLocalPlayer()->makeAABB(&blockPos);
        UIColor flushColor = UIColor();

        if (block->blockLegacy->blockId == 54) flushColor = UIColor(1.f, 1.f, 1.f, opacity);                     // Normal Chest
        if (block->blockLegacy->blockId == 146) flushColor = UIColor(.92f, .14f, .14f, opacity);                 // Trapped Chest
        if (block->blockLegacy->blockId == 130) flushColor = UIColor(0.435294f, 0.215686f, 0.631372f, opacity);  // Ender Chest
        if (block->blockLegacy->blockId == 458) flushColor = UIColor(0.62f, 0.31f, 0.04f, opacity);              // Barrel
        if (block->blockLegacy->blockId == 205) flushColor = UIColor(.49f, .17f, .95f, opacity);                 // Undyed Shulker Box
        if (block->blockLegacy->blockId == 218) flushColor = UIColor(.08f, .91f, .99f, opacity);                 // Shulker Box
        if (block->blockLegacy->blockId == 154) flushColor = UIColor(166 / 255.f, 166 / 255.f, 166 / 255.f, opacity);  // Hopper

        RenderUtils::setColor(flushColor.r, flushColor.g, flushColor.b, flushColor.a);
        Vec3<float> blockPosFloat(
            static_cast<float>(blockPos.x),
            static_cast<float>(blockPos.y),
            static_cast<float>(blockPos.z)
        );
        RenderUtils::drawBox(blockPosFloat, flushColor, flushColor, .3f, false, true);
    }
}
