#pragma once
#include "../../../../Utils/MemoryUtils.h"
#include "../../../../Utils/RenderUtils.h"
#include "../../../../SDK/Classes/ScreenView.h"
#include "../../../../SDK/Classes/UILayer.h"
#include "../../../../SDK/Classes/MinecraftUIRenderContext.h"
#include "../../../../SDK/GameData.h"

#include "../MinecraftUIRenderContext/DrawImageHook.h"
#include "../MinecraftUIRenderContext/DrawTextHook.h"
#include "../Others/SendPacketHook.h"
#include "../Others/NineSlicehook.h"

#include "../../../Client.h"
#include "../../../../Utils/GuiInfo.h" // <--- INCLUDE GuiInfo.h AQUÍ
Vec2<float> titlePos = { 0,0 };
float anim = 0;
Vec2<float> titleSize = { 0,0 };
// *** REMOVE NAMESPACE GuiInfo - NOW IN GuiInfo.h ***
class SetUpAndRenderHook {
protected:
    using func_t = __int64(__fastcall*)(ScreenView*, MinecraftUIRenderContext*);
    static inline func_t func;

    static __int64 SetUpAndRender(ScreenView* _this, MinecraftUIRenderContext* renderCtx) {
        mc.setClientInstance(renderCtx->clientInstance);
        RenderUtils::SetUp(_this, renderCtx);

        // *** LOG - Loguear GuiInfo::ScreenRes después de RenderUtils::SetUp ***
        logF("[SetUpAndRenderHook] - GuiInfo::ScreenRes after RenderUtils::SetUp: x=%f, y=%f", GuiInfo::ScreenRes.x, GuiInfo::ScreenRes.y);


        static bool hooked = false;
        if (UILayer::Is(_this, UILayer::MainMenu_Screen))
        {
            anim = Math::lerp(1, anim, GuiInfo::DeltaTime * 10);
            Vec4<float> screenRes = Vec4<float>(0, 0, GuiInfo::ScreenRes.x, GuiInfo::ScreenRes.y);

            // *** VALORES DE ZOOM FINAL - ZOOM MÍNIMO - CASI SIN ZOOM ***
            constexpr float finalZoom = 1.0f; // <--- FINAL ZOOM SIGUE EN 1.0 (SIN ZOOM)
            static float zoom = 1.01f, opacity = 0; // <--- ZOOM INICIAL REDUCIDO A 1.01 (ZOOM MÍNIMO)
            zoom = Math::lerp(1.01f, finalZoom, anim), opacity = Math::lerp(0, 1, anim);

            Vec2<float> size = Vec2<float>(screenRes.z * zoom, screenRes.w * zoom);

            // *** CALCULO DE RECT POSITION SIMPLIFICADO - CENTRADO DIRECTAMENTE ***
            Vec2<float> rectPosition = Vec2<float>((screenRes.z - size.x) / 2.0f, (screenRes.w - size.y) / 2.0f); // <--- CENTRADO DIRECTO


            // Final rect area
            Vec4<float> rect = Vec4<float>(rectPosition.x, rectPosition.y, rectPosition.x + size.x, rectPosition.y + size.y);

            logF("[SetUpAndRenderHook] - Rect values before renderImage: x=%f, y=%f, z=%f, w=%f", rect.x, rect.y, rect.z, rect.w); // <--- LOG RECT

            RenderUtils::renderImage(Utils::getClientPath() + "Assets\\mainMenuBackground.png", rect);

            opacity = 1.0f; // <--- FORZAR OPACIDAD A 1.0f
            RenderUtils::flushImage(MC_Color(255, 255, 255), opacity);
        }
        if (!hooked) {
            uintptr_t* CtxVTable = *(uintptr_t**)renderCtx;
            uintptr_t* loopbackPacketSenderVTable = *(uintptr_t**)(renderCtx->clientInstance->loopbackPacketSender);
            DrawTextHook::init(CtxVTable[5]);
            DrawImageHook::init(CtxVTable[7]);
            SendPacketHook::init(loopbackPacketSenderVTable[1]);
            NineSlicehook::init(CtxVTable[8]);
            hooked = true;
        }
        __int64 result = func(_this, renderCtx);
        if (_this->visualTree->root->GetName() == UILayer::Debug_DebugScreen) {
            client->moduleMgr->onRender(renderCtx);
        }
        return result;
    }
public:
    static void init() {
        uintptr_t address = findSig(Sigs::hook::SetupAndRenderHook);
        MemoryUtils::CreateHook("SetUpAndRenderHook", address, (void*)&SetUpAndRenderHook::SetUpAndRender, (void*)&func);
    }
};
