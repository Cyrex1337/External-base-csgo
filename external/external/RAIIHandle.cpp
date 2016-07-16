#include "RAIIHandle.hpp"

Utils::RAIIHandle::RAIIHandle( ) {
	handle = INVALID_HANDLE_VALUE;
}

Utils::RAIIHandle::RAIIHandle( const HANDLE handle ) {
	this->handle = handle;
}

Utils::RAIIHandle::~RAIIHandle( ) {
	if ( handle != INVALID_HANDLE_VALUE )
		CloseHandle( handle );
}

Utils::RAIIHandle::RAIIHandle( Utils::RAIIHandle&& other ) {
	handle = other.handle;
	other.handle = INVALID_HANDLE_VALUE;
}

Utils::RAIIHandle& Utils::RAIIHandle::operator=( Utils::RAIIHandle&& other ) {
	if ( this != &other ) {
		if ( handle != INVALID_HANDLE_VALUE )
			CloseHandle( handle );

		handle = other.handle;
		other.handle = INVALID_HANDLE_VALUE;
	}

	return *this;
}

const HANDLE Utils::RAIIHandle::Get( ) const {
	return handle;
}

const HANDLE Utils::RAIIHandle::operator()( ) const {
	return handle;
}