#include "ChatSuffix.h"
bool green = true;
int mode2 = 0;
ChatSuffix::ChatSuffix() : Module("ChatSuffix", "Add a suffix to your chat messages", Category::MISC) {
	addEnumSetting("Mode", "NULL", { "Desmariquead@ si lo lees", "EZZZ", "horionclien", "NukeMobile", "biba Dios", "Put@ Client", "el que se mueva es gei", "si lees esto eres maric@", " Me cojo al mudo", "Me cojo al que hable"}, &mode2);
}

void ChatSuffix::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (packet->getId() == PacketID::Text) {
		TextPacket* textPacket = reinterpret_cast<TextPacket*>(packet);
		std::string message = textPacket->message;
		if (green)
			textPacket->message = std::string(">" + message);
		switch (mode2) {
		case 0:
			textPacket->message = std::string(message + std::string(" | Desmariquead@ si lo lees"));
			break;
		case 1:
			textPacket->message = std::string(message + std::string(" | EZZZ"));
			break;
		case 2:
			textPacket->message = std::string(message + std::string(" | horionclien"));
			break;
		case 3:
			textPacket->message = std::string(message + std::string(" | NukeMobile"));
			break;
		case 4:
			textPacket->message = std::string(message + std::string(" | biba Dios"));
			break;
		case 5:
			textPacket->message = std::string(message + std::string(" | Put@ Client"));
			break;
		case 6:
			textPacket->message = std::string(message + std::string(" | el que se mueva es gei"));
			break;
		case 7:
			textPacket->message = std::string(message + std::string(" | si lees esto eres maric@"));
			break;
		case 8:
			textPacket->message = std::string(message + std::string(" | Me cojo al mudo"));
			break;
		case 9:
			textPacket->message = std::string(message + std::string(" | Me cojo al que hable"));
			break;
		}
	}
}
