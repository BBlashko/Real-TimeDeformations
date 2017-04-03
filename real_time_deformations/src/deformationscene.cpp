#include <atlas\utils\GUI.hpp>
#include <deformationscene.hpp>
#include <handle.hpp>
#include <limits>
#include <pointhandle.hpp>

#include <iostream>

namespace rt_deformations
{
	deformation_scene::deformation_scene()
		: _add_point_handle_mode(false),
		_add_skeleton_handle_mode(false),
		_add_cage_handle_mode(false),
		_selected_handle(-1)
		{}

	void deformation_scene::mousePressEvent(
		int button, int action, int modifiers,
		double xPos, double yPos
		)
	{
		ModellingScene::mousePressEvent(button, action, modifiers, xPos, yPos);
		atlas::utils::Gui::getInstance().mousePressed(button, action, modifiers);

		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		GLint screen_width = viewport[2];
		GLint screen_height = viewport[3];

		if (button == GLFW_MOUSE_BUTTON_LEFT) {
			if (action == GLFW_PRESS) {

				auto new_point = glm::unProject(
					atlas::math::Point(xPos, screen_height - yPos, 0.0f),
					mView, mProjection,
					atlas::math::Point4(0, 0, screen_width, screen_height)
					);

				if (!edit_mode()) {

					std::cout << "It's broken" << std::endl;

					for (auto h : _hr.handles()) {
						//if (h->intersect(new_point)) {
							// When we do more than regular handles,
							// well will handle this case
						//	h->set_selected(true);
						//}
					}
					std::cout << "It's not broken" << std::endl;

				}
				else {
					_hr.add_handle(&point_handle(new_point));
					_add_point_handle_mode = false;
				}
			} else if (action == GLFW_RELEASE){
				auto handles = _hr.handles();
				if (_selected_handle >= 0 && _selected_handle < handles.size()) {
					handles.at(_selected_handle)->set_selected(false);
					_selected_handle = -1;
				}

			}
		}
	}

	void deformation_scene::mouseMoveEvent(double xPos, double yPos) {
		ModellingScene::mouseMoveEvent(xPos, yPos);
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		GLint screen_width = viewport[2];
		GLint screen_height = viewport[3];

		//if (_temp.selected()) {
			auto new_point = glm::unProject(
				atlas::math::Point(xPos, screen_height-yPos, 0.0f),
				mView, mProjection,
				atlas::math::Point4(0, 0, screen_width, screen_height)
				);
		//	_temp.set_point(new_point);
		//}
	}

	void deformation_scene::renderScene()
	{
		atlas::utils::Gui::getInstance().newFrame();
		const float grey = 161.0f / 255.0f;
		glClearColor(grey, grey, grey, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mProjection = glm::perspective(
			glm::radians(mCamera.getCameraFOV()),
			(float)mWidth / mHeight, 2.0f, 1000000.0f);

		mUniformMatrixBuffer.bindBuffer();
        mUniformMatrixBuffer.bufferSubData(0, sizeof(atlas::math::Matrix4),
            &mProjection[0][0]);
        mUniformMatrixBuffer.unBindBuffer();

        mView = mCamera.getCameraMatrix();
        mUniformMatrixBuffer.bindBuffer();
        mUniformMatrixBuffer.bufferSubData(sizeof(atlas::math::Matrix4),
            sizeof(atlas::math::Matrix4), &mView[0][0]);

        if (mShowGrid)
        {
            mGrid.renderGeometry(mProjection, mView);
        }

		_hr.renderGeometry(mProjection, mView);

		// Application interface

		// Camera Controls
		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
		ImGui::Begin("Deformations HUD");
		ImGui::Checkbox("Show grid", &mShowGrid);
		if (ImGui::Button("Reset Camera"))
		{
			mCamera.resetCamera();
		}

		// Handle Editor
		static int delete_handle_index = 0;
		static bool cursor_edit = false;
		ImGui::GetIO().MouseDrawCursor = true;

		ImGui::Spacing();
		ImGui::Separator();

		if (ImGui::BeginMenu("Create")) {
			ImGui::MenuItem("Point Handle", NULL, &_add_point_handle_mode);
			ImGui::MenuItem("Skeleton Handle", NULL, &_add_skeleton_handle_mode);
			ImGui::MenuItem("Cage Handle", NULL, &_add_cage_handle_mode);
			ImGui::EndMenu();
		}

		ImGui::Combo("", &delete_handle_index, "None\0P1\0P2\0P3\0P4\0\0");
		ImGui::SameLine();
		if (ImGui::Button("Delete")) {
		}

		if (edit_mode()) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Move);
		}

        ImGui::End();
        ImGui::Render();
	}

	bool deformation_scene::edit_mode() {
		return _add_point_handle_mode;
			//_add_skeleton_handle_mode ||
			//_add_cage_handle_mode;
	}
}
