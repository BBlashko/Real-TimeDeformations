#include <atlas\utils\GUI.hpp>

#include <deformationscene.hpp>
#include "imguiwrapper.hpp"

namespace rt_deformations
{
    //deformation_scene::deformation_scene() : _image("images/bat.png") {
    deformation_scene::deformation_scene() : _image("images/siimba_2.png") {
    //deformation_scene::deformation_scene() : _image("C:/GitWorkspace/Real-TimeDeformations/images/simmba.jpg") {
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

        _image.show_grid(mShowGrid);
        _image.renderGeometry(mProjection, mView);

        ImGui::SetNextWindowSize(ImVec2(300, 200), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Deformations HUD");
        
        //Load images
        ImGui::Text("Image Properties:");
        ImGui::BeginChild("Image Select:", ImVec2(295, 100), true);
        ImGui::Text("File Path:");

        char path[100] = "";
        ImGui::InputText("", path, 100);
        ImGui::SameLine();
        if (ImGui::Button("Load Image!"))
        {
            //TODO: Input Text is not working...
            //load image
        }
        ImGui::Text("Select Image:");

        int curr_image = _image.curr_image();
        ImGui::Combo("", &_image.curr_image(), _image.image_names());
        if (curr_image != _image.curr_image())
        {
            _image.load_image();
            _image.update_image();
        }
        ImGui::EndChild();

        //Image Properties
        ImGui::Text("Image Properties:");
        ImGui::BeginChild("Image Properties:", ImVec2(295, 100), true);
        if (ImGui::Button("Reset Camera"))
        {
            mCamera.resetCamera();
        }
        ImGui::Checkbox("Show grid", &mShowGrid);
        int scale = _image.scale();
        ImGui::InputInt("Scale", &scale);
        if (scale < 0) 
            scale = 0;
        if (scale != _image.scale())
        {
            _image.scale() = scale;
            _image.update_image();
        }
        ImGui::EndChild();

        //Handles
        ImGui::Text("Handles:");
        ImGui::BeginChild("Handles:", ImVec2(295, 100), true);
        //TODO: add handles
        ImGui::EndChild();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
            ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
        ImGui::Render();
    }
}
