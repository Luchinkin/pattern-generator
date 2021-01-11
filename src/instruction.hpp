#pragma once
#include "utils.hpp"

#include "capstone/include/platform.h"
#include "capstone/include/capstone/capstone.h"

class instruction
{
public:
	instruction( duint addr );

	std::size_t size() const;
	std::uint8_t get( std::size_t idx ) const;
	std::string str() const;
	const std::vector<bool>& mask() const;
private:
	BASIC_INSTRUCTION_INFO m_info = {};
	duint m_address = 0u;
	std::vector<std::uint8_t> m_bytes;
	std::vector<bool> m_mask;

	void analyze();
};
