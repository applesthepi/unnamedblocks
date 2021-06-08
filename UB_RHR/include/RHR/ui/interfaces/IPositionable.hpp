#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for positioning ui.

/// Template interface used for elements that have a position offset from a parent. Can not be abstracted into a joint interface; must be direct extention.

template <typename T>
class IPositionable
{
public:
	/// expected only main thread acccess; non-atomic
	/// \param position of IPositionable
	void SetPosition(const glm::vec<2, T>&& position);

	/// expected only main thread acccess; non-atomic
	/// \return position of IPositionable
	glm::vec<2, T> GetPosition();
protected:
	/// event called when IPositionable<T>::SetPosition(); gets run
	/// \param position of IPositionable
	/// \return condition to allow IPositionable<T>::SetPosition(); to run
	virtual bool OnSetPosition(const glm::vec<2, T>&& position);

	/// event called when IPositionable<T>::GetPosition(); gets run
	virtual void OnGetPosition();

	/// expected only main thread acccess; non-atomic
	glm::vec<2, T> m_Position;
};