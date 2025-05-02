#include "octopus.h"

octopus::octopus() : Module("Octopus",  "like switcher but selects all 9 slots at once", Category::COMBAT) { //this is a test, should use all weapons at once. has worked before.
}


void octopus::onEnable() {
}

void octopus::onAttack(Actor* attackedEnt) {
	mc.DisplayClientMessage("[%sBoost%s] %sAttempting to attack with all 9 slots", BLUE, WHITE, RED);
}

void octopus::onNormalTick(Actor* actor) {
	auto* supplies = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = supplies->inventory;
	supplies->selectedSlot = 0;
  supplies->selectedSlot = 1;
  supplies->selectedSlot = 2;
  supplies->selectedSlot = 3;
  supplies->selectedSlot = 4;
  supplies->selectedSlot = 5;
  supplies->selectedSlot = 6;
  supplies->selectedSlot = 7;
  supplies->selectedSlot = 8;
}
