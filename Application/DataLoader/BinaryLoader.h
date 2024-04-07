#pragma once
#include <fstream>
#include <iostream>
#include "TileData.h"
#include <vector>
#include <cmath>
#include "TileLoader.h"
#include <sys/stat.h>
#include <string>
#include <regex>
//#include "../Map/map.h"
#include "../Utils/CoordsConverter.h"

using namespace sparky;
using namespace graphics;
using namespace maths;
using namespace app_graphics;

namespace app_loader
{
    static struct CountTilesToDownload
    {
        int Right;
        int Left;
        int Up;
        int Down;
    } CountTilesToDownload;

    static int MIN_ZOOM = 2;

    static struct CountTilesToDisplay
    {
        int Right;
        int Left;
        int Up;
        int Down;
    } CountTilesToDisplay;

    static struct ConstantStruct
    {
        double centerTileCoordsOnScreenX;
        double centerTileCoordsOnScreenY;
        int centerTileCoordX;
        int centerTileCoordY;

        uint32_t countOfTilesDisplay;
        uint32_t xTileStartDisplay;
        uint32_t yTileStartDisplay;
        uint32_t xTileCountDisplay;
        uint32_t yTileCountDisplay;

        uint32_t countOfTilesDownload;
        uint32_t xTileStartDownload;
        uint32_t yTileStartDownload;
        uint32_t xTileCountDownload;
        uint32_t yTileCountDownload;

        uint32_t  MinTileXDownload;
        uint32_t  MinTileYDownload;

        uint32_t  MaxTileXDownload;
        uint32_t  MaxTileYDownload;

        uint32_t countTilesBefore;
        double areaTileKm;
    } ConstantStructName;

    class BinaryLoader {
    private:
        int m_TotalTiles = 0;
        static bool IsFileExist(const char *filename);
        static bool IsDirExists(const char *dirname);
        char* ImageToArrayBytes(std::string image_path, int &size);
        vec2 m_MapPosition;
        double m_MapSideX;
        double m_MapSideY;
        double m_TileSide;
        int tilesCountLastZoom;

        std::string m_MapType;

        //Tile GetDataFirstLoadedTile();
    public:
        std::ofstream* file_zooms_info;
        std::ifstream* file_zooms_info_read;

        bool IsLoaded = false;

        BinaryLoader(std::string mapType, int minZoom, int maxZoom, vec2_d centerPoint, double widthMeters, double heightMeters,
                     vec2 mapPosition, double mapSideX, double mapSideY, double tileSide, const char* folderName);
        //BinaryLoader(int minZoom, int maxZoom, vec2_d centerPoint, int lastFullZoom, int countTilesAfterFull);

        void SaveZoomToBinary();
        void SaveColors(const char* texturePath);
        void DownloadDataTiles();
        Texture* GetTileTexture(int z, int x, int y);
        std::vector<int> GetTileColors(int z, int x, int y);
        vec2_d GetCenterTilePositionOnScreen(int z);

        int MinZoom;
        int MaxZoom;
        //int LastFullZoom;
        double WidthMeters;
        double HeightMeters;
        vec2 CenterTileCoords;
        vec2_d CenterPoint;
        vec2_i FirstZoomTileCoords;

        std::string FileName;
        std::vector<ConstantStruct> constants;

        //int GetCountOfTilesPrevZooms(int z);
        int GetCount(int z, int x, int y);
        int GetCountConstants(int z);
        void CheckSavedData();

        vec2_d CalculateCenterTileCoordsOnScreen(int z, vec2 mapPosition, double mapSideX, double mapSideY, double tileSide);
        vec2_d CalculateCenterTileCoordsOnScreenForSecondZoom(vec2 mapPosition, double mapSide, double tileSide);
        vec2_i CalculateCenterMapTileCoords(int z);
        vec2_i GetFinalCenterMapTileCoords(int z);

        vec2_d GetTilePositionRelativeCenter(int z, int x, int y);
        vec2_d GetTilePositionRelativeCenterFromBinary(int z, int x, int y);

        vec2_d m_Diff;

        int get_count_prev_full(int z);

        void CountTilesAroundCenterAreaToDownload(int zoom);
        void CountTilesAroundCenterToDisplay(vec2_d centerTilePos) const;
        double GetAreaInOneTile(int tileZ, int tileX, int tileY);
        int GetCountOfTilesToDownload(int zoom);
        vec2_i GetLeftUpTileCoordsToLoad(int zoom);

        void DeleteUnnecessaryTiles();
        std::vector<std::string> GetAllPathsInDirectory(std::string dirPath);
        void DeleteBinaryFiles();

        bool IsFileFull(const char *filename);
        std::string m_FolderName;

        std::ofstream f;
        unsigned long int sizeOfWrittenData;
        int sizeOneFile = 1073741824;
        int currentFileSize = 0;
        int countConstantWritten = 0;

        std::vector<std::ifstream> BinaryFilesRead;
        std::vector<std::ofstream> BinaryFilesWrite;

        int countBinaryFiles = 1;
        std::vector<int> countTilesInBinaryFile;
        int counterTiles = 0;
        int maxCountTilesInBinary = 0;

        std::string FileNameShort;

        std::vector<std::string> GetBinaryFilesPaths();
        std::vector<std::string> BinaryFilesPaths;
        unsigned long minBinaryFileSize = ULLONG_MAX;
        std::string minBinaryFileName = "";
        bool needReload = false;
        std::string currentFilePath = "";

        int COUNT_BINARY_FILES = 0;
        bool IsFileExistsAndFullLoaded(const std::string& fileName);
        bool IsAllDataFilesAlright();
    };
}
