#include "FallCounter.h"
#include "../../../Client.h"

// Construtor do módulo
FallCounter::FallCounter() : Module("FallCounter", "Displays Y position.", Category::MISC) {
}

auto player = mc.getLocalPlayer();

// Variáveis globais
float previousY = 0.0f;
float maxHeight = 0.0f;
float Kaka23 = 0.0f;
ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // Cor inicial com alpha 1.0

// Variáveis para verificar proximidade com os blocos
bool isNearBlock = false;
bool isNearWater = false; // Variável para verificar proximidade com água
bool toggleOpacity = false; // Variável para alternar a opacidade
float timeElapsed = 0.0f; // Usado para controlar a alternância

// Método chamado a cada tick normal
void FallCounter::onNormalTick(Actor* actor) {

    if (Kaka23 > -1) {
        Xdd = -66.51;

    }
    if (Kaka23 > 9) {
        Xdd = -71.66;

    }
    if (Kaka23 > 99) {
        Xdd = -75.10;

    }


    toggleOpacity = !toggleOpacity; // Alterna o estado
    if (mc.getLocalPlayer() != nullptr) {
        Vec3<float>* playerPosition = mc.getLocalPlayer()->getPosition();
        if (playerPosition != nullptr) {
            float currentY = playerPosition->y;

            // Atualiza a maior altura atingida pelo jogador
            if (currentY > previousY) {
                if (currentY > maxHeight) {
                    maxHeight = currentY;
                }
            }

            // Atualiza a queda caso o jogador esteja descendo
            if (currentY < previousY) {
                Kaka23 = maxHeight - currentY;
                if (Kaka23 < 0) Kaka23 = 0;
            }
            // Atualiza a queda caso o jogador esteja descendo
            if (currentY > previousY) {
                Kaka23 = maxHeight - currentY;
                if (Kaka23 < 0) Kaka23 = 0;
            }


            previousY = currentY;

            // Verifica se o jogador está perto de qualquer bloco
            isNearBlock = false; // Variável para verificar proximidade com qualquer bloco
            isNearWater = false; // Variável para verificar proximidade com blocos de água

            for (int x = (int)playerPosition->x - 1; x <= (int)playerPosition->x + 1; x++) {
                for (int y = (int)playerPosition->y - 1; y <= (int)playerPosition->y + 1; y++) {
                    for (int z = (int)playerPosition->z - 1; z <= (int)playerPosition->z + 1; z++) {

                        // Para blocos gerais (exceto água), usa y - 0.05
                        Vec3<int> blockPos(playerPosition->x, y - 0.001, playerPosition->z); // Converte para Vec3<int> com ajuste em Y
                        int blockID = mc.getLocalPlayer()->dimension->blockSource->getBlock(blockPos)->blockLegacy->blockId;

                        // Detecta qualquer bloco que não seja água nem ar
                        if (blockID != 0 && blockID != 8 && blockID != 9) {
                            isNearBlock = true;
                            Kaka23 = 0; // Redefine Kaka23 quando perto de qualquer bloco
                            maxHeight = 0.0f;
                            break;
                        }

                        // Para blocos de água, usa y sem ajustes
                        Vec3<int> blockPosWater(playerPosition->x, y + 1.00, playerPosition->z); // Converte para Vec3<int> com y sem ajustes
                        int blockIDWater = mc.getLocalPlayer()->dimension->blockSource->getBlock(blockPosWater)->blockLegacy->blockId;

                        // Detecta blocos de água (IDs 8 ou 9 no Minecraft)
                        if ((blockIDWater == 8 || blockIDWater == 9)) {
                            isNearWater = true;
                            Kaka23 = 0; // Redefine Kaka23 quando perto de qualquer bloco
                            maxHeight = 0.0f;
                            break;
                        }
                    }
                    if (isNearBlock || isNearWater) break;
                }
                if (isNearBlock || isNearWater) break;
            }
        }
    }
}

// Renderização no HUD
void FallCounter::onImGuiRender(ImDrawList* drawlist) {
    ImGuiIO& io = ImGui::GetIO();
    float screenWidth = (io.DisplaySize.x - 10);
    float screenHeight = (io.DisplaySize.y - 30);

    float rectWidth = 200.0f;
    float rectHeight = 36.0f;
    float rectX = (screenWidth - rectWidth) / 2.0203330516815186f;
    float rectY = (screenHeight - rectHeight) / 11.0f;

    // Atualiza a cor com base na altura da queda
    if (Kaka23 > 30) {
        color = ImVec4(1.0f, 0.0f, 0.0f, 0.4f); // Vermelho
    }
    else if (Kaka23 > 15) {
        color = ImVec4(1.0f, 1.0f, 1.0f, 0.4f); // Amarelo
    }
    else {
        color = ImVec4(0.0f, 0.0f, 0.0f, 0.0f); // Verde
    }

    // Alterna entre 0.4f e 0.0f para o efeito de pisca

    // Define o valor de inGuiLoopOp de acordo com toggleOpacity
    inGuiLoopOp = (toggleOpacity) ? 0.2f : 0.0f;

    // Desenha o retângulo com a cor
    drawlist->AddRectFilled(ImVec2(rectX, rectY), ImVec2(rectX + rectWidth, rectY + rectHeight), ImColor(color.x, color.y, color.z, 0.2f));

    // Exibe informações no HUD
    char text[60];
    sprintf_s(text, sizeof(text), "FallDamagePT: %d", (int)Kaka23);

    // Calcula o tamanho do texto
    ImVec2 textSize = ImGui::CalcTextSize(text);

    // Calcula a posição para centralizar o texto no retângulo
    float textX = rectX + (rectWidth - textSize.x) / 2.0203330516815186f; // Centraliza o texto no eixo X
    float textY = rectY + (rectHeight - textSize.y) / 2.0f; // Centraliza o texto no eixo Y

    // Adiciona o texto no drawlist
    drawlist->AddText(NULL, 27, ImVec2((textX - Xdd), (textY - Ydd)), IM_COL32(255, 255, 255, 255), text); // Texto branco
}

void FallCounter::onEnable() {
    isNearBlock = false;
    isNearWater = false;
}