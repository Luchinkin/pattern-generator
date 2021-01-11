#include "maker.hpp"
#include <vector>
#include <string>
#include <array>
#include <iterator>

#include "instruction.hpp"

static constexpr auto MAX_PROCESS_INSTRUCTIONS = 128u;

static duint find_pattern( duint start, duint end, const std::string& pattern )
{
    return Script::Pattern::FindMem( start, end - start, pattern.c_str() );
}

void maker::work()
{
    const auto [module_base, module_size] = utils::get_current_module_info();
    const auto module_end = module_base + module_size;
    const auto address = utils::get_current_selected_addr();

    std::vector<std::uint8_t> gbytes;
    std::vector<bool> gmask;
    duint offset = 0;

    std::string final_double_wildcard_pattern;
    bool found = false;

    for ( std::size_t i = 0; i < MAX_PROCESS_INSTRUCTIONS; ++i )
    {
        const auto inst = instruction( address + offset );

        for ( int j = 0; j < inst.size(); ++j )
            gbytes.push_back( inst.get( j ) );

        std::copy( inst.mask().begin(), inst.mask().end(), std::back_inserter( gmask ) );

        const auto test_pattern = utils::generate_pattern_str( gbytes, gmask, true );

        if ( !find_pattern( module_base, address, test_pattern )
             && !find_pattern( address + gbytes.size(), module_end, test_pattern ) )
        {
            final_double_wildcard_pattern = test_pattern;
            found = true;
            break;
        }

        offset += inst.size();
    }

    if ( !found )
    {
        dprintf( "Couldn't found pattern for this place!\n" );
    }
    else
    {
        const auto ida_pattern = utils::generate_pattern_str( gbytes, gmask, false );

        dprintf( "Found unique pattern for this place:\nx64dbg:%s\nIDA:%s\n",
                 final_double_wildcard_pattern.c_str(), ida_pattern.c_str() );
    }

}
