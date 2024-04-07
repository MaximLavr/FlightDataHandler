#include "CoordsConverter.h"

namespace app_utils
{
    vec2 CoordsConverter::PixelToFloats(vec2_i pixelCoords, sparky::graphics::Window* window, float cameraVertical, float cameraHorizontal)
    {
        float floatX = 2.0f * pixelCoords.x_i * abs(cameraHorizontal) / window->getWidth() - abs(cameraHorizontal);
        float floatY = abs(cameraVertical) - 2.0f * pixelCoords.y_i * abs(cameraVertical) / window->getHeight();
        return vec2(floatX, floatY);
    }

    vec2_i CoordsConverter::FloatToPixels(vec2 floatCoords,sparky::graphics::Window* window, float cameraVertical, float cameraHorizontal)
    {
        //int pixelX = int(round(((floatCoords.x + abs(cameraHorizontal)) * (float)window->getWidth()) / abs(cameraHorizontal)));
        //int pixelY = int(round((abs(cameraVertical) - floatCoords.y) * (float)window->getHeight() / abs(cameraVertical)));
        int pixelX = int(round(((floatCoords.x + abs(cameraHorizontal)) * (float)window->getWidth()) / (2.0f * abs(cameraHorizontal))));
        int pixelY = int(round((abs(cameraVertical) - floatCoords.y) * (float)window->getHeight() / (2.0f * abs(cameraVertical))));

        return {pixelX, pixelY};
        //return vec4(pixelX, pixelY, floatCoords.x, floatCoords.y);
    }

    vec2_d CoordsConverter::TileToLatLong(int x, int y, int zoom)
    {
        double longitude = (double)x/ pow(2, zoom) * 360 - 180;

        auto n = M_PI - 2.0 * M_PI * y / pow(2, zoom);

        double latitude = 180.0 / M_PI * atan(0.5 * (exp(M_PI - 2.0 * M_PI * y / pow(2, zoom)) - exp(-n)));

        return {latitude, longitude};
    }

    vec2_i CoordsConverter::LatLongToTile(double latitude, double longitude, int zoom)
    {
        //was floor
        int n = pow(2,zoom);
        double latitude_radians = latitude * (M_PI / 180.0);

        //int x = (floor((longitude + 180.0) / 360.0 * (1 << zoom)));
        auto x = n * ((longitude + 180.0) / 360.0);
        auto y = 0.5 * n * (1 - (log(tan(latitude_radians) + (1/cos(latitude_radians))) /  M_PI));

        //int y = (floor((1.0 - asinh(tan(latitude_radians)) / M_PI) / 2.0 * (1 << zoom)));

        int ans_x = floor(x);
        int ans_y = floor(y);

        return {ans_x, ans_y};
    }

    double degreesToRadians(double degrees)
    {
        return degrees * M_PI / 180.0;
    }

    double radiansToDegrees(double radians)
    {
        return radians / M_PI * 180.0;
    }


    vec2_i CoordsConverter::LatLongToTilePixelXY(double latitude, double longitude, int zoom, int tileSizePixels)
    {
        auto c = ((double)tileSizePixels / (2.0 * M_PI)) * pow(2.0, zoom);
        auto pixelX = int(round((c * (degreesToRadians(longitude) + M_PI))));
        auto pixelY = int(round((c * (M_PI - log( tan((M_PI / 4.0) + degreesToRadians(latitude) / 2.0) )))));

        return {pixelX, pixelY};
    }

    vec2_i CoordsConverter::LatLongToTilePixelXYOnTile(double latitude, double longitude, int zoom, int tileSizePixels)
    {
        auto c = (tileSizePixels / (2.0 * M_PI)) * pow(2.0, zoom);
        auto pixelX = int(round((c * (degreesToRadians(longitude) + M_PI))));
        auto pixelY = int(round((c * (M_PI - log( tan((M_PI / 4.0) + degreesToRadians(latitude) / 2.0) )))));

        return {pixelX % tileSizePixels, pixelY % tileSizePixels};
    }

    vec2_d CoordsConverter::PixelXYToLatLong(vec2_i pixelCoords, int zoom)
    {
        auto c = (TILE_SIDE / (2.0 * M_PI)) * pow(2.0, zoom);

        auto longitudeRadians = pixelCoords.x_i / c - M_PI;
        auto latitudeRadians = 2.0 * (atan(pow(M_E, M_PI - pixelCoords.y_i / c)) - (M_PI / 4.0));

        auto latitude = radiansToDegrees(latitudeRadians);
        auto longitude = radiansToDegrees(longitudeRadians);

        return vec2_d(latitude, longitude);
    }

    /*double CoordsConverter::GetMetersPerPixel(double latitude, int zoom) {
        //return EQUATOR_LENGTH * cos(degreesToRadians(latitude)) / pow(2 ,(zoom + 9));
        return -1;
    }*/

