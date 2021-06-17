#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for objects that need a weak reference.

/// IWeak REQURIES it's extender to be an std::shared_ptr<T: IWeak>.
/// If the extender is an interface, then extender must have NO virtual functions
/// for it's parent interface. Function pointers must be passed to the extender from the extender's parent
/// interface's virtual functions. Extender must be a std::shared_ptr<T: IWeak>
/// and to be given an std::weak_ptr reference to itself.

class IWeak
{
public:
	/// Runs initializer list.
	IWeak();

	/// Sets weak ptr for the extender.
	/// \param Weak ptr of extender.
	template <typename T>
	void SetWeak(std::weak_ptr<T> weak);
protected:
	/// Enforce weak check.
	bool IsWeak();

	/// Weak ptr used to pass to external classes to reference the extender.
	std::weak_ptr<T> m_Weak;
private:
	/// Flags whether or not IWeak<T>::m_Weak has been set.
	bool m_WeakSet;
};