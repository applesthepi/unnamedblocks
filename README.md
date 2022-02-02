# Installation
### General User
Please note that this project isnt setup properly for public use. In the future there will be more downloads. For now, the only demo availible is from 2 years ago is for windows only here https://applesthepi.com/unnamedblocks/downloads

### Development Installation
**master** - active development branch (features not functioning)  
**stable** - stable releases (2yo demo)  
**nightly** - bleeding edge releases (2yo demo)
  
  Your expected to have cmake and git installed and in your path. Follow these steps for repo setup:
#### Windows / Visual Studio
1. `git clone https://github.com/applesthepi/unnamedblocks`
2. `.\initialize.bat`
3. `.\configure.bat`
4. open `UB.sln` inside `build/` directory

#### Linux Shell
1. `git clone https://github.com/applesthepi/unnamedblocks`
2. `./initialize.sh`
3. `./configure.sh`

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
namespace rhr::same::as::path
{
///
class example_use_snake_case : public rhr::some::thing
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

	void overrided_function_init_1() override;
	void overrided_function_init_2() override;
	
	///
	void also_use_snake_case(u8 use_snake_case = 5);
	
	///
	void another_one(u8 something_yes);
private:
	void overrided_function_1() override;
	void overrided_function_2() override;
	void overrided_function_3() override;

	///
	f32 m_use_this_convention;
};
}
```
cpp
```cpp
example_class::also_use_snake_case(u8 use_snake_case)
{

}

example_class::another_one(u8 something_yes)
{

}
```

### Formating
There is a formating script that will format the repo files based on the clang format provided in the repo root.

### Documentation
Unnamed Blocks currently uses doxygen documentation generation. In the example above, notice how overrided functions dont even have a space for documentation, this is because doxygen uses the most super class's documentation. Heres a big example from the i_positionable class. Also notice how any phrase or sentence begins with a capital letter and ends with a period.
```cpp
/// Event called when i_positionable<T>::set_position() or i_positionable<T>::set_super_position() gets run.
/// \param Position of i_positionable.
/// \param Super offset of i_positionable.
/// \return Condition to allow i_positionable<T>::set_position() to run.
virtual bool preposition_update(const glm::vec<S, T>& position, const glm::vec<S, T>& offset);
```
### Version Standard
This is the version standard Unnamed Blocks will be following.  

Major . Minor . Beta  

All Major and Minor updates should be user tested extensivly while beta updates will have tests ran on it and briefly user tested with some projects. Major and Minor updates will both use the stable branch while beta releases will be merged to nightly. Master will just be the development branch that isnt ready for public use and may have issues.
