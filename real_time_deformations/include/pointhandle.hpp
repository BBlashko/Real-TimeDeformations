#pragma once

#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <atlas/math/Math.hpp>
#include <handle.hpp>
#include <vector>

namespace rt_deformations {

	class point_handle : public handle {
	public:
		point_handle(atlas::math::Point point);
		virtual ~point_handle();

		virtual atlas::math::Matrix4 generate_transformations() override;
		virtual void renderGeometry() override;
	private:
		atlas::math::Point _p;
		atlas::gl::Buffer _buffer;
		atlas::gl::VertexArrayObject _vao;
	};

}
