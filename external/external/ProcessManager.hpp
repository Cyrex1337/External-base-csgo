#pragma once

#include "RAIIHandle.hpp"

#include <TlHelp32.h>
#include <string>
#include <memory>
#include <Psapi.h>

namespace Utils {
	class ModuleManager {
	public:
		ModuleManager( const RAIIHandle& processHandle );
		~ModuleManager( ) = default;

	public:
		unsigned int Base( const std::string& moduleName );
		unsigned int Size( const std::string& moduleName );

	private:
		const RAIIHandle* processHandle;
	};

	class ProcessManager {
	public:
		ProcessManager( const std::string& processName, bool forceAppear = true, unsigned int processID = 0 );
		virtual ~ProcessManager( );

	public:
		unsigned int Base( ) const;
		unsigned int Size( ) const;
		bool isActive( );

	private:
		bool searchProcess( bool forceAppear );
		unsigned int getProcessID( ) const;

	protected:
		std::string processName;
		RAIIHandle processHandle;
		std::unique_ptr<ModuleManager> moduleManager;
		unsigned int processID;
	};
}

extern std::unique_ptr<Utils::ProcessManager> procManager;