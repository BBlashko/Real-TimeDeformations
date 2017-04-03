#pragma once

#include <atlas/math/Math.hpp>
#include <handle.hpp>

namespace rt_deformations {

	class point_handle : public handle {
	public:
		point_handle(atlas::math::Point point);

		virtual ~point_handle();

		virtual atlas::math::Matrix4 generate_transformations() override;

		bool selected();

		virtual bool intersect(atlas::math::Point p) override;

		void set_point(atlas::math::Point p);

		virtual void set_selected(bool selected) override;

		virtual std::vector<atlas::math::Point> data() override;

		atlas::math::Point _p;
	private:
		bool _s;
	};

}
