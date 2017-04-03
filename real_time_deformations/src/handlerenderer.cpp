#include <environment.hpp>
#include <handlerenderer.hpp>

#include <iostream>

namespace rt_deformations {

	handle_renderer::handle_renderer()
		: _size(0),
		_buffer(GL_ARRAY_BUFFER)
	{
		std::vector<atlas::gl::ShaderUnit> shaders = {
			{ get_shader_path() + "/deformations.fs.glsl", GL_FRAGMENT_SHADER },
			{ get_shader_path() + "/deformations.vs.glsl", GL_VERTEX_SHADER }
		};
		mShaders.push_back(shaders);
		mShaders[0].setShaderIncludeDir(get_shader_path());
		mShaders[0].compileShaders();
		mShaders[0].linkShaders();

		// Obtain our uniform variables from the shader
		for (auto key : { "color", "projection", "view", "model" }) {
			auto var = mShaders[0].getUniformVariable(key);
			mUniforms.insert(UniformKey(key, var));
		}

		mShaders[0].disableShaders();

		// Populate GPU with our control point
		_vao.bindVertexArray();
		_buffer.bindBuffer();
		_buffer.vertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE,
			0, atlas::gl::bufferOffset<float>(0)
			);
		_buffer.unBindBuffer();

		_vao.enableVertexAttribArray(0);
		_vao.unBindVertexArray();
	}

	void handle_renderer::renderGeometry(atlas::math::Matrix4 const& projection,
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
		glPointSize(handle::_p_size);
		glDrawArrays(GL_POINTS, 0, _size);
		glPointSize(1.0f);
		_vao.unBindVertexArray();

		mShaders[0].disableShaders();
	}

	void handle_renderer::add_handle(handle* h) {
		_handles.push_back(h);
		buffer_data();
	}

	void handle_renderer::buffer_data() {

		auto points = flatten_point_data();
		std::cout << points.size() << std::endl;

		_vao.bindVertexArray();
		_buffer.bindBuffer();
		_buffer.bufferData(
			atlas::gl::size<atlas::math::Point>(points.size()),
			points.data(),
			GL_STATIC_DRAW
			);
		_buffer.unBindBuffer();
		_vao.unBindVertexArray();

	}

	std::vector<atlas::math::Point> handle_renderer::flatten_point_data() {
		std::vector<atlas::math::Point> result;

		for (auto h : _handles) {
			auto v = h->data();
			result.insert(result.end(), v.begin(), v.end());
		}

		_size = result.size();

		return result;
	}

	void handle_renderer::update_handle(int handle_index, atlas::math::Point p) {
		if (handle_index >= 0 && handle_index < _handles.size()) {
			auto h = _handles.at(handle_index);
			h->set_point(p);
		}

		buffer_data();
	}

	std::vector<handle*> handle_renderer::handles() {
		return _handles;
	}
}