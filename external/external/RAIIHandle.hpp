#pragma once

#include <Windows.h>

namespace Utils {
	class RAIIHandle {
	public:
		RAIIHandle( );
		RAIIHandle( HANDLE handle );
		~RAIIHandle( );

		RAIIHandle( const RAIIHandle& other ) = delete;
		RAIIHandle& operator=( const RAIIHandle& other ) = delete;

		RAIIHandle( RAIIHandle&& other );
		RAIIHandle& operator=( RAIIHandle&& other );

	public:
		const HANDLE Get( ) const;
		const HANDLE operator()( ) const;

	private:
		HANDLE handle;
	};
}