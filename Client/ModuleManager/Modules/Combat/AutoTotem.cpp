#include "AutoTotem.h"

AutoTotem::AutoTotem() : Module("AutoTotem", "Automatically replaces totems", Category::COMBAT) {
}

void AutoTotem::onNormalTick(Actor* actor) {
	auto player = mc.getLocalPlayer();
	auto inv = player->getPlayerInventory()->getinventory();
	if (!player->isAlive()) return;

	auto slot = player->getOffhandSlot();
	if (slot->item == NULL) {
		for (int i = 1; i < 36; i++) {
			ItemStack* stack = inv->getItemStack(i);
			if (stack->item != NULL && (*stack->item)->texture_name == "totem") {
				mc.getLocalPlayer()->setOffhandSlot(stack);
				inv->removeItem(i, i);
			}
		}
	}
}