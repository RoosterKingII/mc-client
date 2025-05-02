#include "Spammer.h"

Spammer::Spammer() : Module("Spammer", "Spams messages in chat", Category::MISC) {
    addSlider("Delay", "Delay in seconds between messages", ValueType::INT_T, &delay, 1, 20);
    addSlider("Length", "Random string length for bypass", ValueType::INT_T, &length, 1, 20);
    addEnumSetting("Mode", "Spamming mode", { "Normal", "Bypass" }, &mode);
}

static inline std::string randomString(const int size) {
    std::string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
    std::random_device rd;
    std::mt19937 generator(rd());
    std::shuffle(str.begin(), str.end(), generator);
    return str.substr(0, size);
}

void sendMessage(std::string str) {      // Crear el paquete
    auto* pkt = reinterpret_cast<TextPacket*>(packet.get());

            mc.getClientInstance()->loopbackPacketSender->sendToServer(pkt);
        
    
}
void Spammer::onNormalTick(Actor* actor) {
    static uint64_t lastMessageTime = 0;
    static int messageIndex = 0;

    // Obtener tiempo actual
    uint64_t currentTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

    // Verificar si ha pasado suficiente tiempo
    if (currentTime - lastMessageTime >= (delay * 1000)) {
        std::string spamMessage = messages[messageIndex];

        // Añadir string aleatorio en modo bypass
        if (mode == 1) {
            spamMessage += " | " + randomString(length);
        }

        sendMessage(spamMessage);

        // Actualizar índice y tiempo
        messageIndex = (messageIndex + 1) % messages.size();
        lastMessageTime = currentTime;
    }
}
