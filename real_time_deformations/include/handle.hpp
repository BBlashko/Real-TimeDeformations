#pragma once

#include <atlas/math/Math.hpp>
#include <vector>

namespace rt_deformations {

	class handle {
	public:
		handle() = default;
	
		virtual atlas::math::Matrix4 generate_transformations() = 0;
		virtual void set_selected(bool selected) = 0;
		virtual bool selected() = 0;
		virtual bool intersect(atlas::math::Point p) = 0;
		virtual std::vector<atlas::math::Point> data() = 0;
		virtual void set_point(atlas::math::Point p) = 0;

		const static float _p_size;
		const static float _rp_size;
	};

}