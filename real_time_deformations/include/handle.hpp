#pragma once

#include <atlas/math/Math.hpp>
#include <atlas/utils/Geometry.hpp>
#include <vector>

namespace rt_deformations {

	class handle : public atlas::utils::Geometry {
	public:
		handle() = default;
	
		virtual atlas::math::Matrix4 generate_transformations() = 0;
		virtual void set_selected(bool selected) = 0;
		virtual bool intersect(atlas::math::Point p) = 0;
		virtual std::vector<atlas::math::Point> data() = 0;

		const static float _p_size;
		const static float _rp_size;
	};

	//const float handle::_p_size = 2.0f;
	//const float handle::_rp_size = _p_size / 1000.0f;
}