#include "Playercaching.h"
#include "../Util/Memory/memory.h"
#include "../Util/Offsets/usedOffsets.h"
#include "../Util/Console/Logger.h"

void EntityCaching::startCachingLoop(uintptr_t client) {
	this->clientDLL = client;

	Sleep(500);

	for (;;) {
		this->FilterEntities();

		Sleep(600);
	}
}

void EntityCaching::FilterEntities() {
	auto entityList = mem->Read<uintptr_t>(clientDLL + usedOffsets::dwEntityList);

	for (int i = 1; i < 64; i++) {
		uintptr_t listEntry1 = mem->Read<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);
		if (!listEntry1) continue;

		uintptr_t controller = mem->Read<uintptr_t>(listEntry1 + 120 * (i & 0x1FF));
		if (!controller) continue;

		uintptr_t pawnHandle = mem->Read<uintptr_t>(controller + usedOffsets::client_dll::m_hPawn);
		if (!pawnHandle) continue;
		
		uintptr_t listEntry2 = mem->Read<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
		if (!listEntry2) continue;

		uintptr_t playerPawn = mem->Read<uintptr_t>(listEntry2 + 120 * (pawnHandle & 0x1FF));
		if (!playerPawn) continue;

		int pawnHealth = mem->Read<int>(playerPawn + usedOffsets::client_dll::m_iHealth);

		Logger::Log("[INFO] Player ", i, " Health = ", pawnHealth);
	}
}