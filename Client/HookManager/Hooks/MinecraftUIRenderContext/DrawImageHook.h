#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../SDK/Classes/MinecraftUIRenderContext.h"
#include "../../../../SDK/Classes/TextureData.h"
#include "../../../../Utils/Math.h"
#include "../../../ModuleManager/Modules/Client/HUD.h"
#include "../../../Client.h"
#include "../../../../Utils/ColorUtils.h"  // Incluir la clase para getRainbowColor

float lerping(float endPoint, float current, float speed) {
    if (speed < 0.0) speed = 0.0;
    else if (speed > 1.0) speed = 1.0;

    float dif = std::max(endPoint, current) - std::min(endPoint, current);
    float factor = dif * speed;
    return current + (endPoint > current ? factor : -factor);
}

class DrawImageHook {
protected:
    using func_t = __int64(__fastcall*)(MinecraftUIRenderContext*, TextureData*, Vec2<float>&, Vec2<float>&, Vec2<float>&, Vec2<float>&);
    static inline func_t func;

    static __int64 drawImageCallBack(MinecraftUIRenderContext* _this, TextureData* texturePtr, Vec2<float>& imagePos, Vec2<float>& imageDimension, Vec2<float>& UvPos, Vec2<float>& UvSize) {
        static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");
        static HUD* HudMod = (HUD*)client->moduleMgr->getModule("HUD");
        if (strcmp(texturePtr->ptrToPath->filePath.getText(), "textures/ui/title") == 0) {
            Vec2<float> windowSize = mc.getClientInstance()->guiData->windowSize;
            static const std::string clientName = "Rooster";
            Vec2<float> textPos = Vec2<float>((windowSize.x - RenderUtils::getTextWidth(clientName, 7.5f)) / 2.f, imagePos.y);

            // Obtener color dinámico para el efecto arcoíris
            float rainbowDuration = 5.0f; // Duración del ciclo arcoíris (5 segundos)
            float saturation = 1.0f;
            float brightness = 1.0f;
            UIColor rainbowColor = ColorUtils::getRainbowColor(rainbowDuration, saturation, brightness, 0);

            // Convertir UIColor a MC_Color (Minecraft color)
            MC_Color dynamicColor(rainbowColor.r, rainbowColor.g, rainbowColor.b);

            // Dibujar el texto con color dinámico (arcoíris)
            RenderUtils::drawText(textPos, clientName, dynamicColor, 7.5f, 1.f, true);

            Vec4<float> rect = Vec4<float>(windowSize.x, windowSize.y, windowSize.x + 100, windowSize.y + 100);
            RenderUtils::Flush();
            return 0;
        }
        else if (strcmp(texturePtr->ptrToPath->filePath.getText(), "textures/ui/selected_hotbar_slot") == 0 && HudMod->Hotbar) {
            static float PosX = imagePos.x;
            PosX = lerping(imagePos.x, PosX, 0.016f * 15.f);
            imagePos.x = PosX;
            //return 0;
        }
        const char* prefix = "textures/ui/hotbar_";
        int prefixLen = strlen(prefix);
        if (strncmp(texturePtr->ptrToPath->filePath.getText(), prefix, prefixLen) == 0 && HudMod->Hotbar)
        {
            RenderUtils::fillRectangle(Vec4<float>(imagePos.y + imageDimension.y, imagePos.x - 5, imagePos.y, imagePos.x + imageDimension.x + 5), colorsMod->getColor().toMC_Color(), 0.5f);
            return 0;
        }
        return func(_this, texturePtr, imagePos, imageDimension, UvPos, UvSize);
    }
public:
    static void init(uintptr_t address) {
        MemoryUtils::CreateHook("DrawImageHook", address, (void*)&DrawImageHook::drawImageCallBack, (void*)&func);
    }
};
