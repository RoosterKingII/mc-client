#include "Switcher.h"

Switcher::Switcher() : Module("Switcher",  "Switches between hotbar slots", Category::COMBAT) {
	addBoolCheck("DoubleSwitch","NULL", &ss);
	addBoolCheck("AutoSwitch", "NULL", &autos);
	addSlider<int>("Slots", "NULL", ValueType::INT_T, &ternary,  0, 8);
	addSlider<int>("Delay","NULL", ValueType::INT_T, &ffdl, 0, 5);
}


void Switcher::onEnable() {
}

void Switcher::onAttack(Actor* attackedEnt) {
	auto* supplies = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = supplies->inventory;
	if (!ownage && !ss) {
		ternary1++;
		if (ternary1 >= ternary + 1) {
			ternary1 = 0;
		}
	}
	supplies->selectedSlot = ternary1;

	if (ownage && !autos) {
		if (ternaryO == false) {
			if (toggler == false) {
				toggler = true;
				supplies->selectedSlot = 0;
			}
			else {
				toggler = false;
				supplies->selectedSlot = 4;
				ternaryO = true;
			}
		}
		else {
			supplies->selectedSlot = 5;
			ternaryO = false;
		}
	}

	if (ss && !autos) {
		if (swap == false) {
			swap = true;
			supplies->selectedSlot = 0;
		}
		else {
			swap = false;
			supplies->selectedSlot = 1;
		}
	}
}

void Switcher::onNormalTick(Actor* actor) {
	auto* supplies = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = supplies->inventory;
	ff++;
	if (ff > ffdl) {
		if (autos && !ownage) {
			ternary1++;
			if (ternary1 >= ternary + 1) {
				ternary1 = 0;
			}
			supplies->selectedSlot = ternary1;
		}
		if (autos && ownage) {
			if (ternaryO == false) {
				if (toggler == false) {
					toggler = true;
					supplies->selectedSlot = 0;
				}
				else {
					toggler = false;
					supplies->selectedSlot = 4;
					ternaryO = true;
				}
			}
			else {
				supplies->selectedSlot = 5;
				ternaryO = false;
			}
		}
		if (ss && autos) {
			if (swap == false) {
				swap = true;
				supplies->selectedSlot = 0;
			}
			else {
				swap = false;
				supplies->selectedSlot = 1;
			}
		}
		ff = 0;
	}
}

