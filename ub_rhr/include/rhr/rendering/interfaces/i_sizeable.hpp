#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Template interface used for elements that have a size and bounded size from a parent.
template <size_t S, typename T>
class i_sizeable
{
public:
	///
	i_sizeable();

	///
	void set_size(const glm::vec<S, T>& size);

	///
	void set_super_bounds(const glm::vec<S, T>& super_bounds);

	///
	glm::vec<S, T> get_size();

	///
	glm::vec<S, T> get_super_bounds();
protected:
	/// Event called when i_sizeable<T>::set_size() or i_sizeable<T>::set_super_bounds() gets involked.
	/// \param Size of i_sizeable.
	/// \param Super bounds of i_sizeable.
	/// \return Condition to allow i_sizeable<T>::set_size() or i_sizeable<T>::set_super_bounds() to run.
	virtual bool pre_size_update(const glm::vec<S, T>& size, const glm::vec<S, T>& super_bounds);

	/// Event called after i_sizeable<T>::set_size() or i_sizeable<T>::set_super_bounds() gets run.
	virtual void post_size_update();

	///
	glm::vec<S, T> m_size;

	///
	glm::vec<S, T> m_super_bounds;
};
}