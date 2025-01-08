
#pragma once
#include "../Module.h"
class PlayerJoin : public Module {
public:
    PlayerJoin();

    void onImGuiRender(ImDrawList* drawlist) override;

    std::vector<std::string> getCurrentPlayerList();
};