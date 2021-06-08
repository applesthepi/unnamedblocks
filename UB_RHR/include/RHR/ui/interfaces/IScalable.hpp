#pragma once
#include "config.h"

#include <Cappuccino/Utils.hpp>

/// Interface for sizing ui.

/// Template interface used for elements that have a scale. Can not be abstracted into a joint interface; must be direct extention.

template <typename T>
class IScalable
{
public:
	/// Sets scale for IScalable.
	/// \param Scale of IScalable.
	void SetScale(const glm::vec<2, T>&& scale);

	/// Gets the scale of IScalable.
	/// \return Scale of IScalable.
	glm::vec<2, T> GetScale();
protected:
	/// Event called when IScalable<T>::SetScale(); gets run.
	/// \param Scale of IScalable.
	/// \return Condition to allow IScalable<T>::SetScale(); to run.
	virtual bool OnSetScale(const glm::vec<2, T>&& scale);

	/// Event called when IScalable<T>::GetScale(); gets run.
	virtual void OnGetScale();

	/// Scale of IScalable.
	glm::vec<2, T> m_Scale;
};