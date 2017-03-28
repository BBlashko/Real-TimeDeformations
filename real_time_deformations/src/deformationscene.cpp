#include <atlas\utils\GUI.hpp>

#include <deformationscene.hpp>
#include <handle.hpp>
#include <pointhandle.hpp>

namespace rt_deformations
{
	deformation_scene::deformation_scene() : _temp(atlas::math::Point(0, 0, 0)) {}

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

		if (_edit_mode) {
			float new_x = ((float)xPos / screen_width) * 2 - 1;
			float new_y = ((float)yPos / screen_height) * -2 + 1;

			// Place our point
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
			(float)mWidth / mHeight, 1.0f, 1000000.0f);

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

		_temp.renderGeometry();

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
