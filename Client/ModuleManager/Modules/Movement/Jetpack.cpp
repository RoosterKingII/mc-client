#include "Jetpack.h"
Jetpack::Jetpack() : Module("Jetpack", "Fly around like you had a Jetpack!", Category::MOVEMENT) {
	addSlider<float>("Speed", "NULL", ValueType::FLOAT_T, &speedMod, 0.2f, 5.f);
	addBoolCheck("Bypass", "NULL", &this->isBypass);
}
Jetpack::~Jetpack() {
}
bool Jetpack::isFlashMode() {
	return true;
}
void Jetpack::onNormalTick(Actor* actor) {
	float calcYaw = (mc.getLocalPlayer()->rotationComponent->rotation.y + 90) * (PI / 180);
	float calcPitch = (mc.getLocalPlayer()->rotationComponent->rotation.x) * -(PI / 180);
	if (!isBypass) {
		Vec3<float> moveVec;
		moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
		moveVec.y = sin(calcPitch) * speedMod;
		moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;
		mc.getLocalPlayer()->lerpMotion(moveVec);
	}
	else {
		delay++;
		if (delay >= 5) {
			Vec3<float> pos = *mc.getLocalPlayer()->getPosition();
			MovePlayerPacket a(mc.getLocalPlayer(), pos);
			mc.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			pos.y += 0.35f;
			a = MovePlayerPacket(mc.getLocalPlayer(), pos);
			mc.getClientInstance()->loopbackPacketSender->sendToServer(&a);
			mc.getLocalPlayer()->stateVectorComponent->velocity.y = 0.465f;
			Vec3<float> moveVec;
			moveVec.x = cos(calcYaw) * cos(calcPitch) * speedMod;
			moveVec.z = sin(calcYaw) * cos(calcPitch) * speedMod;
			mc.getLocalPlayer()->stateVectorComponent->velocity.x = moveVec.x;
			mc.getLocalPlayer()->stateVectorComponent->velocity.z = moveVec.z;
			float teleportX = cos(calcYaw) * cos(calcPitch) * 0.00000005f;
			float teleportZ = sin(calcYaw) * cos(calcPitch) * 0.00000005f;
			pos = *mc.getLocalPlayer()->getPosition();
			mc.getLocalPlayer()->setPos(Vec3<float>(pos.x + teleportX, pos.y - 0.15f, pos.z + teleportZ));
			mc.getLocalPlayer()->stateVectorComponent->velocity.y -= 0.15f;
			delay = 0;
		}
	}
}