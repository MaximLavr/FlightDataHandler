#include "VectorRemoval.h"

namespace sparky
{
    void VectorRemoval::remove_element(std::vector<graphics::Renderable2D*>& vec, int index)
    {
        std::swap(vec[index], vec[vec.size() - 1]);
        vec.pop_back();
    }
}
