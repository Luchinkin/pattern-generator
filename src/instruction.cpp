#include "instruction.hpp"

#define ASM_OPCODE_CALL 0xE8
#define ASM_OPCODE_JMP	0xE9


instruction::instruction( duint addr )
{
	ZeroMemory( &m_info, sizeof BASIC_INSTRUCTION_INFO );

	DbgDisasmFastAt( addr, &m_info );
	m_address = addr;

	analyze();
}

std::size_t instruction::size() const
{
	return m_info.size;
}

std::uint8_t instruction::get( std::size_t idx ) const
{
	return m_bytes[ idx ];
}

std::string instruction::str() const
{
	return std::string( m_info.instruction );
}

const std::vector<bool>& instruction::mask() const
{
	return m_mask;
}

template<class T>
static bool apply_mask_by_search( std::vector<std::uint8_t>& where_, int64_t search_val, std::vector<bool>* mask_ptr )
{
	const auto size_fix = ( sizeof( T ) - 1 );
	if ( size_fix >= where_.size() )
		return false;

	for ( auto i = 0u; i < where_.size() - size_fix; i++ )
	{
		if ( *reinterpret_cast<T*>( &where_[ i ] ) == static_cast<T>( search_val ) )
		{
			std::fill( mask_ptr->begin() + i, mask_ptr->end(), false );
			return true;
		}
	}
	return false;
}

void instruction::analyze()
{
	m_bytes.resize( m_info.size );
	DbgMemRead( m_address, m_bytes.data(), m_info.size );

	csh handle;
	if ( cs_open( CS_ARCH_X86, CS_MODE_64, &handle ) != CS_ERR_OK )
		return;

	cs_option( handle, CS_OPT_DETAIL, CS_OPT_ON );
	cs_option( handle, CS_OPT_SKIPDATA , CS_OPT_OFF );

	cs_insn* insn;
	std::size_t count = cs_disasm( handle, m_bytes.data(), m_bytes.size(), m_address, 0, &insn );

	if ( count == 1 )
	{
		const auto& inst = insn[ 0 ];

		const auto& detail = inst.detail;

		if ( !detail )
		{
			dprintf( "ERROR: detail was nullptr!\n" );
		}
		else
		{
			const auto& x86 = detail->x86;

			size_t opcode_size = 0u;
			for ( int i = 0; i < 4; i++ )
			{
				if ( x86.opcode[ i ] )
					opcode_size++;
			}

			m_mask.resize( inst.size, true );

			int64_t next_op_rel_addr = opcode_size;
			for ( auto i = x86.op_count - 1; i >= 0; i-- )
			{
				const auto& op = x86.operands[ i ];

				if ( insn->bytes[ 0 ] == ASM_OPCODE_CALL || insn->bytes[ 0 ] == ASM_OPCODE_JMP )
				{
					std::fill( m_mask.begin() + 1, m_mask.end(), false );
					break;
				}

				if ( ( m_info.branch || m_info.call ) && m_mask.size() > 2 )
				{
					std::fill( m_mask.begin() + 2, m_mask.end(), false );
					break;
				}

				if ( op.type == X86_OP_MEM )
				{
					const auto& disp = op.mem.disp;
					bool found = false;
					if ( disp <= UCHAR_MAX )
						found = apply_mask_by_search<std::uint8_t>( m_bytes, disp, &m_mask );
					else if ( disp <= SHRT_MAX )
						found = apply_mask_by_search<std::uint16_t>( m_bytes, disp, &m_mask );
					else if ( disp <= UINT_MAX )
						found = apply_mask_by_search<std::uint32_t>( m_bytes, disp, &m_mask );
					else if ( disp <= ULLONG_MAX )
						found = apply_mask_by_search<std::uint64_t>( m_bytes, disp, &m_mask );
					
					break;
				}

				if ( op.type == X86_OP_IMM )
				{
					const auto& imm = op.imm;
					bool found = false;
					if ( imm <= UCHAR_MAX )
						found = apply_mask_by_search<std::uint8_t>( m_bytes, imm, &m_mask );
					else if ( imm < SHRT_MAX )
						found = apply_mask_by_search<std::uint16_t>( m_bytes, imm, &m_mask );
					else if ( imm < UINT_MAX )
						found = apply_mask_by_search<std::uint32_t>( m_bytes, imm, &m_mask );
					else if ( imm < ULLONG_MAX )
						found = apply_mask_by_search<std::uint64_t>( m_bytes, imm, &m_mask );

					if ( !found && imm == UINT_MAX )
						found = apply_mask_by_search<std::uint8_t>( m_bytes, imm, &m_mask );

					break;
				}

				if ( op.type != X86_OP_REG && op.type != X86_OP_MEM )
				{
					std::fill( m_mask.begin() + next_op_rel_addr, m_mask.end(), false );
					break;
				}

				next_op_rel_addr += op.size - 1;
			}


			m_mask[ 0 ] = true;
			
		}

		cs_free( insn, count );
	}
	else
	{
		dprintf( "ERROR: Failed to disassemble given code! Instruction count: %d\n", count );
	}

	cs_close( &handle );

}
