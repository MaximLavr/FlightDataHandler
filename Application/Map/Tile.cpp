#include "Tile.h"

namespace app_graphics
{
    Tile::Tile(float xPos, float yPos, float side, int zoom, int x, int y)
            : Renderable2D(maths::vec3(xPos, yPos, 0.0f), maths::vec2(side, side), transform_color(1, 1, 1, 1))
    {
        m_Position = vec2(xPos, yPos);
        m_Size = vec2(side, side);
        this->z = zoom;
        this->x = x;
        this->y = y;
    }

    void Tile::DrawPixelOn(vec2 latLong, int zoom, unsigned int color, Layer* layer, Window* window,
                       float cameraVertical, float cameraHorizontal, vec2 mapPos, vec2_i leftUpTileCoords)
    {
        /*cameraLeft = -11.67f;
        cameraRight = 11.67f;
        cameraBottom = -9.0f;
        cameraUp = 9.0f;
         */

        latLong = vec2(57.865210, 61.750525);
        auto mapPosPixels = app_utils::CoordsConverter::FloatToPixels(mapPos, window, cameraVertical, cameraHorizontal);
        std::cout << "mapPos = " << mapPosPixels << std::endl;

        float floatsInPixel = 23.34f / (float)window->getWidth();
        std::cout << "window width = " << (float)window->getWidth();
        std::cout << "floatsInPixel = " << floatsInPixel << std::endl;

        int pixelsInFloat = int(round((float)window->getWidth() / 23.34f));

        int mapSizePixels = int(round(10.0f * pixelsInFloat));
        std::cout << "mapSize = " << mapSizePixels << std::endl;

        int tilePixelsSide = int(round(((float)window->getWidth() * 2.5f) / 23.34f));
        std::cout << "tilePixelsSide = " << tilePixelsSide << std::endl; //204.8

        vec2_i pointCoordsInTile = app_utils::CoordsConverter::LatLongToTilePixelXY(latLong.x, latLong.y, zoom, tilePixelsSide);
        std::cout << "zoom = " << zoom << ", pointCoordsInTile = " << pointCoordsInTile << std::endl;

        int a = 0;

        //разрешение окна = 1400 * 1080
        //latLongToTilePixelXY
        /*for (int i = 0; i < tilePixelsSide; i++) {
            layer->add(new Sprite(mapPosPixels.x_i + a, mapPosPixels.y_i - mapSizePixels + 1,
                                  floatsInPixel, floatsInPixel, transform_color(0, 1, 0, 1),
                                  window, cameraVertical, cameraHorizontal));
            a++;
        }*/

        int counter = 0;
        int yDif = pow(2, 2 * zoom) - 1;
        for (int i = 0; i < 1; i++) {
            //window size = 1400 * 1080
            /*layer->add(new Sprite(mapPosPixels.x_i + pointCoordsInTile.x_i,
                                  mapPosPixels.y_i - mapSizePixels + pointCoordsInTile.y_i - counter,
                                  2 * floatsInPixel, 2 * floatsInPixel, color,
                                  window, cameraVertical, cameraHorizontal));

            counter+=1;*/

            int maxY = pow(2, 2 * zoom) - 1;
            //int diffY = leftUpTileCoords.y_i +
            if (zoom > 2)
            {
                layer->add(new Sprite(mapPosPixels.x_i + pointCoordsInTile.x_i - leftUpTileCoords.x_i * tilePixelsSide,
                                      mapPosPixels.y_i - mapSizePixels + pointCoordsInTile.y_i - leftUpTileCoords.y_i * tilePixelsSide - counter - tilePixelsSide,
                                      floatsInPixel, floatsInPixel , color, window, cameraVertical, cameraHorizontal));
            }
            else
            {
                layer->add(new Sprite(mapPosPixels.x_i + pointCoordsInTile.x_i - leftUpTileCoords.x_i * tilePixelsSide,
                                      mapPosPixels.y_i - mapSizePixels + pointCoordsInTile.y_i - leftUpTileCoords.y_i * tilePixelsSide - counter,
                                      floatsInPixel, floatsInPixel, color, window, cameraVertical, cameraHorizontal));
            }
            //layer->add(new Sprite(mapPosPixels.x_i + 1 ,mapPosPixels.y_i, floatsInPixel, floatsInPixel, color,window, cameraVertical, cameraHorizontal));
            //counter += 1;
        }

        /*layer->add(new Sprite(350, 270,
                                    3,  3, color, window, cameraVertical, cameraHorizontal));
        auto floatCoords = app_utils::CoordsConverter::PixelToFloats(vec2(0, 270), window, cameraVertical, cameraHorizontal );
        std::cout << "floatCoords = " << floatCoords << std::endl;
        auto pixelCoordsFinal = app_utils::CoordsConverter::FloatToPixels(floatCoords, window, cameraVertical, cameraHorizontal);

        layer->add(new Sprite(floatCoords.x, floatCoords.y, 3, 3, transform_color(1, 1, 1, 1)));
        layer->add(new Sprite(pixelCoordsFinal.x, pixelCoordsFinal.y, 2, 2, transform_color(1, 0, 1, 1), window, cameraVertical, cameraHorizontal));

        layer->add(new Sprite(mapPosPixels.x, mapPosPixels.y, 3.0f, 3.0f, transform_color(0.6f, 0.5f, 1.0f, 1.0f), window, cameraVertical, cameraHorizontal));

        std::cout << "Mask!" << std::endl;*/
    }
}
