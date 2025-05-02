#include "Playerlist.h"

Playerlist::Playerlist() : Module("Playerlist", "Shows a list of players in the world", Category::RENDER, VK_TAB) {
    addSlider<int>("Columnas", "Size of the player list", ValueType::INT_T, &size, 10, 30);
    this->toggleMod = 1;
}

void Playerlist::onImGuiRender(ImDrawList* drawlist) {
    if (!this->isEnabled()) return;

    auto playerList = this->getCurrentPlayerList();
    if (playerList.empty()) return;

    constexpr float columnSpacing = 220.0f;
    constexpr float rowSpacing = 25.0f;
    constexpr float padding = 10.0f;

    Vec2<float> windowSize = mc.getClientInstance()->getguiData()->windowSizeReal;
    int totalColumns = (playerList.size() + size - 1) / size;
    int maxRows = std::min((int)playerList.size(), size);
    Vec2<float> listSize = Vec2<float>(
        totalColumns * columnSpacing,
        maxRows * rowSpacing
    );

    Vec2<float> startPos = Vec2<float>(
        windowSize.x / 2 - listSize.x / 2,
        50
    );

    UIColor whiteColor = UIColor(255, 255, 255, 255);
    UIColor blackColor = UIColor(0, 0, 0, 150);

    Vec2<float> backgroundStart = startPos - Vec2<float>(padding, padding);
    Vec2<float> backgroundEnd = backgroundStart + listSize + Vec2<float>(padding * 2, padding * 2);
    ImGuiUtils::fillRectangle(backgroundStart, backgroundEnd, blackColor);

    for (size_t i = 0; i < playerList.size(); i++) {
        int column = i / size;
        int row = i % size;
        float xOffset = column * columnSpacing;
        Vec2<float> textPos = startPos + Vec2<float>(xOffset, row * rowSpacing);
        ImGuiUtils::drawText(textPos, playerList[i].c_str(), whiteColor);
    }
}

std::vector<std::string> Playerlist::getCurrentPlayerList() {
    std::vector<std::string> playerList;
    Level* level = mc.getLocalPlayer()->getLevel();
    if (level) {
        const auto& playerMap = level->getPlayerMap();
        for (const auto& pair : playerMap) {
            playerList.push_back(pair.second.name);
        }
    }
    return playerList;
}