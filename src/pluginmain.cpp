#include "pluginmain.h"

#include "menu.hpp"

PLUG_EXPORT bool pluginit( PLUG_INITSTRUCT* initStruct )
{
    initStruct->pluginVersion = PLUGIN_VERSION;
    initStruct->sdkVersion = PLUG_SDKVERSION;
    strncpy_s( initStruct->pluginName, PLUGIN_NAME, _TRUNCATE );

    _plugin_registercallback( initStruct->pluginHandle, CB_MENUENTRY, (CBPLUGIN)menu::callback );

    return true;
}

PLUG_EXPORT bool plugstop()
{

    return true;
}

PLUG_EXPORT void plugsetup( PLUG_SETUPSTRUCT* setupStruct )
{
    menu::setup( setupStruct );
}

