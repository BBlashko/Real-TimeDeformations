#include <pointhandle.hpp>

#include <iostream>

namespace rt_deformations {

	point_handle::point_handle(atlas::math::Point point)
		: _p(point),
		_s(false),
		handle(GL_ARRAY_BUFFER)
	{
		// Populate GPU with our control point
		_vao.bindVertexArray();
		_buffer.bindBuffer();
		_buffer.bufferData(
			sizeof(atlas::math::Point),
			&_p,
			GL_STATIC_DRAW
			);
		_buffer.vertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE,
			0, atlas::gl::bufferOffset<float>(0)
			);
		_buffer.unBindBuffer();
		
		_vao.enableVertexAttribArray(0);
		_vao.unBindVertexArray();
		
	}

	point_handle::~point_handle() {}

	atlas::math::Matrix4 point_handle::generate_transformations()
	{
		return atlas::math::Matrix4(1.0f);
	}

	void point_handle::renderGeometry(atlas::math::Matrix4 const& projection,
		atlas::math::Matrix4 const& view)
	{
		if (!mShaders[0].shaderProgramValid()) {
			//TODO Error log
			return;
		}

		mShaders[0].enableShaders();

		glUniform4f(mUniforms["color"], 1, 1, 0, 1);
		glUniformMatrix4fv(mUniforms["model"], 1, GL_FALSE, &mModel[0][0]);
		glUniformMatrix4fv(mUniforms["projection"], 1, GL_FALSE, &projection[0][0]);
		glUniformMatrix4fv(mUniforms["view"], 1, GL_FALSE, &view[0][0]);

		_vao.bindVertexArray();
		glPointSize(_p_size);
		glDrawArrays(GL_POINTS, 0, 1);
		glPointSize(1.0f);
		_vao.unBindVertexArray();

		mShaders[0].disableShaders();
	}

	void point_handle::set_point(atlas::math::Point p) {
		_p = p;

		_vao.bindVertexArray();
		_buffer.bindBuffer();
		_buffer.bufferData(
			sizeof(atlas::math::Point),
			&_p,
			GL_STATIC_DRAW
			);
		_buffer.unBindBuffer();
		_vao.unBindVertexArray();

		std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
	}

	bool point_handle::intersect(atlas::math::Point p) {
		float delta_x = abs(abs(p.x) - abs(_p.x));
		float delta_y = abs(abs(p.y) - abs(_p.y));

		std::cout << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;

		return delta_x <= _rp_size && delta_y <= _rp_size;
	}

	void point_handle::set_selected(bool selected) {
		_s = selected;
	}

	bool point_handle::selected() {
		return _s;
	}
}