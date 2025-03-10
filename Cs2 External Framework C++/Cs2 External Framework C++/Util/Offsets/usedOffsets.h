#pragma once

#include "offsets.hpp"
#include "client_dll.hpp"

using namespace cs2_dumper;

namespace usedOffsets {
	inline std::ptrdiff_t dwEntityList = offsets::client_dll::dwEntityList;
	inline std::ptrdiff_t dwLocalPlayerController = offsets::client_dll::dwLocalPlayerController;
	inline std::ptrdiff_t dwLocalPlayerPawn = offsets::client_dll::dwLocalPlayerPawn;

	namespace client_dll {
		inline std::ptrdiff_t m_hPawn = schemas::client_dll::CBasePlayerController::m_hPawn;
		inline std::ptrdiff_t m_iHealth = schemas::client_dll::C_BaseEntity::m_iHealth;
	}
}