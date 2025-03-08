#include "ChunkESP.h"

ChunkESP::ChunkESP() : Module("ChunkESP", "Visualiza los límites de chunks", Category::RENDER) {
    addEnumSetting("Mode", "Cambia el modo de renderizado", {"3D", "2D"}, &renderMode);
    addColorPicker("Color", "Color del relleno", &color);
    addColorPicker("LineColor", "Color de las líneas", &lineColor);
    addSlider("LineWidth", "Grosor de líneas", ValueType::FLOAT_T, &lineWidth, 0.1f, 5.0f);
    addBoolCheck("Fill", "Rellenar chunks", &fill);
    addBoolCheck("Outline", "Mostrar bordes", &outline);
    addSlider("RenderDistance", "Distancia de renderizado", ValueType::INT_T, &renderDistance, 1, 8);
}

void ChunkESP::onRender(MinecraftUIRenderContext* ctx) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (localPlayer == nullptr) return;
    if (!mc.canUseMoveKeys()) return;

    Vec3<float> playerPos = *localPlayer->getPosition();
    int playerChunkX = static_cast<int>(floor(playerPos.x)) >> 4;
    int playerChunkZ = static_cast<int>(floor(playerPos.z)) >> 4;

    // Renderizar chunks alrededor del jugador
    for (int dx = -renderDistance; dx <= renderDistance; dx++) {
        for (int dz = -renderDistance; dz <= renderDistance; dz++) {
            int chunkX = (playerChunkX + dx) << 4;
            int chunkZ = (playerChunkZ + dz) << 4;

            // Crear AABB para el chunk
            AABB chunkBox(
                Vec3<float>(chunkX, 0, chunkZ),
                Vec3<float>(chunkX + 16, 256, chunkZ + 16)
            );

            switch (renderMode) {
                case 0: // 3D
                    RenderUtils::drawBox(
                        chunkBox,
                        color,
                        lineColor,
                        lineWidth,
                        fill,
                        outline
                    );
                    break;

                case 1: // 2D
                    RenderUtils::draw2DBox(
                        chunkBox,
                        color,
                        lineColor,
                        lineWidth,
                        fill,
                        outline
                    );
                    break;
            }
        }
    }
}
