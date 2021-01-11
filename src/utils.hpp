#pragma once

#include "pluginmain.h"

#include <cstdint>
#include <vector>

struct module_info_t
{
	duint start = 0x0;
	duint size = 0x0;
};

namespace utils
{
	duint get_current_selected_addr();
	module_info_t get_current_module_info();
	std::string generate_pattern_str( std::vector<std::uint8_t> bytes, std::vector<bool> mask, bool double_wildcard );
}

enum EMENU
{
	MENU_PATTERN = 0u,
};
