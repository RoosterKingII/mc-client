#pragma once
#include "../Module.h"

class Emote : public Module {
private:
    static inline int delay = 0;
    static inline int tickDelay = 0;
    static inline int currentEmoteIndex = 0;
    static inline const std::vector<std::string> emoteUUIDs =  {
"97bf992c-a10d-45d1-a730-219d5614822f",
    "e4278b7d-4405-522d-2bd3-cc2a8f9ad2a8",
    "18891e6c-bb3d-47f6-bc15-265605d86525",
    "2bbea8bc-bae7-6fba-a42e-19403d9516a5",
    "4e3413af-86a1-1ea6-6537-c50f4e332f94",
    "c2a47805-c792-4882-a56d-17c80b6c57a8",
    "2c2183c9-d8a2-aacd-9ad6-cdb260d7fc33",
    "2645be4e-c691-5072-a583-cb2acf397abd",
    "2b01a3b6-a86f-dd83-6c53-1c2f89b9e18f",
    "1a321afa-7087-e803-8513-6c79a50929d7",
    "e115b8af-b508-06c0-c4a9-fe540bf2fd5b",
    "d1d6eaef-c80f-e289-7770-5c1df04cf14f",
    "b1997ba4-67c2-4126-8b83-6970ccf05a96",
    "fdc8a22f-2dd6-f6ba-8155-74455ece3f51",
    "0d090971-6b6d-2c69-6461-2cb1ab6cc86b",
    "2d60f1e1-2261-3b47-aeaf-8792021a06bc",
    "3f7db11c-9960-e5fa-5374-28e11bf483f8",
    "95c74c13-88f8-3606-d9ad-78a91015a557",
    "211aabd3-39ac-f3eb-1417-724626276482",
    "d9a3800d-e6b7-86e0-0187-f10c5f5d3651",
    "e7bb3d8c-811d-442a-bfc5-775556d05d7a",
    "009f3f8d-0937-b9f3-6577-9bc1c8b53889",
    "c08c6523-0915-95af-e718-ecbfeba55472",
    "a43b566a-77da-2b2b-e016-e2e82393041e",
    "89539297-3a49-dae3-894e-9731b7ddaf4e",
    "07f49bd8-aff9-eae3-7735-6e14dc4836ba",
    "a0b1a9f2-0ea4-48d0-8641-ada998beee4d",
    "a92a58d8-8a86-61bb-3d59-4ad944252949",
    "61914613-98ee-65ce-7816-b8e076720f05",
    "d1cc0092-92de-69ad-e3ef-6755538d8ab2",
    "e17c1e65-1cd0-1e9c-4e96-55587facea56",
    "2cfb52df-ef9f-4976-b1cc-fac271bcc6da",
    "b1bb260f-5202-47b6-a9c6-550bcaae7671",
    "c9ec1072-0824-8008-6550-a8bcf5a8a945",
    "0e2d45c0-2e2f-1818-f311-c8dbda30c081",
    "dd8da8eb-7df0-532a-fcc1-aa2ca428daa9"
    "f8faa7aa-d9c7-d2ff-e001-26f1db24e6ff",
    "1b923008-d5e1-5edf-76e0-b512a30fbd72",
    "347f0a8b-5b97-675b-34cc-b24ba262a77f",
    "732998c5-2758-e8b6-bee2-2889925a581e",
    "0f79b091-8026-059a-5b9f-0d57da756bec",
    "346ae49b-43b6-2482-8045-0baf4d4725c3",
    "ffa4fb7c-0bc7-4b82-a207-fee3def0c8c6",
    "f3860b0e-fb05-5b8b-863b-ba7693afce11",
    "4699d33f-630a-ee9d-bda2-ca6a8fe4df5a",
    "ba52157f-4dd9-650a-a8f8-ab0fe5b66756",
    "531dc549-7c27-f3c2-7438-8af2f0c6deb9",
    "c4aa9ca0-abcc-7d0c-01c7-76a7e4d4840e",
    "73cead3f-0c5a-42ce-dc8b-8f8dab394204",
    "0e708eb2-74db-3dc8-cb0a-fd320608c8ab",
    "f4ebe54a-fc1e-7f65-dc3e-68e51d2d7ca3",
    "7804548a-79b1-0eb7-1d51-229339c033b7",
    "e35b7cb9-714e-92c6-9131-d777e9c0ebbe",
    "c63d95ad-cf5f-2216-b793-485824adde4d",
    "a4747be9-48d4-e203-d2cf-22bef1e47e10", "1e137be8-8c5a-c880-e65e-8e98548d5b51", "9ee11247-49a1-a816-2775-203727adc879", "b59bd4e3-7695-02b4-30d3-def25971f2bc", "adba2129-80ab-20b3-950c-d6eca27461e7", "68395b7e-f35b-ab76-40af-364178bd56fe", "d2e66d12-cd7a-2d65-1fb2-6048115e07f6", "2d19ab9a-d760-5be3-d75e-93a986c17e1a", "6373d5e3-0fd0-6253-9c99-3a43965e558e", "f2d004ab-6d62-4c21-8517-f7ef03d9bb26", "9bcc476a-301e-3c1c-a1c4-0941d3662dd3", "eea1bdca-68c5-a04a-c5d3-45e4ce92c2be", "97978dbc-9cda-64f7-e70e-9724bef3cc17", "573fdad9-e33a-6766-39e6-41775fb41785", "ea1a6163-5479-4377-9669-1b9b8ff2fb93", "fa68e7a1-5d12-3757-89b9-f1888fcbb0f4", "6a7bef6e-a1e1-4232-391f-55b48ce10372", "e301938b-0c4a-6630-d0a8-b7dde72ccf5c", "5f0f79b3-1205-2b21-4acc-ec24e3618a94", "ee1d7446-2bd4-5b92-95cc-3185a5509de0", "745dab6a-8fda-6c23-ea19-1413ca64a9f1", "8465bd2e-2a59-ba73-70fc-b88cf9d8727c", "a152a09e-b889-0a08-3992-3f2a087bf75c", "5ce324a4-1887-8f07-9d0b-5c0a3e860151", "786fec7c-694b-841a-0a03-d5b9668eca9d", "98f7626d-fa33-08a7-52bc-a4d07acc8b4a", "b6b0b5fe-aa5c-8ba2-3924-55fe16d5f3f8", "d96807f2-f488-3270-7deb-d81715fe620b", "1aed2db4-38a4-488f-ba72-29973f2b736e", "ff06f4b2-a595-49e9-b77e-e2ea9bbd74ff", "fbb4778a-164f-abbb-f24a-a633c936cc10", "36cbb754-1ad4-a0f2-8bf7-210279fdc530"
};

public:
    Emote();
    void onNormalTick(Actor* actor);
    //static void onRender(ImDrawList* drawlist);
};

