#pragma once
#include <atlas/tools/ModellingScene.hpp>
#include <atlas/utils/GUI.hpp>
#include <atlas/gl/GL.hpp>
#include <atlas/utils/GUI.hpp>
#include <GLFW\glfw3.h>
#include <GL/GL.h>
#include <vector>
#include <iostream>

namespace project
{
    class DeformationScene : public atlas::tools::ModellingScene
    {
    public:
        DeformationScene::DeformationScene();
        void DeformationScene::renderScene() override;
    private:
    };
}
