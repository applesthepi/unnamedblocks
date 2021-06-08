#pragma once
#include "config.h"

#include "ui/interfaces/IPositionable.hpp"
#include "ui/interfaces/ISizeable.hpp"

#include <Cappuccino/Utils.hpp>

/// Interface for elements that have a parent's bounds that need to be followed.

class IBoundedParent
{
public:
	/// Sets default bounds.
	IBoundedParent(IPositionable<int32_t>* positionable, ISizeable<int32_t>* sizeable);

	/// Sets positionable.
	/// \param Positionable of parent.
	void SetPositionable(IPositionable<int32_t>* positionable);

	/// Sets sizeable.
	/// \param Sizeable of parent.
	void SetSizeable(ISizeable<int32_t>* sizeable);

	/// Gets positionable.
	/// \return Positionable of parent.
	IPositionable<int32_t>* GetPositionable();

	/// Gets sizeable.
	/// \return Sizeable of parent.
	ISizeable<int32_t>* GetSizeable();
protected:
	/// Parent positionable.
	IPositionable<int32_t>* m_Positionable;

	/// Parent sizeable.
	ISizeable<int32_t>* m_Sizeable;
};