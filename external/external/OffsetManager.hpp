#pragma once

#include <cstdint>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <memory>

namespace Game {
	class OffsetManager {
		// TODO:
		// - implement netvar tree search
		static bool dataCompare( byte* data, byte* byteMask, const char* hitMask );
		static std::uintptr_t findPattern( const HANDLE procHandle, const std::string& moduleName, byte* byteMask, const char* hitMask );
		static std::uintptr_t findPatternString( const HANDLE procHandle, const std::string& moduleName, const char* pattern );
	};
}
