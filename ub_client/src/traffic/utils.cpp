#include "utils.hpp"

template<typename T>
void traffic::bezier_sample_points(
	glm::vec<2, T> p1,
	glm::vec<2, T> p2,
	glm::vec<2, T> p3,
	glm::vec<2, T> p4,
	u32 count,
	std::vector<glm::vec<2, f32>>& out
)
{
	f32 t = 0.0f;
	f32 ti = 1.0f;
	f32 t_inc = 1.0f / static_cast<f32>(count);

	auto function_get_point = [&t, &ti](T p1, T p2, T p3, T p4)
	{
		return
			(glm::pow(ti, 3) * static_cast<f32>(p1)) +
			(glm::pow(ti, 2) * t * static_cast<f32>(p2) * 3) +
			(glm::pow(t, 2) * ti * static_cast<f32>(p3) * 3) +
			(glm::pow(t, 3) * static_cast<f32>(p4));
	};

	for (u32 i = 0; i < count; i++)
	{
		out.emplace_back({
			function_get_point(p1.x, p2.x, p3.x, p4.x),
			function_get_point(p1.y, p2.y, p3.y, p4.y)
		});

		t += t_inc;
		ti -= t_inc;
	}
}