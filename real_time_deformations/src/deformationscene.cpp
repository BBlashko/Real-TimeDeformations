#include <atlas\utils\GUI.hpp>
#include <deformationscene.hpp>
#include <handle.hpp>
#include <limits>
#include <pointhandle.hpp>

namespace rt_deformations
{
	deformation_scene::deformation_scene()
		: _temp(atlas::math::Point(0.0f, 0.0f, 0.0f)),
		_selected_point(-1)
		{}

	void deformation_scene::mousePressEvent(
		int button, int action, int modifiers,
		double xPos, double yPos
		)
	{
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

				if (_temp.intersect(new_point)) {
					_temp.set_selected(true);
				}
			} else if (action == GLFW_RELEASE){
				_temp.set_selected(false);
				_selected_point = -1;
			}
		}
	}

	void deformation_scene::mouseMoveEvent(double xPos, double yPos) {
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		GLint screen_width = viewport[2];
		GLint screen_height = viewport[3];

		if (_temp.selected()) {
			auto new_point = glm::unProject(
				atlas::math::Point(xPos, screen_height-yPos, 0.0f),
				mView, mProjection,
				atlas::math::Point4(0, 0, screen_width, screen_height)
				);
			_temp.set_point(new_point);
		}
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

		_temp.renderGeometry(mProjection, mView);

        ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Deformations HUD");
        ImGui::Checkbox("Show grid", &mShowGrid);
        if (ImGui::Button("Reset Camera"))
        {
            mCamera.resetCamera();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
            ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Render();
	}
}
