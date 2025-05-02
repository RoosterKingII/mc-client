// ChatSuffix.cpp
#include "ChatSuffix.h"

ChatSuffix::ChatSuffix() : Module("ChatSuffix", "Add a random suffix to your chat messages", Category::MISC) {
    addBoolCheck("Green", "Add a '>' prefix", &green);
    addBoolCheck("Random Suffix", "Random suffixes", &toxicSuffix);
    addBoolCheck("Client Tag", "SX1 Client tag", &clientSuffix);
    addBoolCheck("Random String", "NULL", &randomString);
    addSlider<int>("String Length", "Length of random string",ValueType::INT_T, &randomStringLength, 1, 32);

    std::random_device rd;
    rng = std::mt19937(rd());

    // Inicializar vector de sufijos (mantener los sufijos existentes)
    suffixes = {
     " | Desmariquead@ si lo lees",
     " | Desmariquead@ si lo ves",
     " | EZZZ",
     " | gg",
     " | LMAO",
     " | LUL",
     " | 1v1?",
     " | horionclien",
     " | NukeMobile",
     " | biba Dios",
     " | Put@ Client",
     " | Put@ Horion",
     " | el que se mueva es gei",
     " | pobres los que ban de diamante",
     " | me cojo a todos los malditos negros peruanos",
     " | el que lea es gei",
     " | el que hable es gei",
     " | el que respire es gei",
     " | el que exista es gei",
     " | el que juegue es gei",
     " | si lees esto eres marica",
     " | si lees esto eres gei",
     " | Me cojo al mudo",
     " | Me cojo al que hable",
     " | Me cojo al que lea",
     " | Me cojo al mundo",
     " | Me cojo a todos",
     " | MARIC@",
     " | GEI",
     " | Ḧitler client",
     " | El que lo lea es manco",
     " | Manco si lo lees",
    };
}
std::string ChatSuffix::generateRandomString(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<int> dist(0, charset.length() - 1);
    std::string result;
    for (int i = 0; i < length; i++) {
        result += charset[dist(rng)];
    }
    return result;
}

std::string ChatSuffix::getRandomSuffix() {
    std::uniform_int_distribution<int> dist(0, suffixes.size() - 1);
    return suffixes[dist(rng)];
}

void ChatSuffix::onSendPacket(Packet* packet, bool& shouldCancel) {
    if (packet->getId() == PacketID::Text) {
        TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
        std::string message = textPacket->message;

        // Agregar prefijo ">" si green está activado
        if (green) {
            message = ">" + message;
        }

        // Agregar sufijo según las opciones seleccionadas
        if (toxicSuffix) {
            message += getRandomSuffix();
        }

        if (clientSuffix) {
            message += " | SX1 Client";
        }

        if (randomString) {
            message += " | " + generateRandomString(randomStringLength);
        }

        // Actualizar el mensaje del paquete
        textPacket->message = message;
    }
}
