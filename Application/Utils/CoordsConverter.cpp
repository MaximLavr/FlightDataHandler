#include "CoordsConverter.h"

namespace app_utils
{
    vec2_d CoordsConverter::TileToLongLat(int x, int y, int z)
    {
        double longitude = (float)x/ pow(2, z) * 360 - 180;

        auto n = M_PI - 2 * M_PI * y / pow(2, z);

        double latitude = 180.0 / M_PI * atan(0.5 * (exp(M_PI - 2 * M_PI * y / pow(2, z)) - exp(-n)));

        return {longitude, latitude};
    }

    vec2_d CoordsConverter::LongLatToTile(float longitude, float latitude, int z)
    {
        int x = (int)(floor((longitude + 180.0) / 360.0 * (1 << z)));

        double latitude_radians = latitude * M_PI/180.0;
        int y = (int)(floor((1.0 - asinh(tan(latitude_radians)) / M_PI) / 2.0 * (1 << z)));

        return {(double)x, (double)y};
    }
}
