#pragma once

#include <atlas/tools/ModellingScene.hpp>
#include <handle.hpp>
#include <handlerenderer.hpp>
#include <pythagorastree.hpp>
#include <vector>

namespace rt_deformations
{
	class deformation_scene : public atlas::tools::ModellingScene
	{
	public:
		deformation_scene::deformation_scene();

		void mousePressEvent(
			int button, int action, int modifiers,
			double xPos, double yPos
			) override;

		void mouseMoveEvent(double xPos, double yPos) override;

		void deformation_scene::renderScene() override;

	private:
		/* Determine if the user is adding a handle or not */
		bool _add_point_handle_mode;
		bool _add_skeleton_handle_mode;
		bool _add_cage_handle_mode;
		handle_renderer _hr;
		int _selected_handle;
		a3::pythagoras_tree _tree;

		bool edit_mode();
	};
}
