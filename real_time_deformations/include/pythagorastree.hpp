#pragma once

#include <atlas\gl\Buffer.hpp>
#include <atlas\gl\ShaderUnit.hpp>
#include <atlas\gl\VertexArrayObject.hpp>
#include <atlas\utils\Geometry.hpp>
#include <limits>
#include <string>
#include <tuple>
#include <vector>

namespace a3 {
	
	class pythagoras_tree : public atlas::utils::Geometry {

	public:
		using shaders = std::vector<atlas::gl::ShaderUnit>;

		pythagoras_tree(int axiom);

		virtual void resetGeometry();
		virtual void transformGeometry(atlas::math::Matrix4 const& t);
		void renderGeometry(bool regen);

		std::string generate_output_string(int level, int p);
	private:
		int _axiom;
		std::string _system;
		const int _MAX_INT = std::numeric_limits<int>::max();
		std::vector<atlas::math::Point> _vertices;
		std::vector<atlas::math::Point> _transformed_vertices;
		std::vector<GLuint> _indicies;

		atlas::gl::VertexArrayObject _vertex_vao;
		atlas::gl::Buffer _vertex_buff;
		atlas::gl::Buffer _index_buff;

		std::string recurse(std::string input_string, int p);
		std::tuple<std::vector<atlas::math::Point>, std::vector<GLuint>> generate_vertex_data();
		GLuint get_max_index(std::vector<GLuint> indicies);
	};

}