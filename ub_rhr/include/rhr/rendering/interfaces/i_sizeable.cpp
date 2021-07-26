#include "i_sizeable.hpp"

template <usize S, typename T>
rhr::render::interfaces::i_sizeable<S, T>::i_sizeable()
	: m_size(glm::vec<S, T>())
	, m_super_bounds(glm::vec<S, T>())
{

}

template <usize S, typename T>
void rhr::render::interfaces::i_sizeable<S, T>::set_size(const glm::vec<S, T>& size)
{
	if (pre_size_update(size, m_super_bounds))
	{
		m_size = size;
		post_size_update();
	}
}

template <usize S, typename T>
void rhr::render::interfaces::i_sizeable<S, T>::set_super_bounds(const glm::vec<S, T>& super_bounds)
{
	if (pre_size_update(m_size, super_bounds))
	{
		m_super_bounds = super_bounds;
		post_size_update();
	}
}

template <usize S, typename T>
glm::vec<S, T> rhr::render::interfaces::i_sizeable<S, T>::get_size()
{
	return m_size;
}

template <usize S, typename T>
glm::vec<S, T> rhr::render::interfaces::i_sizeable<S, T>::get_super_bounds()
{
	return m_super_bounds;
}

template <usize S, typename T>
void rhr::render::interfaces::i_sizeable<S, T>::set_size_max()
{
	set_size(m_super_bounds);
}

template <usize S, typename T>
bool rhr::render::interfaces::i_sizeable<S, T>::pre_size_update(const glm::vec<S, T>& size, const glm::vec<S, T>& super_bounds)
{
	return true;
}

template <usize S, typename T>
void rhr::render::interfaces::i_sizeable<S, T>::post_size_update()
{

}

template class rhr::render::interfaces::i_sizeable<2, i64>;
template class rhr::render::interfaces::i_sizeable<2, i32>;
template class rhr::render::interfaces::i_sizeable<2, i16>;
template class rhr::render::interfaces::i_sizeable<2, i8>;

template class rhr::render::interfaces::i_sizeable<2, u64>;
template class rhr::render::interfaces::i_sizeable<2, u32>;
template class rhr::render::interfaces::i_sizeable<2, u16>;
template class rhr::render::interfaces::i_sizeable<2, u8>;

template class rhr::render::interfaces::i_sizeable<2, f64>;
template class rhr::render::interfaces::i_sizeable<2, f32>;

template class rhr::render::interfaces::i_sizeable<3, i64>;
template class rhr::render::interfaces::i_sizeable<3, i32>;
template class rhr::render::interfaces::i_sizeable<3, i16>;
template class rhr::render::interfaces::i_sizeable<3, i8>;

template class rhr::render::interfaces::i_sizeable<3, u64>;
template class rhr::render::interfaces::i_sizeable<3, u32>;
template class rhr::render::interfaces::i_sizeable<3, u16>;
template class rhr::render::interfaces::i_sizeable<3, u8>;

template class rhr::render::interfaces::i_sizeable<3, f64>;
template class rhr::render::interfaces::i_sizeable<3, f32>;