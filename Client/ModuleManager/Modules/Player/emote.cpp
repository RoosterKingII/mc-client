#include "emote.h"

Emote::Emote() : Module("Emote", "Spam emotes automatically", Category::PLAYER) {
    addSlider<int>("Delay", "Delay between emotes", ValueType::INT_T, &delay, 0, 15);
}

void Emote::onNormalTick(Actor* actor) {
    if (!actor) return;

	if (mc.getLocalPlayer() == nullptr) return;
	delay++;
	if (delay > delay) {
		if (currentEmoteIndex >= emoteUUIDs.size()) {
			currentEmoteIndex = 0; // Reset to the beginning if reached the end
		}
		mc.getLocalPlayer()->playEmote(emoteUUIDs[currentEmoteIndex],false);
		currentEmoteIndex++;
		delay = 0;
	};
};
