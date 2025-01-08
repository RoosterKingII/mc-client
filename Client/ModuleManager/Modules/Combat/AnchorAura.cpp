#include "AnchorAura.h"


AnchorAura::AnchorAura() : Module("AnchorAura", "Give people PTSD from nuke even before WW3", Category::COMBAT) {
	addSlider<int>("Range", "NULL", ValueType::INT_T, &range, 3, 10);
	this->addBoolCheck("Mob", "NULL", &this->silent);
}
static int getid(Vec3<float> pos) {
	int id = 0;
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	Block* block = localPlayer->dimension->blockSource->getBlock(pos.toInt());

	id = block->blockLegacy->blockId;
	return id;
}
static std::vector<Actor*> targetList;

static void PredictBlok(Vec3<float> pos) {
	GameMode* gm = mc.getGameMode();
	static std::vector<Vec3<float>> blocks;

	// Hedefin çevresine blok yerleþtirme iþlemleri için gerekli olan yerler
	Vec3<float> bottom = pos.add(0, -1, 0);      // Hedefin altý (Y - 1)
	Vec3<float> headTop = pos.add(0, 2, 0);      // Hedefin kafasýnýn üstü (Y + 2)

	Vec3<float> left = pos.add(-1, 0, 0);        // Hedefin sol yaný (X - 1)
	Vec3<float> right = pos.add(1, 0, 0);        // Hedefin sað yaný (X + 1)
	Vec3<float> front = pos.add(0, 0, -1);       // Hedefin ön yaný (Z - 1)
	Vec3<float> back = pos.add(0, 0, 1);         // Hedefin arka yaný (Z + 1)

	// Üst yanlar (X±1, Z±1, Y+1)
	Vec3<float> topLeft = pos.add(-1, 1, 0);     // X - 1, Y + 1, Z
	Vec3<float> topRight = pos.add(1, 1, 0);     // X + 1, Y + 1, Z
	Vec3<float> topFront = pos.add(0, 1, -1);    // X, Y + 1, Z - 1
	Vec3<float> topBack = pos.add(0, 1, 1);      // X, Y + 1, Z + 1

	// Alt ve yan yerlerde blok yerleþtirme iþlemleri
	std::vector<Vec3<float>> blockPositions = { bottom, headTop, left, right, front, back, topLeft, topRight, topFront, topBack };

	// Blok yerleþtirme iþlemi
	for (const Vec3<float>& blockPos : blockPositions) {
		if (gm->tryPlaceBlock(Vec3<int>(blockPos.x, blockPos.y, blockPos.z))) {
			return;
		}
	}
}

static int getAnchor() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 65264) {
				return i;
			}
		}
	}
	return slot;
}//


static int getGS() {
	PlayerInventory* plrInv = mc.getLocalPlayer()->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	int slot = plrInv->selectedSlot;
	for (int i = 0; i < 9; i++) {
		ItemStack* itemStack = inv->getItemStack(i);
		if (itemStack->isValid()) {
			if (itemStack->getItemPtr()->itemId == 89) {
				return i;
			}
		}
	}
	return slot;
}
static void tryAc(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 0) {

		int bestSlot = getAnchor();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		PredictBlok(tryBuildPos.toFloat());
		//gm->buildBlock(tryBuildPos, 0, 0);
		//gm->buildBlock(tryBuildPos, 0, 0);

		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
static void tryGS(Vec3<int> tryBuildPos) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	GameMode* gm = localPlayer->getGameMode();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;

	Vec3<float> playerPos = *localPlayer->getPosition();
	playerPos.y -= 1.f;
	playerPos = playerPos.floor();

	Block* block = localPlayer->dimension->blockSource->getBlock(tryBuildPos);
	if (block->blockLegacy->blockId == 66063) {

		int bestSlot = getGS();
		int oldSlot = plrInv->selectedSlot;
		bool shouldSwitch = (bestSlot != plrInv->selectedSlot);
		if (shouldSwitch) {
			plrInv->selectedSlot = bestSlot;
			MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
			mc.getClientInstance()->loopbackPacketSender->send(&pk);
		}

		PredictBlok(tryBuildPos.toFloat());
		//gm->buildBlock(tryBuildPos, 0, 0);
		if (shouldSwitch) {
			plrInv->selectedSlot = oldSlot;
		}
	}
}
void AnchorAura::onNormalTick(Actor* actor) {
	if (mc.getLocalPlayer() == nullptr)
		return;
	if (!mc.canUseMoveKeys())
		return;
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	PlayerInventory* plrInv = localPlayer->getPlayerInventory();
	Inventory* inv = plrInv->inventory;
	GameMode* gm = mc.getGameMode();
	BlockSource* region = localPlayer->dimension->blockSource;
	Level* level = localPlayer->getLevel();
	targetList.clear();

	for (Actor* actor : level->getRuntimeActorList()) {
		if (TargetUtils::isFriendValid(actor, silent)) {
			float seenPercent = region->getSeenPercent(localPlayer->getEyePos(), *actor->getAABB());
			float dist = actor->getPosition()->dist(*localPlayer->getPosition());
			float rangeCheck = (seenPercent > 0.f) ? 7 : 7;
			if (dist < rangeCheck) targetList.push_back(actor);
		}
	}
	int place = 0;

	if (!targetList.empty()) {
		Vec3<float> enemyLoc = targetList[0]->getPosition()->floor().sub(Vec3<float>(0.f, 1.f, 0.f));
		std::vector<Vec3<float>> positions = {
	{ enemyLoc } };
		for (auto& pos : positions) {
			Block* block = localPlayer->dimension->blockSource->getBlock(pos.toInt());
			if (!block || !block->blockLegacy)
				continue;
			if (block->blockLegacy->blockId != 0) {
				if (block->blockLegacy->blockId != 66063) {
					continue;
				}
			}
			auto rotationToPlacement = mc.getLocalPlayer()->stateVectorComponent->pos.CalcAngle(pos.toFloat());
			rotAnglePlace = rotationToPlacement;
			//getAnchor6();
			if (getid(pos) == 0) {
				tryAc(pos.toInt());
				shit = true;
			}
			if (shit) {
				//getGS6();
				tryGS(pos.toInt());
				shit = false;
			}
			if (!shit) {
				gm->buildBlock(pos.toInt(), 222, true);
			}
		}
	}
}
void AnchorAura::onRender(MinecraftUIRenderContext* renderCtx)
{
}
void AnchorAura::onSendPacket(Packet* packet, bool& shouldCancel)
{


}
void AnchorAura::onDisable()
{
}
