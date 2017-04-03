#include <pointhandle.hpp>

#include <iostream>

namespace rt_deformations {

	point_handle::point_handle(atlas::math::Point point)
		: _p(point),
		_s(false)
	{}

	point_handle::~point_handle() {}

	atlas::math::Matrix4 point_handle::generate_transformations()
	{
		return atlas::math::Matrix4(1.0f);
	}

	void point_handle::set_point(atlas::math::Point p) {
		_p = p;
	}

	bool point_handle::intersect(atlas::math::Point p) {

		float delta_x = abs(abs(p.x) - abs(_p.x));
		float delta_y = abs(abs(p.y) - abs(_p.y));

		//std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;

		return delta_x <= _rp_size && delta_y <= _rp_size;
	}

	void point_handle::set_selected(bool selected) {
		_s = selected;
	}

	bool point_handle::selected() {
		return _s;
	}

	std::vector<atlas::math::Point> point_handle::data() {
		return { _p };
	}
}