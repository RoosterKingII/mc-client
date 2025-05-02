#include "BasePlace.h"

BasePlace::BasePlace() : Module("BasePlace", "Automatically places obsidian to place end crystals", Category::PLAYER)
{
	addSlider<float>("Range","NULL", ValueType::FLOAT_T, &range, 3, 10);
	addBoolCheck("Silent", "NULL", & silentab);
	addBoolCheck("NoTarget", "NULL", &notarget);
	addBoolCheck("Mobs", "NULL", & mobs);
}

int getObsidiansexi() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 49) {
				return i;
			}
		}
	}
	return slot;
}

static std::vector<Actor*> targetListcc2;
bool BasePlace::canpla(Vec3<int> blockPos)
{
	if (mc.getLocalPlayer()->getPosition()->dist(blockPos) >= 7)
	{
		return false;
	}
	else
		return true;
}

bool BasePlace::tryAutoTrap2(Vec3<int> blkPlacement)
{
	// blkPlacement = blkPlacement.floor();

	Block* block = mc.getLocalPlayer()->dimension->blockSource->getBlock(Vec3<int>(blkPlacement));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->blockId == 0)
	{
		Vec3<int> blok(blkPlacement);
		int i = 0;

		static std::vector<Vec3<int>*> checklist;
		bool foundCandidate = false;
		if (checklist.empty())
		{
			checklist.push_back(new Vec3<int>(0, -1, 0));
			checklist.push_back(new Vec3<int>(0, 1, 0));

			checklist.push_back(new Vec3<int>(0, 0, -1));
			checklist.push_back(new Vec3<int>(0, 0, 1));

			checklist.push_back(new Vec3<int>(-1, 0, 0));
			checklist.push_back(new Vec3<int>(1, 0, 0));
		}

		for (auto current : checklist)
		{
			Vec3<int> calc = blok.sub(*current);
			bool Y = ((mc.getLocalPlayer()->dimension->blockSource->getBlock(calc)->blockLegacy))->blockId == 0;
			if (((mc.getLocalPlayer()->dimension->blockSource->getBlock(calc)->blockLegacy))->blockId == 0)
			{
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate)
		{
			mc.getGameMode()->buildBlock(blok, i, true);
			return true;
		}
	}
	return false;
}
void BasePlace::onNormalTick(Actor* actor)
{
	auto player = mc.getLocalPlayer();
	if (player == nullptr)
		return;
	aovcl = false;

	auto* supplies = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = supplies->inventory;

	targetListcc2.clear();
	for (Actor* actor : player->getLevel()->getRuntimeActorList()) {
		if (TargetUtils::isFriendValid(actor, mobs)) {
			float seenPercent = player->dimension->blockSource->getSeenPercent(player->getEyePos(), *actor->getAABB());
			float dist = actor->getPosition()->dist(*player->getPosition());
			float rangeCheck = (seenPercent > 0.f) ? 7 : 7;
			if (dist < rangeCheck) targetListcc2.push_back(actor);
		}

		if (silentab)
		{
			aovcl = true;
		}
		if (mc.getLocalPlayer()->getCarriedItem()->getItemPtr()->itemId == 49 || aovcl)
		{
			if (!notarget)
			{
				if (!targetListcc2.empty())
				{
					for (auto& a : targetListcc2)
					{
						Vec3<int> targetPosi = targetListcc2[0]->getPosition()->floor().toInt();
						for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++)
						{
							for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++)
							{
								if (silentab)
								{
									int sexslot = getObsidiansexi();
									int oldSlot = supplies->selectedSlot;
									bool shouldSwitch = (sexslot != supplies->selectedSlot);
									if (targetListcc2.empty() == false) {
										if (shouldSwitch) {
											supplies->selectedSlot = sexslot;
											MobEquipmentPacket pk(mc.getLocalPlayer()->getRuntimeID(), inv->getItemStack(sexslot), sexslot, sexslot);
											mc.getClientInstance()->loopbackPacketSender->send(&pk);
										}
									}
								}
								if (canpla(Vec3<int>(x, (int)targetPosi.y - 1.6f, z)))
								{
									tryAutoTrap2(Vec3<int>(x, (int)targetPosi.y - 1.6f, z));
								}
								if (silentab)
								{
									supplies->selectedSlot = slotab;
								}
							}
						}
					}
				}
			};
			if (notarget)
			{
				Vec3<int> targetPosi = mc.getLocalPlayer()->getPosition()->floor().toInt();
				for (int x = targetPosi.x - 2; x <= targetPosi.x + 2; x++)
				{
					for (int z = targetPosi.z - 2; z <= targetPosi.z + 2; z++)
					{
						if (silentab)
						{
							int sexslot = getObsidiansexi();
							int oldSlot = supplies->selectedSlot;
							bool shouldSwitch = (sexslot != supplies->selectedSlot);
							if (targetListcc2.empty() == false) {
								if (shouldSwitch) {
									supplies->selectedSlot = sexslot;
									MobEquipmentPacket pk(mc.getLocalPlayer()->getRuntimeID(), inv->getItemStack(sexslot), sexslot, sexslot);
									mc.getClientInstance()->loopbackPacketSender->send(&pk);
								}
							}
						}
						if (canpla(Vec3<int>(x, (int)targetPosi.y - 1.6f, z)))
						{
							tryAutoTrap2(Vec3<int>(x, (int)targetPosi.y - 1.6f, z));
						}
						if (silentab)
						{
							supplies->selectedSlot = slotab;
						}
					}
				}
			};
		}
	}
}
void BasePlace::onRender(MinecraftUIRenderContext* renderCtx)
{
	auto player = mc.getLocalPlayer();
	if (player == nullptr)
		return;
}