#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for sizing ui.

/// Template interface used for elements that have a size. Can not be abstracted into a joint interface; must be direct extention.

template <typename T>
class ISizeable
{
public:
	ISizeable();

	/// Sets size of ISizeable.
	/// \param Size of ISizeable.
	void SetSize(const glm::vec<2, T>& size);

	/// Sets super bounds of ISizeable.
	/// \param Super bounds of ISizeable.
	void SetSuperBounds(const glm::vec<2, T>& bounds);

	/// Gets size of ISizeable.
	/// \return Size of ISizeable.
	glm::vec<2, T> GetSize();

	/// Gets super bounds of ISizeable.
	/// \return Super bounds of ISizeable.
	glm::vec<2, T> GetSuperBounds();
protected:
	/// Event called when ISizeable<T>::SetSize() or ISizeable<T>::SetSuperBounds() gets run.
	/// \param Size of ISizeable.
	/// \param Super bounds of ISizeable.
	/// \return Condition to allow ISizeable<T>::SetSize(); to run.
	virtual bool OnSizeUpdate(const glm::vec<2, T>& size, const glm::vec<2, T>& bounds);

	/// Event called when ISizeable<T>::GetSize() or ISizeable<T>::GetSuperBounds() gets run.
	virtual void OnSizePull();

	/// Size of ISizeable.
	glm::vec<2, T> m_Size;

	/// Super bounds of ISizeable.
	glm::vec<2, T> m_SuperBounds;
};
