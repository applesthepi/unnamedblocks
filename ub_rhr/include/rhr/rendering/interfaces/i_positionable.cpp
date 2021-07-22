#include "i_positionable.hpp"

template <size_t S, typename T>
rhr::render::interfaces::i_positionable<S, T>::i_positionable()
	: m_position(glm::vec<S, T>())
	, m_super_position(glm::vec<S, T>())
{

}

template <size_t S, typename T>
void rhr::render::interfaces::i_positionable<S, T>::set_position(const glm::vec<S, T>& position)
{
	if (pre_position_update(position, super_position))
	{
		m_position = position;
		post_position_update();
	}
}

template <size_t S, typename T>
void rhr::render::interfaces::i_positionable<S, T>::set_super_position(const glm::vec<S, T>& super_position)
{
	if (pre_position_update(m_position, super_position))
	{
		m_super_position = super_position;
		post_position_update();
	}
}

template <size_t S, typename T>
glm::vec<S, T> rhr::render::interfaces::i_positionable<S, T>::get_position()
{
	return m_position;
}

template <size_t S, typename T>
glm::vec<S, T> rhr::render::interfaces::i_positionable<S, T>::get_super_position()
{
	return m_super_position;
}

template <size_t S, typename T>
bool rhr::render::interfaces::i_positionable<S, T>::pre_position_update(const glm::vec<S, T>& position, const glm::vec<S, T>& super_position)
{
	return true;
}

template <size_t S, typename T>
void rhr::render::interfaces::i_positionable<S, T>::post_position_update()
{
	
}

template class rhr::render::interfaces::i_positionable<2, i64>;
template class rhr::render::interfaces::i_positionable<2, i32>;
template class rhr::render::interfaces::i_positionable<2, i16>;
template class rhr::render::interfaces::i_positionable<2, i8>;

template class rhr::render::interfaces::i_positionable<2, u64>;
template class rhr::render::interfaces::i_positionable<2, u32>;
template class rhr::render::interfaces::i_positionable<2, u16>;
template class rhr::render::interfaces::i_positionable<2, u8>;

template class rhr::render::interfaces::i_positionable<2, f64>;
template class rhr::render::interfaces::i_positionable<2, f32>;

template class rhr::render::interfaces::i_positionable<3, i64>;
template class rhr::render::interfaces::i_positionable<3, i32>;
template class rhr::render::interfaces::i_positionable<3, i16>;
template class rhr::render::interfaces::i_positionable<3, i8>;

template class rhr::render::interfaces::i_positionable<3, u64>;
template class rhr::render::interfaces::i_positionable<3, u32>;
template class rhr::render::interfaces::i_positionable<3, u16>;
template class rhr::render::interfaces::i_positionable<3, u8>;

template class rhr::render::interfaces::i_positionable<3, f64>;
template class rhr::render::interfaces::i_positionable<3, f32>;