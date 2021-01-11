#include "menu.hpp"
#include "maker.hpp"

void menu::setup( PLUG_SETUPSTRUCT* setupStruct )
{
	auto handle = setupStruct->hMenu;
	_plugin_menuaddentry( handle, MENU_PATTERN, "&Generate" );
}

void menu::callback( CBTYPE Type, PLUG_CB_MENUENTRY* Info )
{
	switch ( Info->hEntry )
	{
		case MENU_PATTERN:
			maker::work();
			break;
		default:
			break;
	}
}
