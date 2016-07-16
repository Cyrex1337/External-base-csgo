#pragma once

#include "ProcessManager.hpp"
#include "OffsetManager.hpp"

namespace Utils {
	class MemoryManager : public ProcessManager {
	public:
		MemoryManager( const std::string& processName );
		~MemoryManager( ) = default;

	public:
		template <typename Type>
		Type Read( Type* memory ) {
			Type buffer;
			ReadProcessMemory( processHandle( ), memory, &buffer, sizeof( Type ), nullptr );
			return buffer;
		}

		template <typename Type>
		bool Read( Type* memory, Type& buffer ) {
			return !!ReadProcessMemory( processHandle( ), memory, &buffer, sizeof( Type ), nullptr );
		}

		bool readBytes( char* memory, char* buffer, std::size_t length ) {
			return !!ReadProcessMemory( processHandle( ), memory, buffer, length, nullptr );
		}

		template <typename Type>
		bool Write( Type* memory, Type value ) {
			return !!WriteProcessMemory( processHandle( ), memory, &value, sizeof( Type ), nullptr );
		}

		bool writeBytes( char* memory, char* buffer, std::size_t length ) {
			return !!WriteProcessMemory( processHandle( ), memory, buffer, length, nullptr );
		}
	};
}