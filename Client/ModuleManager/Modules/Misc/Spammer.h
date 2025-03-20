#pragma once
#include "../Module.h"
#include <vector>
#include <random>

class Spammer : public Module {
private:
    int delay = 1;
    int length = 8;
    int mode = 0;

    const std::vector<std::string> messages = {
   "ddddd",
   "ggggg"
    };

public:
    Spammer();
    virtual void onNormalTick(Actor* actor) override;
};