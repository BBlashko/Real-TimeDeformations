#include <atlas\gl\ShaderUnit.hpp>
#include <environment.hpp>
#include <handle.hpp>

namespace rt_deformations {

	handle::handle()//GLenum buffer_enum) : _buffer(buffer_enum)
	{
		std::vector<atlas::gl::ShaderUnit> shaders = {
			{get_shader_path() + "/deformations.fs.glsl", GL_FRAGMENT_SHADER},
			{get_shader_path() + "/deformations.vs.glsl", GL_VERTEX_SHADER}
		};
		mShaders.push_back(shaders);
		mShaders[0].setShaderIncludeDir(get_shader_path());
		mShaders[0].compileShaders();
		mShaders[0].linkShaders();

		// Obtain our uniform variables from the shader
		for (auto key : {"color", "projection", "view", "model"}) {
			auto var = mShaders[0].getUniformVariable(key);
			mUniforms.insert(UniformKey(key, var));
		}

		mShaders[0].disableShaders();
	}

}