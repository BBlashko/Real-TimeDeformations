#pragma once

#include <atlas/tools/ModellingScene.hpp>
#include <vector>
#include <pointhandle.hpp>

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
		bool _edit_mode;
		point_handle _temp;
		int _last_x;
		int _last_y;
		int _selected_point;
	};
}
