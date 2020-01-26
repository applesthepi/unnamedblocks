#include "StatmentIf.h"

StatmentIf::StatmentIf(uint32_t loc, uint32_t endLoc)
	:Location(loc), LocationEnd(endLoc)
{
}

StatmentIf::StatmentIf(uint32_t loc, uint32_t elseLoc, uint32_t endLoc)
	: Location(loc), LocationEnd(endLoc), HasElse(true), HasElseIf(false), LocationElse(elseLoc)
{
}

StatmentIf::StatmentIf(uint32_t loc, std::vector<uint32_t> elseIfLoc, uint32_t endLoc)
	: Location(loc), LocationEnd(endLoc), HasElse(false), HasElseIf(true), LocationElseIf(elseIfLoc)
{
}

StatmentIf::StatmentIf(uint32_t loc, std::vector<uint32_t> elseIfLoc, uint32_t elseLoc, uint32_t endLoc)
	: Location(loc), LocationEnd(endLoc), HasElse(true), HasElseIf(true), LocationElseIf(elseIfLoc), LocationElse(elseLoc)
{
}

StatmentIf::StatmentIf()
	: Location(0), LocationEnd(0), HasElse(false), HasElseIf(false)
{

}
