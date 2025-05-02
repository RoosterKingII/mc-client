#include "PlayerJoin.h"
#include "../../../Client.h"
#include "../../../../SDK/GameData.h"
#include "../../../../Utils/TargetUtils.h"
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <chrono> 

PlayerJoin::PlayerJoin() : Module("PlayerJoin", "Show players/mods join messages.", Category::PLAYER) {}

static std::unordered_map<std::string, std::chrono::steady_clock::time_point> lastAlertTime;

void PlayerJoin::onImGuiRender(ImDrawList* drawlist) {
    if (mc.getLocalPlayer() == nullptr) return;
    if (!client->isInitialized()) return;

    std::vector<std::string> currentPlayerList = getCurrentPlayerList();

    static const std::unordered_set<std::string> specialMods = {
    "Lazinesses", // Player list
    "iTzOreoX",
    "xyunami",
    "NotKeru",
    "Piolin177515",
    "Modulo 7",
    "Watermelon54187",
    "Modulo_7",
    "kingK6316",
    "SoftwareSlicer",
    "Holydeesus",
    "IMP deadly",
    "IMP_deadly",
    "x Unii x",
    "x_Unii_x",
    "PlayerYouHate",
    "eua123go TCD",
    "eua123go_TCD",
    "gobbler",
    "Wintry Garli446",
    "Wintry_Garli446",
    "iSSMYT",
    "Misqkii",
    "SupposedJoker40",
    "yTzBlackkoutz",
    "StrayKids4Life",
    "Jolie_Morenita",
    "Im a Uniform",
    "Im_a_Uniform",
    "Jolie Morenita",
    "TaticSoulz",
    "chillarchy",
    "ZacknewAlt",
    "noddae",
    "AL4LA",
    "Mr Blue II",
    "Mr_Blue_II",
    "Mootburrito9652",
    "HHeavennLeaa",
    "ItzMay3173",
    "queenewfie",
    "nz9z",
    "toxica nika",
    "toxica_nika",
    "SloopyKatie",
    "cainesio",
    "SubtotalMeat1",
    "Exposed MRK",
    "Exposed_MRK",
    "omoria",
    "Snow flow7010",
    "Snow_flow7010",
     "x0cyn",
      "cxitty",
    "mprettygirlnc23"
    };

    static std::unordered_set<std::string> seenPlayers;

    auto now = std::chrono::steady_clock::now();

    for (const auto& player : currentPlayerList) {
        if (seenPlayers.find(player) == seenPlayers.end()) {
            mc.DisplayClientMessage("%s[+]%s %s %sJoined", WHITE, DARK_PURPLE, player.c_str(), WHITE);

            if (specialMods.find(player) != specialMods.end()) {
                mc.DisplayClientMessage("%s%s%s %s%sWARNING%s: %sMod %s Joined!", GREEN, BOLD,  RED, BOLD, GREEN,player.c_str());
                lastAlertTime[player] = now;
            }

            seenPlayers.insert(player);
        }
    }

    for (auto it = seenPlayers.begin(); it != seenPlayers.end();) {
        if (std::find(currentPlayerList.begin(), currentPlayerList.end(), *it) == currentPlayerList.end()) {
            mc.DisplayClientMessage("%s[-]%s %s %sLeft", DARK_PURPLE, WHITE, it->c_str(), DARK_PURPLE);

            if (specialMods.find(*it) != specialMods.end()) {
                mc.DisplayClientMessage("%s%s%s %s%sWARNING%s: %sMod %s Left!", GREEN, BOLD, RED, BOLD, GREEN,  it->c_str());
                lastAlertTime.erase(*it);
            }

            it = seenPlayers.erase(it);
        }
        else {
            ++it;
        }
    }

    for (const auto& player : seenPlayers) {
        if (specialMods.find(player) != specialMods.end()) {
            auto lastTime = lastAlertTime[player];
            if (std::chrono::duration_cast<std::chrono::minutes>(now - lastTime).count() >= 2) {
                mc.DisplayClientMessage("%s%s%s %s%sWARNING%s: %sMod %s  online!", GREEN, BOLD, RED, BOLD, GREEN, player.c_str());
                lastAlertTime[player] = now;
            }
        }
    }
}

std::vector<std::string> PlayerJoin::getCurrentPlayerList() {
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