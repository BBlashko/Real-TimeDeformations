#pragma once

#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <atlas/math/Math.hpp>
#include <atlas/utils/Geometry.hpp>

namespace rt_deformations {

	class handle : public atlas::utils::Geometry {
	public:
		handle(GLenum buffer_enum);
	
		virtual atlas::math::Matrix4 generate_transformations() = 0;
		virtual void renderGeometry(atlas::math::Matrix4 const& projection,
			atlas::math::Matrix4 const& view) = 0;
		virtual void set_selected(bool selected) = 0;
	protected:
		atlas::gl::Buffer _buffer;
		atlas::gl::VertexArrayObject _vao;
		const float _p_size = 15.0f;
		const float _rp_size = _p_size / 1000.0f;
	};

}