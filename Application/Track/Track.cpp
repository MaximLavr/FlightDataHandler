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

    Z_AR = m_TrackLoader->GetZ_AR();
    Z_ARU = m_TrackLoader->GetZ_ARU();

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

std::string app_graphics::Track::GetFileNumberFromPath(std::string path)
{
    std::string subString;
    bool begin;
    for (int i = 0; i < path.length(); i++)
    {
        if (path[i] == '(')
            begin = true;
        if (path[i] == ')')
            begin = false;
        if (begin && path[i] != '(')
            subString.push_back(path[i]);
    }

    return subString;
}
#if 1
void app_graphics::Track::AddPoints(vec2 mapPos, float mapSize, int zoom,unsigned int color, vec2_i leftUpTileCoords)
{
    auto mapPosPixels = app_utils::CoordsConverter::FloatToPixels(mapPos, m_Window, m_Window->getHeight() / 60.0f, m_Window->getWidth() / 60.0f);

    int tileSideInPixels = int(round(((float)m_Window->getWidth() * (mapSize / 4.0f)) / (m_Window->getWidth() / 60.0f)));
    tileSideInPixels = int(round((float)tileSideInPixels / (float)2));
    float floatsInPixel = 2.0f * (m_Window->getWidth() / 60.0f) / (float)m_Window->getWidth();
    int pixelsInFloat = int(round((float)m_Window->getWidth() / (2.0f * (m_Window->getWidth() / 60.0f))));
    int mapSizePixels = int(round(mapSize * (float)pixelsInFloat));

    std::string LastFileIndex = std::to_string(m_TrackLoader->BinaryFilesPaths.size() - 1);
    //vec2 latLong = vec2(0, 0);
    int counterPoints = 1;
    long long pointerSize = 0;
    for (auto& path: m_TrackLoader->BinaryFilesPaths)
    {
        //std::cout << "path = " << path << std::endl;
        std::ifstream f;
        f.open(path);
        f.seekg(-sizeof(char) - sizeof(int), std::ios::end);
        int kPoints;
        f.read((char *)&kPoints, sizeof(kPoints));
        pointerSize = 0;
        for (int k = 0; k < kPoints; k++)
        {
            int pointNumber;
            int sizeSig_C1, sizeSig_C2, sizeSig_C3, sizeSig_S1, sizeSig_S2, sizeSig_S3, sizeSwr;
            double ar, aru, head, pitch, roll, vx, vy, vz, alt, lat, lon;
            //f.seekg(9 * sizeof(double) + sizeof(unsigned long) + prevSizeInBytes);
            f.seekg(pointerSize);
            f.read((char*)&pointNumber, sizeof(int));
            f.read((char*)&sizeSig_C1, sizeof(int));
            f.read((char*)&sizeSig_C2, sizeof(int));
            f.read((char*)&sizeSig_C3, sizeof(int));
            f.read((char*)&sizeSig_S1, sizeof(int));
            f.read((char*)&sizeSig_S2, sizeof(int));
            f.read((char*)&sizeSig_S3, sizeof(int));
            f.read((char*)&sizeSwr, sizeof(int));
            f.read((char*)&ar, sizeof(double));
            f.read((char*)&aru, sizeof(double));
            f.read((char*)&head, sizeof(double));
            f.read((char*)&pitch, sizeof(double));
            f.read((char*)&roll, sizeof(double));
            f.read((char*)&vx, sizeof(double));
            f.read((char*)&vy, sizeof(double));
            f.read((char*)&vz, sizeof(double));
            f.read((char*)&alt, sizeof(double));
            f.read((char*)&lat, sizeof(double));
            f.read((char*)&lon, sizeof(double));
            vec2_i fullMapPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(lat, lon, zoom, tileSideInPixels);
            if (zoom > 2)
            {
                add(new Sprite(mapPosPixels.x_i + fullMapPixelCoords.x_i - leftUpTileCoords.x_i * tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                               mapPosPixels.y_i - mapSizePixels + fullMapPixelCoords.y_i - leftUpTileCoords.y_i * tileSideInPixels /*- counter*/ - tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                               floatsInPixel * m_ScalePointSize, floatsInPixel * m_ScalePointSize, color, path,
                               pointerSize,
                               m_Window,
                               m_Window->getHeight() / 60.0f, (m_Window->getWidth() / 60.0f)));
            }
            else
            {
                add(new Sprite(mapPosPixels.x_i + fullMapPixelCoords.x_i - leftUpTileCoords.x_i * tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                               mapPosPixels.y_i - mapSizePixels + fullMapPixelCoords.y_i - leftUpTileCoords.y_i * tileSideInPixels /*- counter*/ - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                               floatsInPixel * m_ScalePointSize, floatsInPixel * m_ScalePointSize, color, path,
                               pointerSize,
                               m_Window,
                               m_Window->getHeight() / 60.0f, (m_Window->getWidth() / 60.0f)));
            }
            pointerSize += 21 * sizeof(double) + 8 * sizeof(int) + sizeSig_C1 * sizeof(double) + sizeSig_C2 * sizeof(double) +
                    sizeSig_C3 * sizeof(double) + sizeSig_S1 * sizeof(double) + sizeSig_S2 * sizeof(double) + sizeSig_S3 * sizeof(double) +
                    sizeSwr * sizeof(double);
            //pointerSize += m_TrackLoader->m_CountDataPoint;
            //latLong.x = lat;
            //latLong.y = lon;
            //std::cout << "[" << counterPoints << "] lat(*) = " << lat << std::endl;
            //std::cout << "[" << counterPoints << "] lon(*) = " << lon << std::endl;
            counterPoints++;
        }
        f.close();
    }
}
#endif

