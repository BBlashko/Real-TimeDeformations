#include <pythagorastree.hpp>
#include <algorithm>
#include <atlas\core\Log.hpp>
#include <iostream>
#include <functional>
#include <random>

namespace a3 {

	constexpr auto grid_vs =
		R"(#version 430 core
			layout (location = 0) in vec3 vPosition;

			void main()
			{
				gl_Position = vec4(vPosition, 1.0);
			}
		)";

	constexpr auto grid_fs =
		R"(#version 430 core
			uniform vec4 color;
			out vec4 fragColor;
			void main()
			{
				fragColor = color;
			}
		)";

	pythagoras_tree::pythagoras_tree(int axiom = 0)
		: _axiom(axiom),
		_vertex_buff(GL_ARRAY_BUFFER),
		_index_buff(GL_ELEMENT_ARRAY_BUFFER) {

		// Load our shaders
		shaders shaders = {
			{ grid_vs, GL_VERTEX_SHADER, true },
			{ grid_fs, GL_FRAGMENT_SHADER, true }
		};
		this->mShaders.push_back(atlas::gl::Shader(shaders));
		this->mShaders[0].compileShaders();
		this->mShaders[0].linkShaders();

		// Set uniform variables and turn the shaders off
		auto color = this->mShaders[0].getUniformVariable("color");
		this->mUniforms.insert(UniformKey("color", color));
		this->mShaders[0].disableShaders();
	}

	std::string pythagoras_tree::generate_output_string(int level, int p) {
		std::string result = std::to_string(this->_axiom);

		for (int i = 0; i < level; ++i) {
			result = this->recurse(result, p);
		}

		this->_system = result;
		return result;
	}

	void pythagoras_tree::transformGeometry(atlas::math::Matrix4 const& t) {
		std::vector<atlas::math::Point> result;
		for (auto p : _transformed_vertices) {
			result.emplace_back(t * atlas::math::Point4(p, 1.0f));
		}

		_transformed_vertices = result;
	}

	void pythagoras_tree::resetGeometry() {
		this->_transformed_vertices = this->_vertices;
	}


	void pythagoras_tree::renderGeometry(bool regen) {

		// Activate the shaders
		if (!mShaders[0].shaderProgramValid()) {
			return;
		}
		this->mShaders[0].enableShaders();

		// Generate point
		if (regen) {
			auto data = this->generate_vertex_data();
			this->_vertices = std::get<0>(data);
			this->_indicies = std::get<1>(data);
		}

		glUniform4f(this->mUniforms["color"], 1.0f, 1.0f, 1.0f, 1.0f);
		this->_vertex_vao.bindVertexArray();

		if (_transformed_vertices.size() == 0) {
			_transformed_vertices = _vertices;
		}

		// Send points to the GPU
		this->_vertex_buff.bindBuffer();
		this->_vertex_buff.bufferData(
			atlas::gl::size<atlas::math::Point>(_transformed_vertices.size()),
			_transformed_vertices.data(),
			GL_STATIC_DRAW
			);
		this->_vertex_buff.vertexAttribPointer(
			0, 3, GL_FLOAT, GL_FALSE,
			0, atlas::gl::bufferOffset<float>(0)
			);
		this->_vertex_vao.enableVertexAttribArray(0);
		this->_vertex_buff.unBindBuffer();

		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(this->_MAX_INT);

		// Draw points
		glDrawElements(GL_LINE_STRIP, _indicies.size(), GL_UNSIGNED_INT, _indicies.data());
		glDisable(GL_PRIMITIVE_RESTART);

		this->_vertex_vao.disableVertexAttribArray(0);
		this->_vertex_vao.unBindVertexArray();

		this->mShaders[0].disableShaders();
	}

	/**
	 *	Current Basis is 0.025f for distance
	 */
	std::tuple<std::vector<atlas::math::Point>, std::vector<GLuint>> pythagoras_tree::generate_vertex_data() {
		std::vector<atlas::math::Point> vertices;
		std::vector<GLuint> indicies;

		constexpr float leaf_size = 0.0125f;
		constexpr float size = 0.025f;

		// start is the origin
		vertices.push_back(atlas::math::Point(0.0f, 0.0f, 0.0f));
		indicies.push_back(0);

		float deg = 0.0f;
		// Stack counters
		int push_count = 0;
		int pop_count = 0;
		int i;
		// Stack
		std::vector<std::tuple<GLuint, float>> pos_stack;
		std::tuple <GLuint, float> back;
		// Random angles
		std::mt19937 g;
		g.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> range(0, 90);

		for (int i = 0; i < this->_system.size(); ++i) {
			char curr = this->_system.at(i);
			atlas::math::Point next(0.0f, 0.0f, 0.0f);

			switch (curr) {
			case '0':
				next.y += leaf_size;
				break;
			case '1':
				next.y += size;
				break;
			case '[':
				// Push back point
				pos_stack.push_back(
					std::make_tuple(
						indicies.back(),
						deg
						)
					);
				deg += 45.0f;
				push_count++ ;
				break;
			case '{':
				// Push back point
				pos_stack.push_back(
					std::make_tuple(
						indicies.back(),
						deg
						)
					);
				deg += range(g);
				push_count++;
				break;
			case ']':
				// Pop back last point
				back = pos_stack.back();
				pos_stack.pop_back();
				indicies.push_back(this->_MAX_INT);
				indicies.push_back(std::get<0>(back));
				deg = std::get<1>(back) - 45.0f;
				break;
			case '}':
				// Pop back last point
				back = pos_stack.back();
				pos_stack.pop_back();
				indicies.push_back(this->_MAX_INT);
				indicies.push_back(std::get<0>(back));
				deg = std::get<1>(back) - range(g);
				break;
			default:
				atlas::core::Log::log(
					atlas::core::Log::SeverityLevel::CRITICAL,
					"Unknown system char %c", curr
					);
				break;
			}

			if (next == vertices.at(0)) {
				continue;
			}

			if (deg != 0.0f) {
				glm::mat4 rotation(1);
				rotation = glm::rotate(
					rotation,
					glm::radians(deg),
					atlas::math::Point(0, 0, 1)
					);
				next = glm::vec3(rotation * glm::vec4(next, 1.0f));
			}

			vertices.push_back(next + vertices.at(indicies.back()));
			indicies.push_back(this->get_max_index(indicies) + 1U);
		}

		return std::make_tuple(vertices, indicies);
	}

	std::string pythagoras_tree::recurse(std::string input_string, int p) {
		std::string result = "";
		std::mt19937 g;
		g.seed(std::random_device()());
		std::uniform_int_distribution<std::mt19937::result_type> range(1, 100);
		int random_num = range(g);
		std::cout << random_num << std::endl;

		for (int i = 0; i < input_string.length(); ++i) {
			char curr = input_string.at(i);

			if (curr == '0') {
				if (random_num <= p) {
					result += "1{0}0";
				}
				else {
					result += "1[0]0";
				}
			}
			else if (curr == '1') {
				result += "11";
			}
			else {
				result += curr;
			}
		}

		return result;
	}

	GLuint pythagoras_tree::get_max_index(std::vector<GLuint> indicies) {
		GLuint max = 0;
		for (auto index : indicies) {
			if (index == this->_MAX_INT) continue;
			max = std::max<int>(max, index);
		}

		return max;
	}
}