    double CoordsConverter::GetDistBetweenPixelsInPixels(vec2_i firstPixelCoords, vec2_i secondPixelCoords) {
        auto a = firstPixelCoords.x_i - secondPixelCoords.x_i;
        auto b =  firstPixelCoords.y_i - secondPixelCoords.y_i;
        return sqrt(pow(a, 2) + pow(b, 2));
    }

    double CoordsConverter::GetDistBetweenLatLongPointsInMeters(vec2_d firstLatLongCoords, vec2_d secondLatLongCoords) {
        auto f1 = firstLatLongCoords.x_d * M_PI / 180.0;
        auto f2 = secondLatLongCoords.x_d * M_PI / 180.0;
        auto d_f = (firstLatLongCoords.x_d - secondLatLongCoords.x_d) * M_PI / 180.0;
        auto d_l = (firstLatLongCoords.y_d - secondLatLongCoords.y_d) * M_PI / 180.0;

        auto a = sin(d_f/2.0) * sin(d_f/2.0) + cos(f1) * cos(f2) * sin(d_l/2.0) * sin(d_l/2.0);
        auto c = 2.0 * atan2(sqrt(a), sqrt(1-a));

        return EARTH_RADIUS * c;
    }

    double CoordsConverter::GetDistBetweenXYPointsInMeters(vec2_i firstXYCoords, vec2_i secondXYCoords, int zoom)
    {
        auto firstLatLongCoords = PixelXYToLatLong(firstXYCoords, zoom);
        auto secondLatLongCoords = PixelXYToLatLong(secondXYCoords, zoom);

        std::cout << "firstLatLongCoords = vec2_d(" << firstLatLongCoords.x_d << ", " << firstLatLongCoords.y_d << ")\n";
        std::cout << "secondLatLongCoords = vec2_d(" << secondLatLongCoords.x_d << ", " << secondLatLongCoords.y_d << ")\n";

        return GetDistBetweenLatLongPointsInMeters(firstLatLongCoords, secondLatLongCoords);
    }

    vec2_i CoordsConverter::LatLongToPixelXYRelative(double latitude, double longitude, int zoom, int pixelSide)
    {
        auto c = (pixelSide / (2.0 * M_PI)) * pow(2.0, zoom);

        int x = int(round((c * (degreesToRadians(longitude) + M_PI))));
        int y = int(round((c * (M_PI - log( tan((M_PI / 4.0) + degreesToRadians(latitude) / 2.0) )))));

        return {x, y};
    }

    vec2_i CoordsConverter::LatLongToPixelXYOnMap(double latitude, double longitude, int zoom, vec2_i mapPositionPixels, int mapSizePixels) {
        //auto pixelXY = app_utils::CoordsConverter::LatLongToPixelXY(latitude, longitude, zoom);
        int pixelsInTile = int(round(mapSizePixels / 4.0f));

        auto pixelXYRel = app_utils::CoordsConverter::LatLongToPixelXYRelative(latitude, longitude, zoom, pixelsInTile);
        std::cout << "pixel_X = " << pixelXYRel.x_i << ", pixel_Y = " << pixelXYRel.y_i << std::endl;

        //получается всегда правее на 1 пиксель, поэтому вычел 1
        return {mapPositionPixels.x_i + pixelXYRel.x_i, mapPositionPixels.y_i + mapSizePixels - pixelXYRel.y_i };
    }

    vec2_d CoordsConverter::GetLatLongFromPointDistanceAngle(vec2_d latLongCenter, double distanceMeters, double angleDegrees)
    {
        auto angleRadians = degreesToRadians(angleDegrees);
        auto lat1 = degreesToRadians(latLongCenter.x_d);
        auto lon1 = degreesToRadians(latLongCenter.y_d);

        auto lat2 = asin(sin(lat1) * cos(distanceMeters/EARTH_RADIUS) + cos(lat1) * sin(distanceMeters/EARTH_RADIUS) * cos(angleRadians));
        auto lon2 = lon1 + atan2(sin(angleRadians) * sin(distanceMeters/EARTH_RADIUS) * cos(lat1),cos(distanceMeters/EARTH_RADIUS) - sin(lat1) * sin(lat2));

        auto lat2Degrees = radiansToDegrees(lat2);
        auto lon2Degrees = radiansToDegrees(lon2);

        return {lat2Degrees, lon2Degrees};
    }

    double CoordsConverter::GetScaleOfZoom(int zoom) {
        //return round(log(591657550.500000 /(zoom/2))/log(2));
        //return round(591657550.500000 / (pow(2, zoom)));
        return round(591657550.500000 / (pow(2, zoom)));
    }

    std::string CoordsConverter::ScaleToString(int zoom)
    {
        return scalesString[zoom];
    }

    double CoordsConverter::GetMetersPerPixel(int zoom) {
        //return 2.0 * M_PI * (double)EARTH_RADIUS / (double)(TILE_SIDE * pow(2, zoom));
        return M_PI * (double)EARTH_RADIUS / (double)(TILE_SIDE * pow(2, zoom));
    }
}