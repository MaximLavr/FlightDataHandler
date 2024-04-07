/* latitude, longtitude
 * time, alt, GPS_alt, head, pitch, roll, velocity, all_impulse */

#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "../DataLoader/TrackLoader.h"

namespace app_graphics
{

    class Track : public Layer {
    private:
        //std::string m_FileNameTxt;
        int m_CountStringRead = 0;
        std::vector<unsigned long> m_PrevSizes;

        //Map* m_Map;
    public:
        static const int COUNT_IMPULSES_IN_TRAIN = 341;
        static const int COUNT_IMPULSE_TRAINS = 63;
        std::string GetFileNumberFromPath(std::string path);
        std::vector<double> Z_AR;
        std::vector<double> Z_ARU;
        Track(std::string pathToDirPoints, unsigned int color, Shader* shader,
            mat4 projectionMatrix, Window *window, int scalePointSize = 1);
        TrackLoader* m_TrackLoader;

        std::vector<std::string> GetAllPathsInDirectory(std::string dirPath);

        int m_ScalePointSize = 1;
        void Draw(unsigned int color, vec2_i leftUpTileCoords);
        std::string FileNameBinary;
        std::ofstream* BinaryFileTrackOut;
        std::ifstream* BinaryFileTrackIn;
        double Longitude;
        double Latitude;
        vec2 m_MapPos;
        float m_MapSize;
        double GetFromBinaryMinLatitude();
        double GetFromBinaryMinLongitude();
        double GetFromBinaryMaxLatitude();
        double GetFromBinaryMaxLongitude();

        vec2_d CenterLatLong;
        double SideMeters;

        int CountCoords;
        int CountDataString;
        int CountZooms;
        double MinLatitude = 1000000;
        double MinLongitude = 1000000;
        double MaxLatitude = 0;
        double MaxLongitude = 0;

        unsigned int m_Color;

        struct BordersLatLong
        {
            vec2_d LeftUpCoord;
            vec2_d RightUpCoord;
            vec2_d LeftDownCoord;
            vec2_d RightDownCoord;
        } BordersLatLong;

        std::vector<double> points;

        void PrintInfoByLatLong(std::vector<std::string> latLong);

        void Draw(vec2 latLong, int zoom, unsigned int color, vec2 mapPosition, vec2_i leftUpTileCoords);
        void AddPoint(vec2 latLong, vec2 mapPos, float mapSize, int zoom, unsigned int color, size_t pointerSize, vec2_i leftUpTileCoords);
        void AddPoints(vec2 mapPos, float mapSize, int zoom, unsigned int color, vec2_i leftUpTileCoords);
        FlightData* GetInfoFromPixelCoords(Renderable2D* trackPoint, Window* window,
                                       MapLayer* mapLayer, float tileSide, float mapSide, vec2 mapPos, vec2_i tileLeftUp, int zoom, int xTileCountDisplay);

        void PrintAbsolutePixel(vec2_i absPixelCoords, int zoom, vec2 mapPos, Window *window, vec2_i leftUpTileCoords);

        void DeleteBinaryFile();

        vec2_i GetAbsolute(vec2 coords, Window* window, MapLayer* mapLayer, float tileSide, float mapSide, vec2_i tileLeftUp);

        void SaveDataToBinary(/*float mapSize*/);

        bool IsPointFound = false;

        double* AllData;

        double* GetInfoGraphicsPixelCoords(vec2 coords, Window* window,MapLayer* mapLayer, float tileSide, vec2_i tileLeftUp, int zoom);
        std::vector<double> times;
        std::vector<double> alts;
        std::vector<double> impulses;

        bool IsLoaded;
        Window* m_Window;

        bool IsFileExist(const char *filename);

        bool IsBinaryFileFull(const char *filename);

        std::ifstream::pos_type FileSize;

        void CompareFile();

        /*vec2_i GetPointPixelCoordsOnFullMap(vec2_i screenPixelCoords, vec2_i mapPosPixels, float mapSize,
                                            vec2_i TileLeftUp, int xTileCountDisplay, Window *window);*/

        double MinDelta = 1000;
        double LatitudeFinal;
        double LongitudeFinal;

        vec2* pointLatLong = nullptr;

        void PrintZ_AR_Z_ARUToFile(const char* filePath);

        void HighlightPoint(Renderable2D* point, unsigned int color);
        void HighlightPointsWithEvolutions(vec2 pitchMinMax, vec2 rollMinMax, unsigned int color);

        static struct Eathalon
        {
            int Forest;
            int Water;
        } Eathalon;

        void AddNewEthalons(double* impulses, const char* path);
        void ClearEthalons(const char* path);
        const char* ethalonsForest = "../Data/Track/ethalons/ethalonForest.txt";
        const char* ethalonsWater = "../Data/Track/ethalons/ethalonWater.txt";
    };
}

