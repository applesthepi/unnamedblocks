#include "StatmentIf.h"

StatmentIf::StatmentIf(uint32_t loc, uint32_t endLoc)
	:Location(loc), LocationEnd(endLoc)
{
}

StatmentIf::StatmentIf(uint32_t loc, uint32_t elseLoc, uint32_t endLoc)
	: HasElse(true), HasElseIf(false), Location(loc), LocationEnd(endLoc), LocationElse(elseLoc)
{
}

StatmentIf::StatmentIf(uint32_t loc, std::vector<uint32_t> elseIfLoc, uint32_t endLoc)
	: HasElse(false), HasElseIf(true), Location(loc), LocationEnd(endLoc), LocationElseIf(elseIfLoc)
{
}

StatmentIf::StatmentIf(uint32_t loc, std::vector<uint32_t> elseIfLoc, uint32_t elseLoc, uint32_t endLoc)
	: HasElse(true), HasElseIf(true), Location(loc), LocationEnd(endLoc), LocationElseIf(elseIfLoc), LocationElse(elseLoc)
{
}

StatmentIf::StatmentIf()
	: HasElse(false), HasElseIf(false), Location(0), LocationEnd(0)
{

}