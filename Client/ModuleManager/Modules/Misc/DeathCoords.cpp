#include "DeathCoords.h"
#include "../../../Client.h"

DeathCoordinates::DeathCoordinates() : Module("DeathCoordinates", "Displays your position when you die", Category::MISC) {
    messageDisplayed = false; // Ba�lang��ta mesaj g�r�nt�lenmedi
}

void DeathCoordinates::onNormalTick(Actor* actor) {
    auto* player = mc.getLocalPlayer();
    if (player != nullptr) {
        if (!player->isAlive() && !messageDisplayed) {
            // E�er oyuncu �lm��se ve mesaj hen�z g�sterilmediyse
            auto position = player->getPosition(); // Pozisyonu al
            float x = position->x;
            float y = position->y;
            float z = position->z;

            // Mesaj� olu�tur
            std::string message = "Your last death position was: X: " + std::to_string(x) +
                " Y: " + std::to_string(y) +
                " Z: " + std::to_string(z);

            // Mesaj� ekrana yazd�r

            messageDisplayed = true; // Mesaj�n g�r�nt�lendi�ini i�aretle
        }
        else if (player->isAlive()) {
            // E�er oyuncu yeniden do�mu�sa, mesaj g�r�nt�leme bayra��n� s�f�rla
            messageDisplayed = false;
        }
    }
}