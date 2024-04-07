#pragma once
#include <cmath>
#include "../../SparkyEngine/src/engine.h"

//256 side tile: Spixel = Stile / 256 = EQUATOR_LENGTH ∙ cos(latitude) / 2 (zoomlevel + 8)
//512 side tile: Spixel = Stile / 512 = EQUATOR_LENGTH ∙ cos(latitude) / 2 (zoomlevel + 9)
using namespace sparky;
using namespace graphics;
using namespace maths;

namespace app_utils
{
    const double EQUATOR_LENGTH = 40075696;
    const double METERS_PER_PIXEL_0_ZOOM = 78271.516964;
    const int EARTH_RADIUS = 6371000;
    const int TILE_SIDE = 512;
    static const double PIXELS_IN_ONE_FLOAT = 0.00488281;
    static const double FLOATS_IN_ONE_PIXEL = 1.0 / PIXELS_IN_ONE_FLOAT;
    static const double EARTH_FULL_AREA_KM2 = 510065600;

    static const std::string scalesString[24] = {
            "1:500000000",
            "1:250000000",
            "1:150000000",
            "1:70000000",
            "1:35000000",
            "1:15000000",
            "1:10000000",
            "1:4000000",
            "1:2000000",
            "1:1000000",
            "1:500000",
            "1:250000",
            "1:150000",
            "1:70000",
            "1:35000",
            "1:15000",
            "1:8000",
            "1:4000",
            "1:2000",
            "1:1000",
            "1:500",
            "1:250",
            "1:100",
            "1:50",
    };

    class CoordsConverter {
    private:
    public:
        static vec2_d TileToLatLong(int x, int y, int zoom);
        static vec2_i LatLongToTile(double latitude, double longitude, int zoom);

        static vec2_i LatLongToTilePixelXY(double latitude, double longitude, int zoom, int tileSizePixels); //new
        static vec2_i LatLongToTilePixelXYOnTile(double latitude, double longitude, int zoom, int tileSizePixels); //new

        static vec2_d PixelXYToLatLong(vec2_i pixelCoords, int zoom);
        static double GetMetersPerPixel(double latitude, int zoom);
        static double GetDistBetweenPixelsInPixels(vec2_i firstPixelCoords, vec2_i secondPixelCoords);
        static double RadiansToDegrees(double angleRadians);

        static double GetDistBetweenLatLongPointsInMeters(vec2_d firstLatLongCoords, vec2_d secondLatLongCoords);
        static double GetDistBetweenXYPointsInMeters(vec2_i firstXYCoords, vec2_i secondXYCoords, int zoom); //?

        static vec2 PixelToFloats(vec2_i pixelCoords, Window* window, float cameraVertical, float cameraHorizontal);
        static vec2_i FloatToPixels(vec2 floatCoords, Window* window, float cameraVertical, float cameraHorizontal);

        static vec2_i LatLongToPixelXYOnMap(double latitude, double longitude, int zoom, vec2_i mapPositionPixels, int MapSizePixels);
        static vec2_i LatLongToPixelXYRelative(double latitude, double longitude, int zoom, int pixelSide);

        static vec2_d GetLatLongFromPointDistanceAngle(vec2_d latLongCenter, double distanceMeters, double angleRadians);

        static double GetScaleOfZoom(int zoom);

        static std::string ScaleToString(int zoom);

        static double GetMetersPerPixel(int zoom);
    };
}


