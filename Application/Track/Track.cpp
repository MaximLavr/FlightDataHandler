#include "Track.h"
#include <cmath>
#include <float.h>

app_graphics::Track::Track(std::string pathToDirPoints, unsigned int color, Shader* shader,
                           mat4 projectionMatrix, Window *window, int scalePointSize)
    : Layer(new BatchRenderer2D(window), shader, projectionMatrix, false)
{
    m_TrackLoader = new TrackLoader(pathToDirPoints);
    m_Color = color;

    //m_FileNameTxt = pathToTxtFile;
    m_ScalePointSize = scalePointSize;
    //std::ifstream in_file(m_FileNameTxt);

    m_Window = window;
    CenterLatLong = m_TrackLoader->GetCenterLatLong();
    SideMeters = m_TrackLoader->GetSideMeters();
    //CenterLatLong = m_TrackLoader->getCenterLatLong();
    //SideMeters = m_TrackLoader->getSideMeters();
}


double app_graphics::Track::GetFromBinaryMinLatitude()
{
    std::ifstream binary(FileNameBinary, std::ios::in | std::ios::binary);

    auto size = 2 * sizeof(int) + CountCoords * sizeof(double) + CountDataString * CountCoords * sizeof(double);
    binary.seekg(size);

    double minLatitude;
    binary.read((char*)&minLatitude, sizeof(double));
    binary.close();

    return minLatitude;
}

double app_graphics::Track::GetFromBinaryMaxLatitude()
{
    std::ifstream binary(FileNameBinary, std::ios::in | std::ios::binary);

    auto size = 2 * sizeof(int) + CountCoords * sizeof(double) + CountDataString * CountCoords * sizeof(double) + sizeof(double);
    binary.seekg(size);

    double maxLatitude;
    binary.read((char*)&maxLatitude, sizeof(double));
    binary.close();

    return maxLatitude;
}

double app_graphics::Track::GetFromBinaryMinLongitude()
{
    std::ifstream binary(FileNameBinary, std::ios::in | std::ios::binary);

    auto size = 2 * sizeof(int) + CountCoords * sizeof(double) + CountDataString * CountCoords * sizeof(double) + 2 * sizeof(double);
    binary.seekg(size);

    double minLongitude;
    binary.read((char*)&minLongitude, sizeof(double));
    binary.close();

    return minLongitude;
}

double app_graphics::Track::GetFromBinaryMaxLongitude()
{
    std::ifstream binary(FileNameBinary, std::ios::in | std::ios::binary);

    auto size = 2 * sizeof(int) + CountCoords * sizeof(double) + CountDataString * CountCoords * sizeof(double) + 3 * sizeof(double);
    binary.seekg(size);

    double maxLongitude;
    binary.read((char*)&maxLongitude, sizeof(double));
    binary.close();

    return maxLongitude;
}

static bool isDoubleEqual(double inValue, double inCompareTo)
{
    return std::fabs(inValue - inCompareTo) <= DBL_EPSILON;
}

void app_graphics::Track::AddPoint(vec2 latLong, vec2 mapPos, float mapSize, int zoom, unsigned int color, Window *window, size_t pointerSize, vec2_i leftUpTileCoords) {
    auto mapPosPixels = app_utils::CoordsConverter::FloatToPixels(mapPos, window, window->getHeight() / 60.0f, window->getWidth() / 60.0f);

    int tileSideInPixels = int(round(((float)m_Window->getWidth() * (mapSize / 4.0f)) / (m_Window->getWidth() / 60.0f)));
    tileSideInPixels = int(round((float)tileSideInPixels / (float)2));
    vec2_i fullMapPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(latLong.x, latLong.y, zoom, tileSideInPixels);
    float floatsInPixel = 2.0f * (window->getWidth() / 60.0f) / (float)window->getWidth();
    int pixelsInFloat = int(round((float)window->getWidth() / (2.0f * (window->getWidth() / 60.0f))));
    int mapSizePixels = int(round(mapSize * (float)pixelsInFloat));
    if (zoom > 2)
    {
        add(new Sprite(mapPosPixels.x_i + fullMapPixelCoords.x_i - leftUpTileCoords.x_i * tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                       mapPosPixels.y_i - mapSizePixels + fullMapPixelCoords.y_i - leftUpTileCoords.y_i * tileSideInPixels /*- counter*/ - tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                       floatsInPixel * m_ScalePointSize, floatsInPixel * m_ScalePointSize, color,
                       pointerSize,
                       window,
                       window->getHeight() / 60.0f, (window->getWidth() / 60.0f)));
    }
    else
    {
        add(new Sprite(mapPosPixels.x_i + fullMapPixelCoords.x_i - leftUpTileCoords.x_i * tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                       mapPosPixels.y_i - mapSizePixels + fullMapPixelCoords.y_i - leftUpTileCoords.y_i * tileSideInPixels /*- counter*/ - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                       floatsInPixel * m_ScalePointSize, floatsInPixel * m_ScalePointSize, color,
                       pointerSize,
                       window,
                       window->getHeight() / 60.0f, (window->getWidth() / 60.0f)));
    }
}

