#pragma once
#include <vector>
#include <cinttypes>

class StatmentIf
{
public:
	StatmentIf();
	StatmentIf(uint32_t loc, uint32_t endLoc);
	StatmentIf(uint32_t loc, uint32_t elseLoc, uint32_t endLoc);
	StatmentIf(uint32_t loc, std::vector<uint32_t> elseIfLoc, uint32_t endLoc);
	StatmentIf(uint32_t loc, std::vector<uint32_t> elseIfLoc, uint32_t elseLoc, uint32_t endLoc);

	uint32_t Location;
	uint32_t LocationEnd;

	//extended
	bool HasElse;
	bool HasElseIf;

	std::vector<uint32_t> LocationElseIf;
	uint32_t LocationElse;
};
