#include "i_scalable.hpp"

template <size_t S, typename T>
rhr::render::interfaces::i_scaleable<S, T>::i_scaleable()
	: m_position(glm::vec<S, T>())
	, m_super_position(glm::vec<S, T>())
{

}

template <size_t S, typename T>
void rhr::render::interfaces::i_scaleable<S, T>::set_scale(const glm::vec<S, T>& scale)
{
	if (pre_scale_update(scale, m_super_scale))
	{
		m_scale = scale;
		post_scale_update();
	}
}

template <size_t S, typename T>
void rhr::render::interfaces::i_scaleable<S, T>::set_super_scale(const glm::vec<S, T>& super_scale)
{
	if (pre_position_update(m_scale, super_scale))
	{
		m_super_scale = super_scale;
		post_scale_update();
	}
}

template <size_t S, typename T>
glm::vec<S, T> rhr::render::interfaces::i_scaleable<S, T>::get_scale()
{
	return m_scale;
}

template <size_t S, typename T>
glm::vec<S, T> rhr::render::interfaces::i_scaleable<S, T>::get_super_scale()
{
	return m_super_scale;
}

template <size_t S, typename T>
bool rhr::render::interfaces::i_scaleable<S, T>::pre_scale_update(const glm::vec<S, T>& scale, const glm::vec<S, T>& super_scale)
{
	return true;
}

template <size_t S, typename T>
void rhr::render::interfaces::i_scaleable<S, T>::post_scale_update()
{

}

template class rhr::render::interfaces::i_scaleable<2, i64>;
template class rhr::render::interfaces::i_scaleable<2, i32>;
template class rhr::render::interfaces::i_scaleable<2, i16>;
template class rhr::render::interfaces::i_scaleable<2, i8>;

template class rhr::render::interfaces::i_scaleable<2, u64>;
template class rhr::render::interfaces::i_scaleable<2, u32>;
template class rhr::render::interfaces::i_scaleable<2, u16>;
template class rhr::render::interfaces::i_scaleable<2, u8>;

template class rhr::render::interfaces::i_scaleable<2, f64>;
template class rhr::render::interfaces::i_scaleable<2, f32>;

template class rhr::render::interfaces::i_scaleable<3, i64>;
template class rhr::render::interfaces::i_scaleable<3, i32>;
template class rhr::render::interfaces::i_scaleable<3, i16>;
template class rhr::render::interfaces::i_scaleable<3, i8>;

template class rhr::render::interfaces::i_scaleable<3, u64>;
template class rhr::render::interfaces::i_scaleable<3, u32>;
template class rhr::render::interfaces::i_scaleable<3, u16>;
template class rhr::render::interfaces::i_scaleable<3, u8>;

template class rhr::render::interfaces::i_scaleable<3, f64>;
template class rhr::render::interfaces::i_scaleable<3, f32>;