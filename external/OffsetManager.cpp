#include "OffsetManager.hpp"

#define INRANGE(x,a,b)		(x >= a && x <= b) 
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

namespace Game {
	bool OffsetManager::dataCompare( byte* data, byte* byteMask, const char* hitMask ) {
		for ( ; *hitMask; ++data, ++byteMask, ++hitMask ) {
			if ( *data != *byteMask && *hitMask == 'x' )
				return false;
		}

		return *hitMask == '\0';
	}

	std::uintptr_t OffsetManager::findPattern( const HANDLE procHandle, const std::string& moduleName, byte* byteMask, const char* hitMask ) {
		MODULEINFO modInfo;
		GetModuleInformation( procHandle, GetModuleHandleA( moduleName.c_str( ) ), &modInfo, sizeof( MODULEINFO ) );

		std::unique_ptr<byte> memorySnapshot = std::unique_ptr<byte>( new byte[modInfo.SizeOfImage] );
		byte* capturedMemory = memorySnapshot.get( );

		for ( unsigned int i = 0; i < modInfo.SizeOfImage; ++i ) {
			if ( dataCompare( capturedMemory + i, byteMask, hitMask ) ) {
				return reinterpret_cast< std::uintptr_t >( modInfo.lpBaseOfDll ) + i;
			}
		}

		return 0;
	}

	// credits: learn_more
	std::uintptr_t OffsetManager::findPatternString( const HANDLE procHandle, const std::string& moduleName, const char* pattern ) {
		MODULEINFO modInfo;
		GetModuleInformation( procHandle, GetModuleHandleA( moduleName.c_str( ) ), &modInfo, sizeof( MODULEINFO ) );

		std::unique_ptr<byte> memorySnapshot = std::unique_ptr<byte>( new byte[modInfo.SizeOfImage] );
		byte* capturedMemory = memorySnapshot.get( );

		int returnedIndex = 0;

		const unsigned char* pat = reinterpret_cast< const unsigned char* >( pattern );
		int firstMatch = 0;
		for ( unsigned int i = 0; i < modInfo.SizeOfImage; ++i ) {
			if ( *pat == '\?' || capturedMemory[i] == getByte( pat ) ) {
				if ( !firstMatch ) {
					firstMatch = i;
				}
				pat += ( *( PWORD )pat == ( WORD )'\?\?' || *pat != '\?' ) ? 3 : 2;
				if ( !*pat ) {
					returnedIndex = firstMatch;
					break;
				}
			} else if ( firstMatch ) {
				i = firstMatch;
				pat = reinterpret_cast< const unsigned char* >( pattern );
				firstMatch = 0;
			}
		}

		return reinterpret_cast< std::uintptr_t >( modInfo.lpBaseOfDll ) + returnedIndex;
	}
}