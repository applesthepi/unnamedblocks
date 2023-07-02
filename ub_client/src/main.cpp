#include "cxx.h"
#include "program.rs.h"

#include <iostream>
using namespace rust::cxxbridge1;

int main() {
	vpe::ProgramContext program_context = vpe::ProgramContext{};
	program_context.name = "unnamedblocks";
	vpe::initialize(Box(program_context));
	vpe::run();
	return 0;
}