#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for positioning ui.

/// Template interface used for elements that have a position offset from a parent. Can not be abstracted into a joint interface; must be direct extention.

template <typename T>
class IPositionable
{
public:
	IPositionable();

	/// expected only main thread acccess; non-atomic
	/// \param position of IPositionable
	void SetPosition(const glm::vec<2, T>&& position);

	/// expected only main thread acccess; non-atomic
	/// \param super offset of IPositionable
	void SetSuperOffset(const glm::vec<2, T>&& offset);

	/// expected only main thread acccess; non-atomic
	/// \return position of IPositionable
	glm::vec<2, T> GetPosition();

	/// expected only main thread acccess; non-atomic
	/// \return super offset of IPositionable
	glm::vec<2, T> GetSuperOffset();
protected:
	/// event called when IPositionable<T>::SetPosition() or IPositionable<T>::SetSuperOffset() gets run
	/// \param position of IPositionable
	/// \param super offset of IPositionable
	/// \return condition to allow IPositionable<T>::SetPosition() to run
	virtual bool OnPositionUpdate(const glm::vec<2, T>&& position, const glm::vec<2, T>&& offset);

	/// event called when IPositionable<T>::GetPosition() or IPositionable<T>::GetSuperOffset(); gets run
	virtual void OnPositionPull();

	/// expected only main thread acccess; non-atomic
	glm::vec<2, T> m_Position;

	/// expected only main thread acccess; non-atomic
	glm::vec<2, T> m_SuperOffset;
};