#pragma once
#include "../Module.h"
#include "../../ModuleManager.h"

class Origem : public Module {
public:
    Origem();

    void onSendPacket(Packet* packet, bool& shouldCancel) override;
    std::string getModName();

private:
};