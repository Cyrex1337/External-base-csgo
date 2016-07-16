#include "ProcessManager.hpp"

namespace Utils {
	ModuleManager::ModuleManager( const RAIIHandle& processHandle ) {
		this->processHandle = &processHandle;
	}

	unsigned int ModuleManager::Base( const std::string& moduleName ) {
		MODULEINFO modInfo;
		GetModuleInformation( processHandle->Get( ), GetModuleHandleA( moduleName.c_str( ) ), &modInfo, sizeof( MODULEINFO ) );
		return reinterpret_cast< unsigned int >( modInfo.lpBaseOfDll );
	}

	unsigned int ModuleManager::Size( const std::string& moduleName ) {
		MODULEINFO modInfo;
		GetModuleInformation( processHandle->Get( ), GetModuleHandleA( moduleName.c_str( ) ), &modInfo, sizeof( MODULEINFO ) );
		return static_cast< unsigned int >( modInfo.SizeOfImage );
	}

	ProcessManager::ProcessManager( const std::string& processName, bool forceAppear, unsigned int processID ) {
		this->processName = processName;

		if ( processID != 0 ) {
			processHandle = OpenProcess( PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processID );
			moduleManager = std::make_unique<ModuleManager>( processHandle );
			this->processID = processID;
		} else if ( searchProcess( forceAppear ) ) {
			moduleManager = std::make_unique<ModuleManager>( processHandle );
			this->processID = GetProcessId( processHandle( ) );
		}
	}

	ProcessManager::~ProcessManager( ) {

	}

	unsigned int ProcessManager::Base( ) const {
		MODULEINFO modInfo;
		GetModuleInformation( processHandle( ), nullptr, &modInfo, sizeof( MODULEINFO ) );
		return reinterpret_cast< unsigned int >( modInfo.lpBaseOfDll );
	}

	unsigned int ProcessManager::Size( ) const {
		MODULEINFO modInfo;
		GetModuleInformation( processHandle( ), nullptr, &modInfo, sizeof( MODULEINFO ) );
		return static_cast< unsigned int >( modInfo.SizeOfImage );
	}

	bool ProcessManager::isActive( ) {
		return searchProcess( false );
	}

	bool ProcessManager::searchProcess( bool forceAppear ) {
		if ( forceAppear ) {
			std::size_t strSize = 0;
			std::unique_ptr<char> buffer;

			while ( true ) {
				RAIIHandle safeSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
				PROCESSENTRY32 pe;
				pe.dwSize = sizeof( PPROCESSENTRY32 );

				if ( !safeSnapshot( ) ) {
					continue;
				}

				if ( Process32First( safeSnapshot.Get( ), &pe ) ) {
					do {
						strSize = wcslen( pe.szExeFile );
						buffer = std::unique_ptr<char>( new char[strSize + 1] );
						wcstombs_s( nullptr, buffer.get( ), strSize + 1, pe.szExeFile, strSize );

						if ( strcmp( buffer.get( ), processName.c_str( ) ) == 0 ) {
							processHandle = OpenProcess( PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processID );
							return true;
						}
					} while ( Process32Next( safeSnapshot.Get( ), &pe ) );
				}
			}
		} else {
			std::size_t strSize = 0;
			std::unique_ptr<char> buffer;

			RAIIHandle safeSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
			PROCESSENTRY32 pe;
			pe.dwSize = sizeof( PPROCESSENTRY32 );

			if ( !safeSnapshot( ) ) {
				return false;
			}

			if ( Process32First( safeSnapshot.Get( ), &pe ) ) {
				do {
					strSize = wcslen( pe.szExeFile );
					buffer = std::unique_ptr<char>( new char[strSize + 1] );
					wcstombs_s( nullptr, buffer.get( ), strSize + 1, pe.szExeFile, strSize );

					if ( strcmp( buffer.get( ), processName.c_str( ) ) == 0 ) {
						processHandle = OpenProcess( PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processID );
						return true;
					}
				} while ( Process32Next( safeSnapshot.Get( ), &pe ) );
			}
		}

		return false;
	}

	unsigned int ProcessManager::getProcessID( ) const {
		return processID;
	}
}