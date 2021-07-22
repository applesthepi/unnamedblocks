#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Template interface used for elements that have a position offset from a parent.
template <size_t S, typename T>
class i_positionable
{
public:
	///
	i_positionable();

	///
	void set_position(const glm::vec<S, T>& position);

	///
	void set_super_position(const glm::vec<S, T>& offset);

	///
	glm::vec<S, T> get_position();

	///
	glm::vec<S, T> get_super_position();
protected:
	/// Event called when i_positionable<T>::set_position() or i_positionable<T>::set_super_position() gets involked.
	/// \param Position of i_positionable.
	/// \param Super position of i_positionable.
	/// \return Condition to allow i_positionable<T>::set_position() or i_positionable<T>::set_super_position() to run.
	virtual bool pre_position_update(const glm::vec<S, T>& position, const glm::vec<S, T>& super_position);

	/// Event called after i_positionable<T>::set_position() or i_positionable<T>::set_super_position() gets run.
	virtual void post_position_update();

	///
	glm::vec<S, T> m_position;

	///
	glm::vec<S, T> m_super_position;
};
}