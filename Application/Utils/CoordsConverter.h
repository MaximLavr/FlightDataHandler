#pragma once
#include <cmath>
#include "../../SparkyEngine/src/engine.h"

namespace app_utils
{
    class CoordsConverter {
    private:
    public:
        static vec2_d TileToLongLat(int x, int y, int z);
        static vec2_d LongLatToTile(float longitude, float latitude, int z);
    };
}


