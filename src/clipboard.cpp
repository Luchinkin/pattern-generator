#include "clipboard.hpp"
#include <Windows.h>

struct clipboard_t
{
	clipboard_t( HWND hwnd )
	{
		OpenClipboard( hwnd );
		EmptyClipboard();
	}
	~clipboard_t()
	{
		CloseClipboard();
	}
};

struct global_alloc_t
{
	global_alloc_t( std::size_t size )
	{
		handle = GlobalAlloc( GMEM_MOVEABLE, size );
		if ( !handle )
			__debugbreak();
	}

	const HGLOBAL& get() { return handle; }

	void write( const void* ptr, std::size_t size )
	{
		memcpy( GlobalLock( handle ), ptr, size );
		GlobalUnlock( handle );
	}

	~global_alloc_t()
	{
		GlobalFree( handle );
	}

	HGLOBAL handle = NULL;
};

void save_to_clipboard( const std::string& str )
{
	const auto hDesktop = GetDesktopWindow();

	clipboard_t clipboard( hDesktop );

	global_alloc_t alloc_mem( str.size() + 1u );

	alloc_mem.write( str.data(), str.size() + 1u );

	SetClipboardData( CF_TEXT, alloc_mem.get() );
}
