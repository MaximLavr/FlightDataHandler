#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "FlightData.h"
#include <matio_pubconf.h>
#include <matio.h>
#include "../Utils/CoordsConverter.h"
#include "../Map/MapLayer.h"

class TrackLoader {
private:
    std::string m_PathToMatFilesFolder;
    //std::string m_BinaryFilePath;
    std::vector<std::string> m_MatFilesPaths;
    static std::vector<std::string> GetAllPathInDirectory(std::string dirPath);

    //int sizeOneFile = 1073741824;
    long sizeOneFile = 1073741824;
    std::vector<std::ofstream> BinaryFilesWrite;

    int m_CountCoords;
    double m_MinLatitude = 1000000;
    double m_MinLongitude = 1000000;
    double m_MaxLatitude = 0;
    double m_MaxLongitude = 0;

    vec2_d m_CenterLatLong;
    double m_SideMeters;
    std::string CurrentFilePath;
    std::ofstream CurrentStream;

    int COUNT_BINARY_FILES = 0;

    struct BordersLatLong
    {
        vec2_d LeftUpCoord;
        vec2_d RightUpCoord;
        vec2_d LeftDownCoord;
        vec2_d RightDownCoord;
    } BordersLatLong;

    std::string FirstBinaryFileName;

    bool IsAllDataFilesAlright();
    std::vector<std::string> GetBinaryFilesPaths();
    std::vector<std::ifstream> BinaryFilesRead;

    void DeleteBinaryFiles();

    bool IsFileExist(const char *filename);
    bool IsFileFull(const char *filename);
    bool IsFileExistsAndFullLoaded(const std::string& fileName);

    static double GetDataOneDouble(std::string fileName, std::string dataFieldName);
    static std::vector<double> GetDataVectorDoubles(std::string fileName, std::string dataFieldName);

    bool IsLoaded;
    void LoadData();

    std::vector<double> m_Z_AR;
    std::vector<double> m_Z_ARU;
    std::string m_PathToZ_AR_Z_ARU;

public:
    TrackLoader(std::string pathToMatFilesFolder);

    ~TrackLoader()
    {
        m_MatFilesPaths.clear();
    }

    void SetCenterLatLongAndSideMeters();
    vec2_d GetCenterLatLong() { return m_CenterLatLong; }
    double GetSideMeters() { return m_SideMeters; }
    std::vector<std::string> BinaryFilesPaths;

    std::vector<double> GetLatLongs();
    long m_CountDataPoint;
    std::string FileNameShort;

    std::vector<double> GetZ_AR() { return m_Z_AR; }
    std::vector<double> GetZ_ARU() { return m_Z_ARU; }
};