vec2_i app_graphics::Track::GetAbsolute(vec2 coords, Window* window,
                                        MapLayer* mapLayer, float tileSide, float mapSide, vec2_i tileLeftUp) {

    int tilePixelsSide = int(round(((float)m_Window->getWidth() * tileSide) / (m_Window->getWidth() / 60.0f)));
    tilePixelsSide = int(round((float)tilePixelsSide / (float)2));
    //vec2_i fullMapPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(coords.x, coords.y, i + m_MinZoom, tileSideInPixels);

    float floatsInPixel = 2.0f * (window->getWidth() / 60.0f) / (float)window->getWidth();
    int pixelsInFloat = int(round((float)window->getWidth() / (2.0f * (window->getWidth() / 60.0f))));
    int mapSizePixels = int(round(mapSide * (float)pixelsInFloat));
    //int tilePixelsSide = int(round(((float)window->getWidth() * (mapSide / 4.0f)) / (2.0f * cameraHorizontal)));

    auto inputPixelCoords = app_utils::CoordsConverter::FloatToPixels(coords, window, window->getHeight() / 60.0f, (window->getWidth() / 60.0f));

    //vec2_i pointCoordsInTile = app_utils::CoordsConverter::LatLongToTilePixelXY(Latitude, Longitude, i + m_MinZoom, tilePixelsSide);

    vec2_i ans = vec2_i(-1, -1);
    for (auto& i : mapLayer->m_Renderables->getArr())
    {
        bool con1 = coords.x > i->m_Position.x && coords.x < i->m_Position.x + i->m_Size.x;
        bool con2 = coords.y >i->m_Position.y && coords.y < i->m_Position.y + i->m_Size.y;
        if (con1 && con2) {
            auto pixelTileCoords = app_utils::CoordsConverter::FloatToPixels(i->m_Position, window, window->getHeight() / 60.0f,
                                                                             (window->getWidth() / 60.0f));
            int xDifInsideTile = inputPixelCoords.x_i - pixelTileCoords.x_i;
            //int yDifInsideTile = pixelTileCoords.y_i - tilePixelsSide;
            int yDifInsideTile =  inputPixelCoords.y_i - (pixelTileCoords.y_i - tilePixelsSide);

            int xTileDiff = i->x - tileLeftUp.x_i;
            int yTileDiff = i->y - tileLeftUp.y_i;

            int xAns = int(round(i->x * tileSide * pixelsInFloat)) + xDifInsideTile;//int(round((i->x+1) * tileSide * pixelsInFloat));// + xDifInsideTile;
            int yAns = int(round(i->y * tileSide * pixelsInFloat)) + yDifInsideTile;

            ans.x_i = xAns;
            ans.y_i = yAns;
            break;
        }
    }

    return ans;
}

std::vector<double> app_graphics::Track::GetInfoFromPixelCoords(Renderable2D* trackPoint, Window* window,
                                                    MapLayer* mapLayer, float tileSide, float mapSide, vec2 mapPos, vec2_i tileLeftUp, int zoom, int xTileCountDisplay){
    times.clear();
    impulses.clear();
    alts.clear();
    data.clear();

    //double* InfoNumbers;
    IsPointFound = false;
    std::cout << "HERE IS PRINTING\n";

    vec2_i screenPixelCoords = app_utils::CoordsConverter::FloatToPixels(vec2(trackPoint->m_Position.x, trackPoint->m_Position.y),
                                                                         window, window->getHeight() / 60.0f,
                                                                         window->getWidth() / 60.0f);
    vec2_i mapPosPixels = app_utils::CoordsConverter::FloatToPixels(mapPos, window, window->getHeight() / 60.0f,
                                                                    window->getWidth() / 60.0f);
    //vec2_i absCoordsFromMap = GetPointPixelCoordsOnFullMap(screenPixelCoords, mapPosPixels, mapSide, tileLeftUp,
      //                                                     xTileCountDisplay, window);

    //std::cout << "absCoordsFromMap = " << absCoordsFromMap << std::endl;


    std::ifstream binary(FileNameBinary, std::ios::in | std::ios::binary);
    binary.seekg(trackPoint->PointerFile);

    for (int i = 0; i < 10 + CountDataString - 7; i++)
    {
        data.push_back(1);
    }
    //double* data = new double[10];
    for (int i  = 0; i < 10; i++)
    {
        if (i != 2) {
            binary.read((char *) &data[i], sizeof(double));
        }
        else {
            binary.seekg(trackPoint->PointerFile + 2 * sizeof(double) + 2 * sizeof(int));
        }
    }

    data.erase(data.begin() + 2);

    for (int i = 9; i < CountDataString + 3; i++)
    {
        binary.read((char *) &data[i], sizeof(double));
    }

    for (int i = 0; i < data.size(); i++)
    {
        std::cout << "data[" << i << "] = " << data[i] << std::endl;
    }

    pointLatLong = new vec2(data[0], data[1]);

    binary.close();
    return data;
}