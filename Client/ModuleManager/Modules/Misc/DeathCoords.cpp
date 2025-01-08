#include "DeathCoords.h"
#include "../../../Client.h"

DeathCoordinates::DeathCoordinates() : Module("DeathCoordinates", "Displays your position when you die", Category::MISC) {
    messageDisplayed = false; // Baþlangýçta mesaj görüntülenmedi
}

void DeathCoordinates::onNormalTick(Actor* actor) {
    auto* player = mc.getLocalPlayer();
    if (player != nullptr) {
        if (!player->isAlive() && !messageDisplayed) {
            // Eðer oyuncu ölmüþse ve mesaj henüz gösterilmediyse
            auto position = player->getPosition(); // Pozisyonu al
            float x = position->x;
            float y = position->y;
            float z = position->z;

            // Mesajý oluþtur
            std::string message = "Your last death position was: X: " + std::to_string(x) +
                " Y: " + std::to_string(y) +
                " Z: " + std::to_string(z);

            // Mesajý ekrana yazdýr

            messageDisplayed = true; // Mesajýn görüntülendiðini iþaretle
        }
        else if (player->isAlive()) {
            // Eðer oyuncu yeniden doðmuþsa, mesaj görüntüleme bayraðýný sýfýrla
            messageDisplayed = false;
        }
    }
}