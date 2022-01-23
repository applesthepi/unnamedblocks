#pragma once
#include "config.h"

#include <cappuccino/utils.hpp>

namespace rhr::render::interfaces
{
/// Template interface used for elements that have a size and bounded size from a parent.
template<usize S, typename T>
class i_scaleable
{
public:
	///
	i_scaleable();

	///
	void set_scale(const glm::vec<S, T>& scale);

	///
	void set_super_scale(const glm::vec<S, T>& super_scale);

	///
	glm::vec<S, T> get_scale();

	///
	glm::vec<S, T> get_super_scale();

protected:
	/// Event called when i_scaleable<T>::set_scale() or i_scaleable<T>::set_super_scale() gets involked.
	/// \param Size of i_scaleable.
	/// \param Super bounds of i_scaleable.
	/// \return Condition to allow i_scaleable<T>::set_scale() or i_scaleable<T>::set_super_scale() to run.
	virtual bool pre_scale_update(const glm::vec<S, T>& size, const glm::vec<S, T>& super_bounds);

	/// Event called after i_scaleable<T>::set_scale() or i_scaleable<T>::set_super_scale() gets run.
	virtual void post_scale_update();

	///
	glm::vec<S, T> m_scale;

	///
	glm::vec<S, T> m_super_scale;
};
} // namespace rhr::render::interfaces