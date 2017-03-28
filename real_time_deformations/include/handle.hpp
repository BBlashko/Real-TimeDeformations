#pragma once

#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <atlas/math/Math.hpp>
#include <atlas/utils/Geometry.hpp>

namespace rt_deformations {

	class handle : public atlas::utils::Geometry {
	public:
		handle();//GLenum buffer_enum);
	
		virtual atlas::math::Matrix4 generate_transformations() = 0;
		virtual void renderGeometry() override = 0;
	//protected:
	//	atlas::gl::Buffer _buffer;
	//	atlas::gl::VertexArrayObject _vao;
	};

}