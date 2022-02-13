[![CMake](https://github.com/applesthepi/unnamedblocks/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/applesthepi/unnamedblocks/actions/workflows/cmake.yml)
[![CodeQL](https://github.com/applesthepi/unnamedblocks/actions/workflows/codeql-analysis.yml/badge.svg?branch=master)](https://github.com/applesthepi/unnamedblocks/actions/workflows/codeql-analysis.yml)

# Installation
### General User
Please note that this project isnt setup properly for public use. In the future there will be more downloads. For now, the only demo availible is from 2 years ago is for windows only here https://unnamedblocks.com/downloads

### Development Installation
**master** - active development branch  
**stable** - stable releases (major and minor)  
**nightly** - bleeding edge releases (beta and alpha)  

Your expected to have cmake, git, vulkan, and clang installed and in your path. Follow these steps for repo setup:
#### Windows / Visual Studio
1. `git clone https://github.com/applesthepi/unnamedblocks --recurse-submodules`
2. `cd unnamedblocks/`
3. `.\win_configure_debug.bat`
4. copy the imgui_XX-XX-XXXX.ini file to `build/debug/ub_client/` and rename it to `imgui.ini` (**FIRST TIME ONLY**)
5. open `UB.sln` inside `build/debug/` directory
6. rebuild solution
7. run `UB_CLIENT.exe` in `build/debug/ub_client/` or run in Visual Studio

#### Linux Shell
1. `git clone https://github.com/applesthepi/unnamedblocks --recurse-submodules && cd unnamedblocks`
2. `./configure_debug.sh`
3. copy the imgui_XX-XX-XXXX.ini file to `build/debug/ub_client/` and rename it to `imgui.ini` (**FIRST TIME ONLY**)
4. `cd build/debug/`
5. `make -j8`
6. run `./UB_CLIENT` in `ub_client/`

# Code Style
This describes the code style that should be used throughout the Unnamed Blocks repository. External additions like mods absolutely do not have to follow this strict standard. I plan to add bindings using different case styles for c++ in the future.

### Filename
`snake_case_only_please.hpp`
`snake_case_only_please.cpp`

### Includes
hpp
```cpp
/* required     */ #pragma once
/* required     */ #include "config.h"

/* current lib  */ #include "rhr/stacking/block.hpp"
/* current lib  */ #include "rhr/stacking/arguments/argument_text.hpp"
/* current lib  */ #include "rhr/registries/char_texture.hpp"

/* required     */ #include <cappuccino/utils.hpp>
/* external lib */ #include <cappuccino/mod_block_pass.hpp>
/* external lib */ #include <cappuccino/preprocessor_data.hpp>
/* external lib */ #include <cappuccino/registration.hpp>
```
cpp
```cpp
/* req if class */ #include "example_class.hpp"

/* current lib  */ #include "rhr/stacking/block.hpp"
/* current lib  */ #include "rhr/stacking/arguments/argument_text.hpp"
/* current lib  */ #include "rhr/registries/char_texture.hpp"

/* external lib */ #include <cappuccino/color.hpp>
```

### File Structure
- If inside the class, you can reference objects without using the absolute namespace.
- You may use `using namespace` in a cpp file for namespaces that you dont want to type out, though it is still recommended you type it out so its clear. That way you can easily do `handler::project` instead of `project_handler`.
- Use the absolute paths for including files unless your including the hpp file of a class from the class's cpp file.
- namespaces should follow the path. Have the path be plural/verb and the namespace be singular/noun. For example you might have a path of `rendering/objects/text.hpp` and you would reference it though `render::object::text`.

hpp
```cpp
namespace rhr::some::space
{
///
class example_class : public rhr::some::thing
{
public:
	///
	struct data_snake_case
	{
		///
		u64 use_these_types;
		
		///
		i8 use_snake_case;
	};
	
	///
	enum class some_enum_thing
	{
		LIGHT_NORMAL,
		BOLD_NORMAL,
		LIGHT_ITALIC,
		BOLD_ITALIC
	};
	
	///
	void also_use_snake_case(u8 use_snake_case = 5);
	
	///
	void another_one(u8 something_yes);
	
	void overrided_function_init_1() override;
	void overrided_function_init_2() override;

protected:
	///
	void some_protected_function();
	
	void overrided_function_1() override;
	void overrided_function_2() override;
	void overrided_function_3() override;
	
private:
	///
	void some_private_function();

	///
	f32 m_use_this_convention;
};
}
```
cpp
```cpp
rhr::some::space::example_class::also_use_snake_case(u8 use_snake_case)
{

}

rhr::some::space::example_class::another_one(u8 something_yes)
{

}
```
### Documentation
Unnamed Blocks currently uses doxygen documentation generation. In the example above, notice how overrided functions dont even have a space for documentation, this is because doxygen uses the most super class's documentation. Heres a big example from the i_ui class. Also notice how any phrase or sentence begins with a capital letter and ends with a period.
```cpp
/// Possible buffer update, use flags to make sure you need to call this.
/// \param offset This object's physical position (render space). See (TODO: link) for more clarification.
/// \param update_child Notifies this of a transform update using i_ui::transform_update_spec_position
void set_position_local_physical(const glm::vec<2, i32>& offset, bool update_child);
```
### Version Standard
This is the version standard Unnamed Blocks will be following.

Major . Minor . Beta

All Major and Minor updates should be user tested extensively while beta updates will have tests ran on it and briefly user tested with some projects. Major and Minor updates will both use the stable branch while beta releases will be merged to nightly. Master will just be the development branch that isnt ready for public use and may have issues.

# Tools
### Formating
There is a formatting shell script that will format the repo files based on the clang format provided in the repo root. If you want the script to format cmake as well, you need to install `cmake-format` package on linux.

### Configuring
Debug and Release configuration scripts are provided in the repo root for both linux and windows (prefixed with `win_`).