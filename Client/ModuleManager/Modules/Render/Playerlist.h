#pragma once
#include "../Module.h"
class Playerlist : public Module {
public:
    Playerlist();

    void onImGuiRender(ImDrawList* drawlist) override;

    std::vector<std::string> getCurrentPlayerList();
private:
    int size = 30; // Define e inicializa size
};
