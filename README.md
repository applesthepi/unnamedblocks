# Installation
Please note that this project isnt setup properly for public use. In the future there will be proper downloadable demos and an easy way to clone and build the repository. For now, the only demo availible is from 1 year ago here https://applesthepi.com/unnamedblocks/downloads

### General User
TODO

### Development Installation
TODO

# Code Style
This describes the code style that everyone should use throughout UB and in mods. While the style might not be what people are used to, it would be very helpful if everyone used the same style. Nothing will happen if you don't follow these, it's just a strong recommendation to follow these standards.

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
class example_use_snake_case
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
private:
	///
	f32 m_use_this_convention;
};
};
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

### Documentation
Unnamed Blocks currently uses doxygen documentation generation. Heres a big example from the i_positionable class. Also notice how any phrase or sentence begins with a capital letter and ends with a period.
```cpp
/// Event called when i_positionable<T>::set_position() or i_positionable<T>::set_super_position() gets run.
/// \param Position of i_positionable.
/// \param Super offset of i_positionable.
/// \return Condition to allow i_positionable<T>::set_position() to run.
virtual bool preposition_update(const glm::vec<S, T>& position, const glm::vec<S, T>& offset);
```
