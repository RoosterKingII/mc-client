#include "ModuleManager.h"
#include "../Client.h"

ModuleManager::ModuleManager() {

	{ // Init modules
		//Combat
		moduleList.push_back(new Killaura());
		moduleList.push_back(new AutoCrystal());
		moduleList.push_back(new Killaura2());
		moduleList.push_back(new Reach());
		moduleList.push_back(new Switcher());
		moduleList.push_back(new Hitbox());
		moduleList.push_back(new AutoTrap());
		moduleList.push_back(new Godmode());
		moduleList.push_back(new AnchorNeo());
		moduleList.push_back(new AnchorAura());
		//push_back(new octopus());
		moduleList.push_back(new AutoTotem());
		moduleList.push_back(new TPAura());
		moduleList.push_back(new Shoreline());
		//Misc
		moduleList.push_back(new NoPacket());
		moduleList.push_back(new Timer());
		moduleList.push_back(new Origem());
		moduleList.push_back(new AntiCrystal());
		moduleList.push_back(new Clip());
		moduleList.push_back(new FallCounter());
		moduleList.push_back(new LifeboatReach());
		moduleList.push_back(new Blink());
		moduleList.push_back(new DeathCoordinates());
		moduleList.push_back(new Disabler());
		//moduleList.push_back(new ProxyFake());
		moduleList.push_back(new DamageText());
		moduleList.push_back(new ChatSuffix());
		//Render
		moduleList.push_back(new CameraNoClip());
		moduleList.push_back(new CustomFov());
		moduleList.push_back(new Fullbright());
		moduleList.push_back(new HurtColor());
		moduleList.push_back(new NameTags());
		moduleList.push_back(new BlockOutline());
		moduleList.push_back(new BlockESP());
		moduleList.push_back(new StorageESP());
		moduleList.push_back(new NoHurtcam());
		moduleList.push_back(new NoRender());
		moduleList.push_back(new Tracer());
		moduleList.push_back(new ESP());
		moduleList.push_back(new CustomSky());
		moduleList.push_back(new Playerlist());
		//Movement
		moduleList.push_back(new AutoSprint());
		moduleList.push_back(new Fly());
		moduleList.push_back(new Glide());
		moduleList.push_back(new Jetpack());
		moduleList.push_back(new Velocity());
		moduleList.push_back(new NoClip());
		moduleList.push_back(new Phase());
		moduleList.push_back(new Speed());
		moduleList.push_back(new ElytraFly());
		moduleList.push_back(new NoSlowDown());
		//Player
		moduleList.push_back(new Antibot());
		moduleList.push_back(new TeleportModule());
		moduleList.push_back(new BlockReach());
		moduleList.push_back(new Scaffold());
		moduleList.push_back(new FastEat());
		moduleList.push_back(new PlayerJoin());
		moduleList.push_back(new PacketMine());
		moduleList.push_back(new BasePlace());
		moduleList.push_back(new Swing());
		//moduleList.push_back(new Surround());
		//moduleList.push_back(new AutoOffhand());
		moduleList.push_back(new MidClick());
		//Client
		moduleList.push_back(new ArrayList());
		moduleList.push_back(new Colors());
		moduleList.push_back(new CustomFont());
		moduleList.push_back(new ClickGui());
		moduleList.push_back(new HUD());
		moduleList.push_back(new Notifications());
		moduleList.push_back(new Config121());
	}
	std::sort(moduleList.begin(), moduleList.end(), [](Module* lhs, Module* rhs) {
		return lhs->getModuleName() < rhs->getModuleName();
		});
	getModule("ClickGui")->setEnabled(false);
	getModule("Notifications")->setEnabled(true);
	getModule("HUD")->setEnabled(true);
	this->initialized = true;
}

ModuleManager::~ModuleManager() {
	for (Module* mod : moduleList) {
		mod->setEnabled(false);
		delete mod;
	}
}

void ModuleManager::onSaveConfig(json* currentConfig) {
	((ClickGui*)getModule("ClickGui"))->setEnabled(false);
	for (Module* mod : moduleList) {
		mod->onSaveConfig(currentConfig);
	}
}

void ModuleManager::onLoadConfig(json* config) {
	for (Module* mod : moduleList) {
		mod->onLoadConfig(config);
	}
	((ClickGui*)getModule("ClickGui"))->setEnabled(false);
}

void ModuleManager::onKeyUpdate(int key, bool isDown) {
	if (!client->isInitialized()) return;
	for (Module* mod : moduleList) {
		mod->onKeyUpdate(key, isDown);
	}
}

void ModuleManager::onRender(MinecraftUIRenderContext* ctx) {
	if (!client->isInitialized()) return;
	for (Module* mod : moduleList) {
		if (mod->isEnabled()) {
			mod->onRender(ctx);
		}
	}
}

void ModuleManager::onImGuiRender(ImDrawList* drawlist) {
	if (!client->isInitialized()) return;
	for (Module* mod : moduleList) {
		if (mod->isEnabled()) {
			mod->onImGuiRender(drawlist);
		}
	}
}

void ModuleManager::onNormalTick(Actor* actor) {
	if (!client->isInitialized()) return;
	for (Module* mod : moduleList) {
		if (mod->isEnabled()) {
			mod->onNormalTick(actor);
		}
	}
}
void ModuleManager::onContainerScreenControllerTickEvent(ContainerScreenControllerTickEvent* events) {
	if (!client->isInitialized()) return;
	for (Module* mod : moduleList) {
		if (mod->isEnabled()) {
			mod->onContainerScreenControllerTickEvent(events);
		}
	}
}

void ModuleManager::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (!client->isInitialized()) return;
	for (Module* mod : moduleList) {
		if (mod->isEnabled()) {
			mod->onSendPacket(packet, shouldCancel);
			if (shouldCancel) break;
		}
	}
}

void ModuleManager::onLevelRender() {
	if (!client->isInitialized()) return;
	for (Module* mod : moduleList) {
		if (mod->isEnabled()) {
			mod->onLevelRender();
		}
	}
}