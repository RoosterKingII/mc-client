#include "ChatSuffix.h"

ChatSuffix::ChatSuffix() : Module("ChatSuffix", "Add a random suffix to your chat messages", Category::MISC) {
    std::random_device rd;
    rng = std::mt19937(rd());

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
          " | Put@ Horion"
          " | el que se mueva es gei",
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
    };
}

std::string ChatSuffix::getRandomSuffix() {
    std::uniform_int_distribution<int> dist(0, suffixes.size() - 1);
    return suffixes[dist(rng)];
}

void ChatSuffix::onSendPacket(Packet* packet, bool& shouldCancel) {
    if (packet->getId() == PacketID::Text) {
        TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
        std::string message = textPacket->message.getText(); 

        if (green) {
            message = ">" + message;
        }

        message += getRandomSuffix();

        textPacket->message = TextHolder(message); 
    }
}
