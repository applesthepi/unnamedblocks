#pragma once
#include <utility>

#include "config.h"

#include "data.hpp"
#include "flags.hpp"
#include "lungo/interfaces/i_ui.hpp"
#include "lungo/vertex.hpp"
#include "espresso/argument.hpp"

#include <espresso/mod/block/pass.hpp>
#include <cappuccino/preprocessor_data.hpp>
#include <latte/utils.hpp>
#include <espresso/color.hpp>
#include <espresso/mod/block/block.hpp>

/*
Important Modding Information

- Do NOT use the "latte::logger" class during runtime! Use the logger methods inside the ModBlockPass.

*/

namespace espresso::mod::block
{
///
class block
{
public:
	/// Wrapper struct for other data structures.
	struct argument
	{
		/// This is the argument type, an argument can only allow this variable type to be used.
		/// "ANY" is indicated for any *variable* input. The type will unspecified in cappuccino and will be presented
		/// as a "void*".
		enum class type
		{
			TEXT,
			REAL,
			BOOL,
			STRING,
			ANY
		};

		///
		class initializer
		{
		public:
			initializer(
				const type type,
				const esp::argument::mode_restriction restriction,
				const esp::argument::mode mode,
				std::string default_value)
				: m_type(type)
				, m_restriction(restriction)
				, m_mode(mode)
				, m_default_value(std::move(default_value))
			{}

			///
			type& get_type()
			{
				return m_type;
			}

			///
			esp::argument::mode_restriction& get_restriction()
			{
				return m_restriction;
			}

			///
			esp::argument::mode& get_mode()
			{
				return m_mode;
			}

			///
			std::string& get_default_value()
			{
				return m_default_value;
			}

		private:
			///
			type m_type;

			///
			esp::argument::mode_restriction m_restriction;

			///
			esp::argument::mode m_mode;

			///
			std::string m_default_value;
		};
	};

	///
	typedef void (*execution)(espresso::mod::block::pass*);

	///
	typedef bool (*initialization)(cap::preprocessor_data&);

	///
	typedef bool (*data_initialization)(cap::preprocessor_data&, espresso::mod::block::data&);

	/// How the block will be identified as. Convention example: "mymod_some_block_name".
	virtual const char* get_unlocalized_name() const;

	/// Category of the block.
	virtual const char* get_category() const;

	/// Does the block start a stack.
	virtual bool is_topical() const;

	/// What will be executed in a release build.
	virtual execution pull_execute_release() const;

	/// What will be executed in a debug build.
	virtual execution pull_execute_debug() const;

	/// Actions to run once per block type; during runtime before Initialization.
	virtual initialization get_runtime_global_pre_init() const;

	/// Actions to run once per block type; during runtime after Initialization.
	virtual initialization get_runtime_global_post_init() const;

	/// Actions to run once per block type per stack; during runtime before Initialization.
	virtual initialization get_runtime_local_pre_init() const;

	/// Actions to run once per block type per stack; during runtime after Initialization.
	virtual initialization get_runtime_local_post_init() const;

	/// Actions to run on each block preprocessed; during Initialization.
	virtual std::vector<std::pair<data_initialization, u16>> get_runtime_stages() const;

	/// Block arguments, including text.
	virtual std::vector<argument::initializer> get_arguments() const;
};
} // namespace espresso::mod::block
