#pragma once

#include <atlas/gl/Buffer.hpp>
#include <atlas/gl/VertexArrayObject.hpp>
#include <atlas/math/Math.hpp>
#include <atlas/utils/Geometry.hpp>
#include <handle.hpp>
#include <vector>

namespace rt_deformations {

	class handle_renderer : public atlas::utils::Geometry {
	public:
		handle_renderer();

		virtual void renderGeometry(atlas::math::Matrix4 const& projection,
			atlas::math::Matrix4 const& view) override;

		void add_handle(handle* h);
		void pop_handle(int index);
		void update_handle(int handle_index, atlas::math::Point p);

		std::vector<handle*> handles();
	private:
		std::vector<handle*> _handles;
		GLuint _size;
		atlas::gl::Buffer _buffer;
		atlas::gl::VertexArrayObject _vao;

		void buffer_data();
		std::vector<atlas::math::Point> flatten_point_data();
	};

}