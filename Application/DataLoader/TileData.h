#pragma once

#include <fstream>
#include <iostream>

namespace app_loader
{
    class TileData {
    public:
        TileData()
                : zoom(0), x(0), y(0), XPosOnMap(0.0), YPosOnMap(0.0), size(0), startPoint(0) { }
        int zoom;
        int x;
        int y;

        double XPosOnMap;
        double YPosOnMap;

        int size;
        unsigned long startPoint;

        double stepLatitude;
        double stepLongitude;

        int countTilesBefore;
    };
}