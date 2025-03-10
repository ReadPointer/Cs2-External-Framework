#pragma once

#include <Windows.h>

class EntityCaching {
public:
	uintptr_t clientDLL;

	void startCachingLoop(uintptr_t client);
private:
	void FilterEntities();
}; inline EntityCaching reader;