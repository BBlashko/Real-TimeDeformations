#include <pointhandle.hpp>

namespace rt_deformations {

	point_handle::point_handle(atlas::math::Point point)
		: _p(point), _buffer(GL_ARRAY_BUFFER)//,
		//handle(GL_ARRAY_BUFFER)
	{
		_vao.bindVertexArray();

		_buffer.bindBuffer();
		_buffer.bufferData(
			sizeof(atlas::math::Point),
			&_p,
			GL_DYNAMIC_DRAW
			);
		_buffer.vertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE,
			0, atlas::gl::bufferOffset<float>(0)
			);
		//_vao.enableVertexAttribArray(0);
		_buffer.unBindBuffer();
		
		_vao.unBindVertexArray();
		//mShaders[0].disableShaders();
	}

	point_handle::~point_handle() {}

	atlas::math::Matrix4 point_handle::generate_transformations()
	{
		return atlas::math::Matrix4(1.0f);
	}

	void point_handle::renderGeometry()
	{
		if (!mShaders[0].shaderProgramValid()) {
			//TODO Error log
			return;
		}

		mShaders[0].enableShaders();

		_vao.bindVertexArray();
		glPointSize(10.0f);
		glUniform4f(mUniforms["color"], 0, 0, 0, 1);
		glDrawArrays(GL_POINTS, 0, 1);
		glPointSize(1.0f);
		_vao.unBindVertexArray();

		mShaders[0].disableShaders();
	}
}