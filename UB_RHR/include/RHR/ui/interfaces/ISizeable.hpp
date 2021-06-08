#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for sizing ui.

/// Template interface used for elements that have a size. Can not be abstracted into a joint interface; must be direct extention.

template <typename T>
class ISizeable
{
public:
	/// Sets size of ISizeable.
	/// \param Size of ISizeable.
	void SetSize(const glm::vec<2, T>&& size);

	/// Gets size of ISizeable.
	/// \return Size of ISizeable.
	glm::vec<2, T> GetSize();
protected:
	/// Event called when ISizeable<T>::SetSize(); gets run.
	/// \param Size of ISizeable.
	/// \return Condition to allow ISizeable<T>::SetSize(); to run.
	virtual bool OnSetSize(const glm::vec<2, T>&& size);

	/// Event called when ISizeable<T>::GetSize(); gets run.
	virtual void OnGetSize();

	/// Size of ISizeable.
	glm::vec<2, T> m_Size;
};