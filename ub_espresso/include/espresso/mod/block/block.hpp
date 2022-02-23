#pragma once
#include <utility>

#include "config.h"

#include "data.hpp"
#include "flags.hpp"

#include <espresso/mod/block/pass.hpp>
#include <cappuccino/preprocessor_data.hpp>
#include <latte/utils.hpp>

/*
Important Modding Information

- Do NOT use the "cap::logger" class during runtime! Use the logger methods inside the ModBlockPass.

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
		enum class variable_mode_restriction
		{
			/// Excepts either raw or var, raw will be raw, var will be the value.
			NONE,

			/// Does not change from initial "espresso::mod::block::argument::variable_mode".
			RESTRICTED
		};

		/// This is the default assignment of whether it starts as var or raw input. The user can change what they
		/// input. If you want to restrict how data is inputted see the
		/// "espresso::mod::block::argument::variable_mode_restriction" class.
		enum class variable_mode
		{
			VAR,
			RAW
		};

		///
		class initializer
		{
		public:
			initializer(
				const type type,
				const variable_mode_restriction restriction,
				const variable_mode mode,
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
			variable_mode_restriction& get_restriction()
			{
				return m_restriction;
			}

			///
			variable_mode& get_mode()
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
			variable_mode_restriction m_restriction;

			///
			variable_mode m_mode;

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
