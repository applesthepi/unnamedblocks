#include "cxx.h"
#include "program.rs.h"

#include <latte/utils.hpp>

using namespace rust::cxxbridge1;

int main() {
	latte::logger::initialize();
	latte::logger::info(latte::logger::level::SYSTEM, __FILE__, __LINE__, "testing");
	vpe::ProgramContext program_context = vpe::ProgramContext{};
	program_context.name = "unnamedblocks";
	vpe::initialize(Box(program_context));
	vpe::run();
	return 0;
}