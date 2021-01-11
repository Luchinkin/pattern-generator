#pragma once

// Plugin information
#ifndef PLUGIN_NAME
#include "pluginconfig.h"
#else
#define PLUGIN_NAME "UnnamedPlugin"
#endif // PLUGIN_NAME
#define PLUGIN_VERSION 1

#include "x64dbg_sdk/pluginsdk/bridgemain.h"
#include "x64dbg_sdk/pluginsdk/_plugins.h"

#include "x64dbg_sdk/pluginsdk/_scriptapi_argument.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_assembler.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_bookmark.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_comment.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_debug.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_flag.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_function.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_gui.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_label.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_memory.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_misc.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_module.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_pattern.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_register.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_stack.h"
#include "x64dbg_sdk/pluginsdk/_scriptapi_symbol.h"

#include "x64dbg_sdk/pluginsdk/DeviceNameResolver/DeviceNameResolver.h"
#include "x64dbg_sdk/pluginsdk/jansson/jansson.h"
#include "x64dbg_sdk/pluginsdk/lz4/lz4file.h"
#include "x64dbg_sdk/pluginsdk/TitanEngine/TitanEngine.h"
#include "x64dbg_sdk/pluginsdk/XEDParse/XEDParse.h"

#ifdef _WIN64
#pragma comment(lib, "x64dbg_sdk/pluginsdk/x64dbg.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/x64bridge.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/DeviceNameResolver/DeviceNameResolver_x64.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/jansson/jansson_x64.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/lz4/lz4_x64.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/TitanEngine/TitanEngine_x64.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/XEDParse/XEDParse_x64.lib")
#else
#pragma comment(lib, "x64dbg_sdk/pluginsdk/x32dbg.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/x32bridge.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/DeviceNameResolver/DeviceNameResolver_x86.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/jansson/jansson_x86.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/lz4/lz4_x86.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/TitanEngine/TitanEngine_x86.lib")
#pragma comment(lib, "x64dbg_sdk/pluginsdk/XEDParse/XEDParse_x86.lib")
#endif //_WIN64

#define Cmd(x) DbgCmdExecDirect(x)
#define Eval(x) DbgValFromString(x)
#define dprintf(x, ...) _plugin_logprintf("[" PLUGIN_NAME "] " x, __VA_ARGS__)
#define dputs(x) _plugin_logputs("[" PLUGIN_NAME "] " x)
#define PLUG_EXPORT extern "C" __declspec(dllexport)
