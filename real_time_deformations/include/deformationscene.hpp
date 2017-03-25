#pragma once

#include <atlas/tools/ModellingScene.hpp>

namespace rt_deformations
{
    class deformation_scene : public atlas::tools::ModellingScene
    {
    public:
        deformation_scene::deformation_scene();
        void deformation_scene::renderScene() override;
    private:
    };
}
