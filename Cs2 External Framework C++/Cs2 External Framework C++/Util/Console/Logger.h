#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cstdio>
#include <iostream>
#include <sstream>

namespace Logger 
{
	inline bool shouldLog = true;

	inline void CreateConsole() {
		if (!shouldLog) return;

		if (AllocConsole()) {
			FILE* fp;
			freopen_s(&fp, "CONOUT$", "w", stdout);
			freopen_s(&fp, "CONOUT$", "w", stderr);
			freopen_s(&fp, "CONIN$", "r", stdin);

			std::cout << "Console allocated successfully!" << std::endl;
		}
		else {
			std::cerr << "Failed to allocate console." << std::endl;
		}
	}

	template <typename... Args>
	inline void Log(const std::string& message, Args... args) {
		if (!shouldLog) return;
		std::ostringstream oss;
		(oss << ... << args);
		std::cout << message << oss.str() << std::endl;
	}
}