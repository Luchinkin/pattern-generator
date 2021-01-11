#include "utils.hpp"
#include <sstream>
#include <iomanip>

duint utils::get_current_selected_addr()
{
	if ( !DbgIsDebugging() )
	{
		dprintf( "DbgIsDebugging failed!\n" );
		return 0;
	}

	SELECTIONDATA data;
	if ( !GuiSelectionGet( GUI_DISASSEMBLY, &data ) )
	{
		dprintf( "GuiSelectionGet failed!\n" );
		return 0;
	}

	return data.start;
}

module_info_t utils::get_current_module_info()
{
	module_info_t result;

	const auto current_selected = get_current_selected_addr();
	if ( !current_selected )
	{
		dprintf( "get_current_selected_addr failed!\n" );
		return result;
	}


	result.start = DbgFunctions()->ModBaseFromAddr( current_selected );

	if ( result.start <= 0 )
	{
		dprintf( "Couldn't get module base addr for selected addr 0x%llX\n", current_selected );
		return result;
	}

	result.size = DbgFunctions()->ModSizeFromAddr( current_selected );
	if ( result.size <= 0 )
	{
		dprintf( "Couldn't get module size for selected addr 0x%llX\n", current_selected );
		return result;
	}

	return result;
}

std::string utils::generate_pattern_str( std::vector<std::uint8_t> bytes, std::vector<bool> mask, bool double_wildcard )
{
	std::stringstream stream;

#define FORMAT_NUMBER_TO_HEX \
std::hex << std::setfill('0') << std::uppercase << std::setw( 2 )

	for ( auto i = 0u; i < bytes.size(); ++i )
	{
		if ( mask[ i ] )
			stream << FORMAT_NUMBER_TO_HEX << static_cast<int>( bytes[ i ] ) << " ";
		else
		{
			if ( double_wildcard )
				stream << "?? ";
			else
				stream << "? ";
		}
	}

#undef FORMAT_NUMBER_TO_HEX

	auto result = stream.str();
	if ( !result.empty() )
		result.pop_back();
	return result;
}
