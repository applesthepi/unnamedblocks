#pragma once
#include "config.h"

#include "ui/Vertex.hpp"
#include "ui/interfaces/IUI.hpp"

#include <Cappuccino/Utils.hpp>
#include <Cappuccino/block/ModBlock.hpp>

/// Base class for visual block arguments.

class Argument : public IUI
{
public:
	/// TODO
	Argument();

	/// Sets data. Data of the argument is stored as a string.
	/// \param Data to set the argument to.
	void SetData(const std::string& data);

	/// Gets data. Data of the argument is stored as a string.
	/// \return Data from the argument.
	const std::string& GetData();

	/// Sets mode. Mode can either be raw (rvalue) or var (lvalue).
	/// \param Argument variable mode.
	void SetMode(BlockArgumentVariableMode mode);

	/// Gets mode. Mode can either be raw (rvalue) or var (lvalue).
	/// \return Argument variable mode.
	BlockArgumentVariableMode GetMode();

	/// Pull whether or not the argument is requesting to goto the next argument.
	/// \return Pull request.
	// bool PullNext();

	/// How to interpret the data.
	/// \return Type of argument.
	virtual BlockArgumentType GetType();

	/// Get width for surrounding argument spacing.
	/// \return Width of argument.
	virtual uint32_t GetWidth();

	/// Whether or not the argument contains data for storing.
	/// \return Has data.
	virtual bool HasData();

	/// Select the argument and enable it.
	virtual void Select();

	/// When the focus turns off of the argument.
	virtual void UnSelect();

	/// Gets all elements used by the argument.
	/// \return Vector of elements subject to change.
	std::vector<IUI>& GetElements();
protected:
	/// Gets pulled when Argument::PullNext(); gets called.
	// bool m_Next;

	/// Data of argument stored as a string.
	std::string m_Data;

	/// Mode of argument.
	BlockArgumentVariableMode m_Mode;

	/// Vector of elements used by the argument.
	std::vector<IUI> m_Elements;
};
