#pragma once
#include <vector>
#include "../graphics/renderable2d.h"

namespace sparky
{
    class VectorRemoval {
    public:
        static void remove_element(std::vector<graphics::Renderable2D*>& vec, int index);
    };
}
