#include <atlas\utils\GUI.hpp>

#include <deformationscene.hpp>

namespace rt_deformations
{
    deformation_scene::deformation_scene() {}

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
