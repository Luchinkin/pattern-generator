#pragma once
#include "utils.hpp"

namespace menu
{
	void setup( PLUG_SETUPSTRUCT* setupStruct );
	void callback( CBTYPE Type, PLUG_CB_MENUENTRY* Info );
}
