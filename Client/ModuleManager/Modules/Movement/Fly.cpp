#include "Fly.h"
#include "../Combat/Killaura.h"
#include <cmath> // Para sqrt

Fly::Fly() : Module("TestFly", "USE WITH DISABLER TO BYPASS FLYING IN LB.", Category::MOVEMENT) {
    addSlider<float>("Horizontal Speed", "NULL", ValueType::FLOAT_T, &this->HorizontalSpeed, 1.00, 5.f);
    addBoolCheck("SM Border", "NULL", &smborder);
    addSlider<float>("Vertical Speed", "NULL", ValueType::FLOAT_T, &this->upanddown, 1.00, 5.f);
    addSlider<float>("Glide", "How fast you want to glide.", ValueType::FLOAT_T, &glideMod, -0.2f, 1.f);
    addEnumSetting("Mode", "NULL", { "Motion", "Noclip" }, &Modefly); // Apenas o modo Motion
    lastMoveTime = 0; // Inicializando o tempo do último movimento
}

Fly::~Fly() {
}

void Fly::onNormalTick(Actor* actor) {
    // Define DelayUpDown apenas para o modo Motion
    auto* pos = mc.getLocalPlayer()->getPosition();
    DelayUpDown = 0; // Sempre 0 para o modo Motion

    auto gm = mc.getGameMode();
    gm->player->stateVectorComponent->velocity = Vec3<float>(0, 0, 0); // Zera a velocidade do jogador

    float glideModEffective = glideMod; // Modificação do deslizamento

    // Verifica as teclas pressionadas para movimento vertical com delay
    if (mc.canUseMoveKeys()) {
        if (mc.isKeyDown(VK_SPACE)) {
            glideModEffective += upanddown; // Sobe
        }
        if (mc.isKeyDown(VK_SHIFT)) {
            glideModEffective -= upanddown; // Desce
        }
    }

    // Limitar a movimentação do jogador
    if (smborder) {
        auto* pos = mc.getLocalPlayer()->getPosition();

        if (pos->x > 1090)
            mc.getLocalPlayer()->setPos(Vec3(pos->x = 1090, pos->y, pos->z));

        if (pos->z > 1090)
            mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y, pos->z = 1090));

        if (pos->x < 60)
            mc.getLocalPlayer()->setPos(Vec3(pos->x = 60, pos->y, pos->z));

        if (pos->z < 60)
            mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y, pos->z = 60));

        if (pos->y > 155)
            mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y = 155, pos->z));
    }

    gm->player->stateVectorComponent->velocity.y = glideModEffective; // Define a velocidade vertical

    auto player = mc.getLocalPlayer();
    if (player == nullptr) return;

    // Verificar as teclas pressionadas
    bool w = mc.isKeyDown('W');
    bool a = mc.isKeyDown('A');
    bool s = mc.isKeyDown('S');
    bool d = mc.isKeyDown('D');

    float yaw = player->rotationComponent->Get().y; // Captura o yaw (ângulo de rotação)
    float radiansYaw = yaw * (PI / 180); // Converte o yaw para radianos para usar nas funções trigonométricas

    Vec3<float> moveVec(0, 0, 0); // Inicializa o vetor de movimento

    // Verifica se IsWpressed é 1 para inverter as direções
    bool IsWpressed = this->IsWpressed;

    // Lógica de movimentação invertida

        // Lógica padrão de movimentação
    if (w) {
        moveVec.x -= sin(radiansYaw) * HorizontalSpeed;
        moveVec.z += cos(radiansYaw) * HorizontalSpeed;
    }
    if (a) {
        moveVec.x += cos(radiansYaw) * HorizontalSpeed;
        moveVec.z += sin(radiansYaw) * HorizontalSpeed;
    }
    if (s) {
        moveVec.x += sin(radiansYaw) * HorizontalSpeed;
        moveVec.z -= cos(radiansYaw) * HorizontalSpeed;
    }
    if (d) {
        moveVec.x -= cos(radiansYaw) * HorizontalSpeed;
        moveVec.z -= sin(radiansYaw) * HorizontalSpeed;
    }


    // Normaliza o vetor de movimento se houver mais de uma direção sendo pressionada
    float length = sqrt(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
    if (length > 0) {
        moveVec.x = (moveVec.x / length) * HorizontalSpeed;
        moveVec.z = (moveVec.z / length) * HorizontalSpeed;
    }

    // Aplica a velocidade vertical
    moveVec.y = player->stateVectorComponent->velocity.y;
    if (Modefly == 0) {
        // Aplica o movimento ao jogador
        player->lerpMotion(moveVec);
    }
    else {
        // Apply movement only if pressed
        pos->x += moveVec.x;
        pos->y += moveVec.y;
        pos->z += moveVec.z;
        player->setPos(*pos);
    }
}

