#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
///
class i_ui
{
public:
	///
	i_ui();

	///
	void set_position_local_physical(const glm::vec<2, i32>& offset, bool update_child = true);

	///
	void set_position_local_virtual(const glm::vec<2, i32>& offset, bool update_child = true);

	///
	void set_position_parent_physical(const glm::vec<2, i32>& offset, bool update_child = true);

	///
	void set_position_parent_virtual(const glm::vec<2, i32>& offset, bool update_child = true);

	///
	glm::vec<2, i32> get_position_local_physical();

	///
	glm::vec<2, i32> get_position_local_virtual();

	///
	glm::vec<2, i32> get_position_parent_physical();

	///
	glm::vec<2, i32> get_position_parent_virtual();

	///
	glm::vec<2, i32> get_position_physical();

	///
	glm::vec<2, i32> get_position();

	///
	void update_child();
protected:
	/// Event called when i_positionable<T>::set_position() or i_positionable<T>::set_super_position() gets involked.
	/// \param Position of i_positionable.
	/// \param Super position of i_positionable.
	/// \return Condition to allow i_positionable<T>::set_position() or i_positionable<T>::set_super_position() to run.
//	virtual bool pre_position_update(const glm::vec<S, T>& position, const glm::vec<S, T>& super_position);

	/// Event called after any position update function gets run.
	virtual void post_position_update();
private:
	///
	glm::vec<2, i32> m_position_local_physical;

	///
	glm::vec<2, i32> m_position_local_virtual;

	///
	glm::vec<2, i32> m_position_parent_physical;

	///
	glm::vec<2, i32> m_position_parent_virtual;

	///
	glm::vec<2, i32> m_position_physical;

	///
	glm::vec<2, i32> m_position;
};
}