void app_graphics::Track::AddPoint(vec2 latLong, vec2 mapPos, float mapSize, int zoom, unsigned int color, size_t pointerSize, vec2_i leftUpTileCoords) {
    auto mapPosPixels = app_utils::CoordsConverter::FloatToPixels(mapPos, m_Window, m_Window->getHeight() / 60.0f, m_Window->getWidth() / 60.0f);

    int tileSideInPixels = int(round(((float)m_Window->getWidth() * (mapSize / 4.0f)) / (m_Window->getWidth() / 60.0f)));
    tileSideInPixels = int(round((float)tileSideInPixels / (float)2));
    vec2_i fullMapPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(latLong.x, latLong.y, zoom, tileSideInPixels);
    float floatsInPixel = 2.0f * (m_Window->getWidth() / 60.0f) / (float)m_Window->getWidth();
    int pixelsInFloat = int(round((float)m_Window->getWidth() / (2.0f * (m_Window->getWidth() / 60.0f))));
    int mapSizePixels = int(round(mapSize * (float)pixelsInFloat));
    if (zoom > 2)
    {
        add(new Sprite(mapPosPixels.x_i + fullMapPixelCoords.x_i - leftUpTileCoords.x_i * tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                       mapPosPixels.y_i - mapSizePixels + fullMapPixelCoords.y_i - leftUpTileCoords.y_i * tileSideInPixels /*- counter*/ - tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                       floatsInPixel * m_ScalePointSize, floatsInPixel * m_ScalePointSize, color,
                       pointerSize,
                       m_Window,
                       m_Window->getHeight() / 60.0f, (m_Window->getWidth() / 60.0f)));
    }
    else
    {
        add(new Sprite(mapPosPixels.x_i + fullMapPixelCoords.x_i - leftUpTileCoords.x_i * tileSideInPixels - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                       mapPosPixels.y_i - mapSizePixels + fullMapPixelCoords.y_i - leftUpTileCoords.y_i * tileSideInPixels /*- counter*/ - pixelsInFloat * (floatsInPixel * m_ScalePointSize) / 2.0f,
                       floatsInPixel * m_ScalePointSize, floatsInPixel * m_ScalePointSize, color,
                       pointerSize,
                       m_Window,
                       m_Window->getHeight() / 60.0f, (m_Window->getWidth() / 60.0f)));
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

FlightData* app_graphics::Track::GetInfoFromPixelCoords(Renderable2D* trackPoint, Window* window,
                                                    MapLayer* mapLayer, float tileSide, float mapSide, vec2 mapPos, vec2_i tileLeftUp, int zoom, int xTileCountDisplay){
    times.clear();
    impulses.clear();
    alts.clear();

    FlightData* flightData = new FlightData();
#if 0
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
#endif

    std::ifstream binaryFile;
    //std::cout << "file path: " << trackPoint->FilePath << std::endl;
    //std::string path = m_TrackLoader->FileNameShort + "(" + trackPoint->FileNumber + ").txt";
    binaryFile.open(trackPoint->FilePath);
    binaryFile.seekg(trackPoint->PointerFile + 0 * sizeof(double));

    int pointNumber;
    int sizeSig_C1, sizeSig_C2, sizeSig_C3, sizeSig_S1, sizeSig_S2, sizeSig_S3, sizeSwr;
    double AR, ARU, B_Head, B_Pitch, B_Roll, B_Vx, B_Vy, B_Vz, G_Alt, G_Lat, G_Long, G_Vx, G_Vy, G_Vz, H0, Hrv, R0;

    binaryFile.read((char*)&pointNumber, sizeof(pointNumber));

    binaryFile.read((char*)&sizeSig_C1, sizeof(sizeSig_C1));
    binaryFile.read((char*)&sizeSig_C2, sizeof(sizeSig_C2));
    binaryFile.read((char*)&sizeSig_C3, sizeof(sizeSig_C3));
    binaryFile.read((char*)&sizeSig_S1, sizeof(sizeSig_S1));
    binaryFile.read((char*)&sizeSig_S2, sizeof(sizeSig_S2));
    binaryFile.read((char*)&sizeSig_S3, sizeof(sizeSig_S3));
    binaryFile.read((char*)&sizeSwr, sizeof(sizeSwr));

    binaryFile.read((char*)&AR, sizeof(AR));
    binaryFile.read((char*)&ARU, sizeof(ARU));
    binaryFile.read((char*)&B_Head, sizeof(B_Head));
    binaryFile.read((char*)&B_Pitch, sizeof(B_Pitch));
    binaryFile.read((char*)&B_Roll, sizeof(B_Roll));
    binaryFile.read((char*)&B_Vx, sizeof(B_Vx));
    binaryFile.read((char*)&B_Vy, sizeof(B_Vy));
    binaryFile.read((char*)&B_Vz, sizeof(B_Vz));
    binaryFile.read((char*)&G_Alt, sizeof(G_Alt));
    binaryFile.read((char*)&G_Lat, sizeof(G_Lat));
    binaryFile.read((char*)&G_Long, sizeof(G_Long));
    binaryFile.read((char*)&G_Vx, sizeof(G_Vx));
    binaryFile.read((char*)&G_Vy, sizeof(G_Vy));
    binaryFile.read((char*)&G_Vz, sizeof(G_Vz));
    binaryFile.read((char*)&H0, sizeof(H0));
    binaryFile.read((char*)&Hrv, sizeof(Hrv));
    binaryFile.read((char*)&R0, sizeof(R0));

    flightData->pointNumber = pointNumber;
    flightData->sizeSig_C1 = sizeSig_C1;
    flightData->sizeSig_C2 = sizeSig_C2;
    flightData->sizeSig_C3 = sizeSig_C3;
    flightData->sizeSig_S1 = sizeSig_S1;
    flightData->sizeSig_S2 = sizeSig_S2;
    flightData->sizeSig_S3 = sizeSig_S3;
    flightData->sizeSwr = sizeSwr;

    flightData->AR = AR;
    flightData->ARU = ARU;
    flightData->B_Head = B_Head;
    flightData->B_Pitch = B_Pitch;
    flightData->B_Roll = B_Roll;
    flightData->B_Vx = B_Vx;
    flightData->B_Vy = B_Vy;
    flightData->B_Vz = B_Vz;
    flightData->G_Alt = G_Alt;
    flightData->G_Lat = G_Lat;
    flightData->G_Long = G_Long;
    flightData->G_Vx = G_Vx;
    flightData->G_Vy = G_Vy;
    flightData->G_Vz = G_Vz;
    flightData->H0 = H0;
    flightData->Hrv = Hrv;
    flightData->R0 = R0;

    //Sig_C1
    for (int i = 0; i < sizeSig_C1; i++)
    {
        double sig;
        binaryFile.read((char*)&sig, sizeof(sig));
        flightData->Sig_C1.push_back(sig);
    }

    //Sig_C2
    for (int i = 0; i < sizeSig_C2; i++)
    {
        double sig;
        binaryFile.read((char*)&sig, sizeof(sig));
        flightData->Sig_C2.push_back(sig);
    }

    //Sig_C3
    for (int i = 0; i < sizeSig_C3; i++)
    {
        double sig;
        binaryFile.read((char*)&sig, sizeof(sig));
        flightData->Sig_C3.push_back(sig);
    }

    //Sig_S1
    for (int i = 0; i < sizeSig_S1; i++)
    {
        double sig;
        binaryFile.read((char*)&sig, sizeof(sig));
        flightData->Sig_S1.push_back(sig);
    }

    //Sig_S2
    for (int i = 0; i < sizeSig_S2; i++)
    {
        double sig;
        binaryFile.read((char*)&sig, sizeof(sig));
        flightData->Sig_S2.push_back(sig);
    }

    //Sig_S3
    for (int i = 0; i < sizeSig_S3; i++)
    {
        double sig;
        binaryFile.read((char*)&sig, sizeof(sig));
        flightData->Sig_S3.push_back(sig);
    }

    //Swr
    for (int i = 0; i < sizeSwr; i++)
    {
        double sig;
        binaryFile.read((char*)&sig, sizeof(sig));
        flightData->Swr.push_back(sig);
    }

    double Trv, dH, tau, tau_H;
    binaryFile.read((char*)&Trv, sizeof(Trv));
    binaryFile.read((char*)&dH, sizeof(dH));
    binaryFile.read((char*)&tau, sizeof(tau));
    binaryFile.read((char*)&tau_H, sizeof(tau_H));

    flightData->Trv = Trv;
    flightData->dH = dH;
    flightData->tau = tau;
    flightData->tau_H = tau_H;

    binaryFile.close();

    return flightData;
}

void app_graphics::Track::PrintZ_AR_Z_ARUToFile(const char* filePath)
{
    std::ofstream file;
    file.open(filePath);
    file << "<------------Z_AR, Z_ARU BEGIN------------>" << std::endl;
    file << "Z_AR (size = " << Z_AR.size() << "): ";
    for (auto& i : Z_AR)
    {
        file << i << " ";
    }
    file << std::endl;

    file << "Z_ARU (size = " << Z_ARU.size() << "): ";
    for (auto& i : Z_ARU)
    {
        file << i << " ";
    }
    file << std::endl;
    file << "<------------Z_AR, Z_ARU END------------>" << std::endl;
    file.close();
}

void app_graphics::Track::HighlightPoint(Renderable2D* point, unsigned int color)
{
    point->setColor(color);
}

void app_graphics::Track::ClearEthalons(const char* path)
{
    std::ofstream file;
    file.open(path, std::ofstream::out | std::ofstream::trunc);
    file.close();
}

void app_graphics::Track::AddNewEthalons(double *impulses, const char *path)
{
    std::ofstream file;
    file.open(path, std::ofstream::app);

    for (int i = 0; i < 63 * 341; i++)
    {
        file << impulses[i] << std::endl;
    }
    file.close();
}

void app_graphics::Track::HighlightPointsWithEvolutions(vec2 pitchMinMax, vec2 rollMinMax, unsigned int color)
{
    for (auto& point : m_Renderables)
    {
        std::ifstream binaryFile;
        //std::cout << "file path: " << trackPoint->FilePath << std::endl;
        //std::string path = m_TrackLoader->FileNameShort + "(" + trackPoint->FileNumber + ").txt";
        binaryFile.open(point->FilePath);
        binaryFile.seekg(point->PointerFile + 8 * sizeof(int) + 3 * sizeof(double));
        double pitch, roll;
        binaryFile.read((char*)&pitch, sizeof(pitch));
        binaryFile.read((char*)&roll, sizeof(roll));

        if (pitch >= pitchMinMax.x && pitch <= pitchMinMax.y && roll >= rollMinMax.x && roll <= rollMinMax.y)
            point->setColor(color);
        binaryFile.close();
    }
}