void Fly::onEnable() {
}

void Fly::onDisable() {
    if (mc.getLocalPlayer() != nullptr) {
        // Se precisar, reinicie as propriedades do jogador aqui
    }
}


/*#include "Fly.h"

Fly::Fly() : Module("VeloFly", "Velocityfly", Category::MOVEMENT) {
	addSlider<float>("Horizontal Speed", "NULL", ValueType::FLOAT_T, &speed, 0.03, 2.f );
	addSlider<float>("Vertical Speed", "NULL", ValueType::FLOAT_T, &this->upanddown, 0.03, 2.f);
	//this->addBoolCheck("Save Old POS", "NULL", &this->NoPacket);
	addSlider<float>("Glide", "How fast you want to glide.", ValueType::FLOAT_T, &glideMod, -0.2f, 1.f);
	addBoolCheck("SM Border", "NULL", &smborder);
	addBoolCheck("Sneak", "NULL", &sneak);
}

Fly::~Fly() {
}

void Fly::onNormalTick(Actor* actor) {
	auto gm = mc.getGameMode();
	gm->player->stateVectorComponent->velocity = Vec3<float>(0, 0, 0);
	glideModEffective = glideMod;
	if (mc.canUseMoveKeys()) {
		if (mc.isKeyDown(VK_SPACE))
			glideModEffective += upanddown;
		if (mc.isKeyDown(VK_SHIFT))
			glideModEffective -= upanddown;
	}
	if (smborder) {
		auto* pos = mc.getLocalPlayer()->getPosition();

		if (pos->x > 1090)
			mc.getLocalPlayer()->setPos(Vec3(pos->x = 1090, pos->y, pos->z));

		if (pos->z > 1090)
			mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y, pos->z = 1090));

		if (pos->x < 60)
			mc.getLocalPlayer()->setPos(Vec3(pos->x = 60, pos->y, pos->z));

		if (pos->z < 60)
			mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y, pos->z = 60));
		if (pos->y > 155)
			mc.getLocalPlayer()->setPos(Vec3(pos->x, pos->y = 155, pos->z));
	};
	gm->player->stateVectorComponent->velocity.y = glideModEffective;
	auto player = mc.getLocalPlayer();
	if (player == nullptr) return;
	bool w = mc.isKeyDown('W');
	bool a = mc.isKeyDown('A');
	bool s = mc.isKeyDown('S');
	bool d = mc.isKeyDown('D');

	float yaw = player->rotationComponent->Get().y;

	bool pressed = w || a || s || d;

	if (w && s)
		return;


	if (d) {
		yaw += 90.f;
		if (w)
			yaw -= 45.f;
		else if (s)
			yaw += 45.f;
	}
	if (a) {
		yaw -= 90.f;
		if (w)
			yaw += 45.f;
		else if (s)
			yaw -= 45.f;
	}

	if (s && !a && !d)
		yaw += 180.f;

	float calcYaw = (yaw + 90) * (PI / 180);
	Vec3<float> moveVec;
	moveVec.x = cos(calcYaw) * speed;
	moveVec.y = player->stateVectorComponent->velocity.y;
	moveVec.z = sin(calcYaw) * speed;
	if (pressed) player->lerpMotion(moveVec); 
	if (sneak) { 
		mc.getLocalPlayer()->setSneaking(true);
	}
}

void Fly::onEnable() {
}

void Fly::onDisable() {
	if (mc.getLocalPlayer() != nullptr)return;
}
*/
