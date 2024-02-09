#include "TrackLoader.h"
#include <iostream>
#include <filesystem>
#include <dirent.h>
#include <cmath>
#include <utility>

namespace fs = std::filesystem;

std::string getFileExtension1(std::string fileName)
{
    size_t dot_index = -1;
    dot_index = fileName.find_last_of('.');
    if (dot_index != -1)
        fileName.erase(0, dot_index + 1);
    return fileName;
}

void TrackLoader::DeleteBinaryFiles()
{
    std::string dir_path = "../Data/Track";
    for (const auto& entry : std::__fs::filesystem::directory_iterator(dir_path)) {
        std::string file_path = entry.path();
        if (getFileExtension1(file_path) == "bin")
            std::remove(file_path.c_str());
    }
}

bool TrackLoader::IsFileExist(const char *filename)
{
    FILE *file;
    return file = fopen(filename, "r");
}

bool TrackLoader::IsFileFull(const char* path) { //!!!!!!!!

    std::ifstream in(path, std::ifstream::in | std::ifstream::binary);
    in.seekg(-sizeof(char), std::ios::end);
    //in.seekg(-sizeof(int) - sizeof(int), std::ios::end);
    //in.seekg(-sizeof(int) - sizeof(int) - sizeof(int), std::ios::end);

    char character;
    in.read((char*)&character, sizeof(character));
    in.close();

    return character == 'L';
}

bool TrackLoader::IsFileExistsAndFullLoaded(const std::string& fileName)
{
    return IsFileExist(fileName.c_str()) && IsFileFull(fileName.c_str());
}

std::vector<std::string> TrackLoader::GetAllPathInDirectory(std::string dirPath) {

    std::vector<std::string> paths;
    /*for (const auto &file: std::filesystem::directory_iterator(dirPath)) {
        if (std::filesystem::is_directory(file)) {
            GetAllPathInDirectory(dirPath);
        } else {
            paths.push_back(file.path().string());
        }
    }*/
    for (const auto& file : std::filesystem::directory_iterator(dirPath)) {
        if (file.path().extension() == ".mat")
        {
            paths.push_back(file.path().string());
        }
    }
    return paths;
}

std::vector<std::string> TrackLoader::GetBinaryFilesPaths()
{
    std::vector<std::string> binaryFilesPaths;
    std::string dir_path = "../Data/Track";
    for (const auto& entry : std::__fs::filesystem::directory_iterator(dir_path)) {
        std::string file_path = entry.path();
        if (getFileExtension1(file_path) == "bin") {
            binaryFilesPaths.push_back(file_path);
        }
    }

    return binaryFilesPaths;
}

bool TrackLoader::IsAllDataFilesAlright() {
    auto binaryFilesPaths = GetBinaryFilesPaths();
    //BinaryFilesWrite.push_back(std::ofstream(BinaryFilesPaths[0], std::ios::out | std::ios::binary));
    //std::cout << "BEGIN ANALYSIS" << std::endl;
    //std::cout << "BinaryFilesPaths.size = " << BinaryFilesPaths.size() << std::endl;
    //std::cout << "FileName = " << FileName << std::endl;
    for (int i = 0; i < binaryFilesPaths.size(); i++) {

        BinaryFilesRead.push_back(std::ifstream(binaryFilesPaths[i], std::ios::out | std::ios::binary));
    }

    //375 fps
    /*if (IsFileExistsAndFullLoaded(FileName))
        std::cout << "first file: " << FileName << " - exists and full loaded!\n";
    else
        std::cout << "first file: " << FileName << " - NOT exists and full loaded!\n";*/


    if (IsFileExistsAndFullLoaded(FirstBinaryFileName))
    {
        std::ifstream firstBinaryFile;
        firstBinaryFile.open(FirstBinaryFileName);
        int readCountFiles;
        firstBinaryFile.seekg(-sizeof(char) - sizeof(int), std::ios::end);
        firstBinaryFile.read((char*)&readCountFiles, sizeof(readCountFiles));
        firstBinaryFile.close();

        bool fullLoaded;
        fullLoaded = IsFileExistsAndFullLoaded(FirstBinaryFileName);

        if (readCountFiles == binaryFilesPaths.size())
        {
            for (int i = 0; i < binaryFilesPaths.size(); i++)
            {
                std::ifstream in(binaryFilesPaths[i], std::ios::binary);
                in.seekg(-sizeof(char), std::ios::end);
                char character;
                in.read((char*)&character, sizeof(character));
                if (character != 'L') {
                    return false;
                }
                in.close();
            }
        }
        else
            return false;
    }
    else {
        return false;
    }

    //file_zooms_info = new std::ofstream(FileNameShort + "(" + std::to_string(BinaryFiles.size()) + ").bin", std::ios::out | std::ios::binary);
    //BinaryFiles.push_back(file_zooms_info);
    return true;
}

double TrackLoader::GetDataOneDouble(std::string fileName, std::string dataFieldName) {
    mat_t    *matfp;
    matvar_t *matvar;
    double finalData = 37;
    matfp = Mat_Open(fileName.c_str(), MAT_ACC_RDONLY);

    matvar = Mat_VarRead(matfp, dataFieldName.c_str());
    if ( nullptr != matvar ) {
        auto* matData = (double*)(matvar[0].data);
        finalData = (double)matData[0];
        //std::cout << "read = " << ans << std::endl;

        /*for (int i = 0; i < 63 * 341; i++)
        {
            if (i % 63 == 0)
                std::cout << std::endl;
            std::cout << matData[i] << " ";
        }*/
        Mat_VarFree(matvar);
    }
    Mat_Close(matfp);

    return finalData;
}

std::vector<double> TrackLoader::GetDataVectorDoubles(std::string fileName, std::string dataFieldName, int size) {
    mat_t* matfp;
    matvar_t* matvar;
    std::vector<double> finalData;
    matfp = Mat_Open(fileName.c_str(), MAT_ACC_RDONLY);

    matvar = Mat_VarRead(matfp, dataFieldName.c_str());
    if ( nullptr != matvar ) {
        auto matData = (double*)(matvar[0].data);

        for (int i = 0; i < size; i++)
        {
            finalData.push_back(matData[i]);
        }
        Mat_VarFree(matvar);
    }

    Mat_Close(matfp);

    return finalData;
}

TrackLoader::TrackLoader(std::string pathToMatFilesFolder)
{
    m_PathToMatFilesFolder = std::move(pathToMatFilesFolder);
    m_MatFilesPaths = GetAllPathInDirectory(m_PathToMatFilesFolder);
    m_CountCoords = m_MatFilesPaths.size();

    m_CountDataPoint = 17 * sizeof(double) +
            6 * 63 * 341 * sizeof(double) + 80 * sizeof(double) + 4 * sizeof(double);

    CurrentFilePath = "../Data/Track/TrackMat_" +
            std::to_string(m_CountCoords) +
            "(0).bin";

    FirstBinaryFileName = CurrentFilePath;


    FileNameShort = "../Data/Track/TrackMat_" +
                    std::to_string(m_CountCoords);

    //if (!(IsFileAlreadyExist(m_BinaryFilePath.c_str()) && IsFileFull()))
    if (!IsAllDataFilesAlright())
    {
        DeleteBinaryFiles();
        BinaryFilesWrite.clear();
        BinaryFilesWrite.push_back(std::ofstream(CurrentFilePath, std::ios::out | std::ios::binary));
        LoadData();
    }
    else {
        //DeleteBinaryFiles();
        IsLoaded = true;
        SetCenterLatLongAndSideMeters();
        std::cout << "CenterLatLong = " << m_CenterLatLong << std::endl;
        std::cout << "SideMeters = " << m_SideMeters << std::endl;
        //DeleteUnnecessaryTiles();
    }
}

void TrackLoader::LoadData()
{
    //BinaryFilesWrite.push_back(std::ofstream(CurrentFilePath, std::ios::out | std::ios::binary));
    //std::cout << "ss = " << BinaryFilesWrite.size() << std::endl;
    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].open(CurrentFilePath);
    //f.open(CurrentFilePath);
    //std::ofstream binaryFile;
    //binaryFile.open(CurrentFilePath);

    std::vector<std::string> matFilesPaths = GetAllPathInDirectory(m_PathToMatFilesFolder);
    std::cout << "matFilesPaths.size = " << matFilesPaths.size() << std::endl;

    /*for (auto& path : m_MatFilesPaths)
    {
        std::cout << "path = " << path << std::endl;
    }*/
    m_CountCoords  = matFilesPaths.size();

    //binaryFile << m_CountCoords;
    //binaryFile << m_CountZooms;
    //binaryFile << m_CountDataPoint;

    int counter = 0;
    long currentDataSize = 0;
    for (auto& path : matFilesPaths) {
        //std::string path = "/Users/maxim_lavrinenko/Documents/projects/FlightDataHandlerDIST/Data/Data_19.05.07/Alapaevsk_18_09_21/data/bsi00002.mat";
        std::ifstream matFile;
        matFile.open(path);

        FlightData fd;

        currentDataSize += m_CountDataPoint;
        if (currentDataSize > sizeOneFile)
        {
            char loadedFull = 'L';
            if (BinaryFilesWrite.size() - 1 == 0) {
                int countFiles = 1;
                BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char *) &countFiles, sizeof(countFiles));
            }
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&loadedFull, sizeof(char));
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].close();

            CurrentFilePath = FileNameShort + "(" + std::to_string(BinaryFilesWrite.size()) + ").bin";
            BinaryFilesWrite.push_back(std::ofstream(CurrentFilePath, std::ios::out | std::ios::binary));
            currentDataSize = 0;
            //binaryFile << loadedFull;
        }

        fd.AR = GetDataOneDouble(path, "AR");
        fd.ARU = GetDataOneDouble(path, "ARU");
        fd.B_Head = GetDataOneDouble(path, "B_Head");
        fd.B_Pitch = GetDataOneDouble(path, "B_Pitch");
        fd.B_Roll = GetDataOneDouble(path, "B_Roll");
        fd.B_Vx = GetDataOneDouble(path, "B_Vx");
        fd.B_Vy = GetDataOneDouble(path, "B_Vy");
        fd.B_Vz = GetDataOneDouble(path, "B_Vz");
        fd.G_Alt = GetDataOneDouble(path, "G_Alt");
        fd.G_Lat = GetDataOneDouble(path, "G_Lat");
        fd.G_Long = GetDataOneDouble(path, "G_Long");
        fd.G_Vx = GetDataOneDouble(path, "G_Vx");
        fd.G_Vy = GetDataOneDouble(path, "G_Vy");
        fd.G_Vz = GetDataOneDouble(path, "G_Vz");
        fd.H0 = GetDataOneDouble(path, "H0");
        fd.Hrv = GetDataOneDouble(path, "Hrv");
        fd.R0 = GetDataOneDouble(path, "R0");

        fd.Sig_C1 = GetDataVectorDoubles(path, "Sig_C1", 63 * 341);
        fd.Sig_C2 = GetDataVectorDoubles(path, "Sig_C2", 63 * 341);
        fd.Sig_C3 = GetDataVectorDoubles(path, "Sig_C3", 63 * 341);

        fd.Sig_S1 = GetDataVectorDoubles(path, "Sig_S1", 63 * 341);
        fd.Sig_S2 = GetDataVectorDoubles(path, "Sig_S2", 63 * 341);
        fd.Sig_S3 = GetDataVectorDoubles(path, "Sig_S3", 63 * 341);

        fd.Swr = GetDataVectorDoubles(path, "Swr", 80);

        fd.Trv = GetDataOneDouble(path, "Swr");
        fd.dH = GetDataOneDouble(path, "dH");
        fd.tau = GetDataOneDouble(path, "tau");
        fd.tau_H = GetDataOneDouble(path, "tau_H");
        matFile.close();

        //binaryFile << counter << ")" << std::endl;
        //binaryFile << path << std::endl;

        //vec2_i fullMapPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(fd.G_Lat, fd.G_Long, counter + m_MinZoom, 512);

        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << fullMapPixelCoords.x_i;
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << fullMapPixelCoords.y_i;

        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.AR, sizeof(fd.AR));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.ARU, sizeof(fd.ARU));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.B_Head, sizeof(fd.B_Head));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.B_Pitch, sizeof(fd.B_Pitch));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.B_Roll, sizeof(fd.B_Roll));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.B_Vx, sizeof(fd.B_Vx));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.B_Vy, sizeof(fd.B_Vy));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.B_Vz, sizeof(fd.B_Vz));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.G_Alt, sizeof(fd.G_Alt));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.G_Lat, sizeof(fd.G_Lat));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.G_Long, sizeof(fd.G_Long));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.G_Vx, sizeof(fd.G_Vx));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.G_Vy, sizeof(fd.G_Vy));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.G_Vz, sizeof(fd.G_Vz));

        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.H0, sizeof(fd.H0));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.Hrv, sizeof(fd.Hrv));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.R0, sizeof(fd.R0));

        for (double j: fd.Sig_C1)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        for (double j: fd.Sig_C2)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        for (double j: fd.Sig_C3)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        for (double j: fd.Sig_S1)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        for (double j: fd.Sig_S2)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        for (double j: fd.Sig_S3)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        for (double j: fd.Swr)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        /*BinaryFilesWrite[BinaryFilesWrite.size() - 1] << fd.Trv << std::endl;
        BinaryFilesWrite[BinaryFilesWrite.size() - 1] << fd.dH << std::endl;
        BinaryFilesWrite[BinaryFilesWrite.size() - 1] << fd.tau << std::endl;
        BinaryFilesWrite[BinaryFilesWrite.size() - 1] << fd.tau_H << std::endl;*/
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.Trv, sizeof(fd.Trv));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.dH, sizeof(fd.dH));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.tau, sizeof(fd.tau));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.tau_H, sizeof(fd.tau_H));

        if (fd.G_Lat < m_MinLatitude)
            m_MinLatitude = fd.G_Lat;
        if (fd.G_Lat > m_MaxLatitude)
            m_MaxLatitude = fd.G_Lat;

        if (fd.G_Long < m_MinLongitude)
            m_MinLongitude = fd.G_Long;
        if (fd.G_Long > m_MaxLongitude)
            m_MaxLongitude = fd.G_Long;

        counter++;
        std::cout << "LOADING TRACK... (" << (double)counter / (double)m_CountCoords * 100 << "%)" << std::endl;
    }

    /*BinaryFilesWrite[BinaryFilesWrite.size() - 1] << m_MinLatitude << std::endl;
    BinaryFilesWrite[BinaryFilesWrite.size() - 1] << m_MaxLatitude << std::endl;
    BinaryFilesWrite[BinaryFilesWrite.size() - 1] << m_MinLongitude << std::endl;
    BinaryFilesWrite[BinaryFilesWrite.size() - 1] << m_MaxLongitude << std::endl;*/
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_MinLatitude, sizeof(m_MinLatitude));
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_MaxLatitude, sizeof(m_MaxLatitude));
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_MinLongitude, sizeof(m_MinLongitude));
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_MaxLongitude, sizeof(m_MaxLongitude));

    m_CenterLatLong = vec2_d(m_MinLatitude + (m_MaxLatitude - m_MinLatitude) / 2.0,
                             m_MinLongitude + (m_MaxLongitude - m_MinLongitude) / 2.0);

    /*BinaryFilesWrite[BinaryFilesWrite.size() - 1] << m_CenterLatLong.x_d << std::endl;
    BinaryFilesWrite[BinaryFilesWrite.size() - 1] << m_CenterLatLong.y_d << std::endl;*/
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_CenterLatLong.x_d, sizeof(m_CenterLatLong.x_d));
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_CenterLatLong.y_d, sizeof(m_CenterLatLong.y_d));

    BordersLatLong.LeftUpCoord = vec2_d(m_MaxLatitude, m_MinLongitude);
    BordersLatLong.RightUpCoord = vec2_d(m_MaxLatitude, m_MaxLongitude);
    BordersLatLong.RightDownCoord = vec2_d(m_MinLatitude, m_MaxLongitude);
    BordersLatLong.LeftDownCoord = vec2_d(m_MinLatitude, m_MinLongitude);

    double temp1 = app_utils::CoordsConverter::GetDistBetweenLatLongPointsInMeters(BordersLatLong.LeftDownCoord, BordersLatLong.LeftUpCoord);
    double temp2 = app_utils::CoordsConverter::GetDistBetweenLatLongPointsInMeters(BordersLatLong.LeftUpCoord, BordersLatLong.RightUpCoord);
    double temp3 = app_utils::CoordsConverter::GetDistBetweenLatLongPointsInMeters(BordersLatLong.RightUpCoord, BordersLatLong.RightDownCoord);
    double temp4 = app_utils::CoordsConverter::GetDistBetweenLatLongPointsInMeters(BordersLatLong.RightDownCoord, BordersLatLong.LeftDownCoord);

    double maxWidth, maxHeight;
    if (temp1 >= temp3)
        maxHeight = temp1;
    else
        maxHeight = temp3;

    if (temp2 >= temp4)
        maxWidth = temp2;
    else
        maxWidth = temp4;

    int areaAdditionMeters = 10000;

    if (maxWidth >= maxHeight)
        m_SideMeters = maxWidth + areaAdditionMeters;
    else
        m_SideMeters = maxHeight + areaAdditionMeters;

    //std::string fileName = std::to_string(CenterLatLong.x_d) + "_" + std::to_string(CenterLatLong.y_d) + "_" + std::to_string(SideMeters) + ".bin";
    //std::rename(FileNameBinary.c_str(), fileName.c_str());

    //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << m_SideMeters << std::endl;
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_SideMeters, sizeof(m_SideMeters));

    char loadedFull = 'L';
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&loadedFull, sizeof(loadedFull));
    //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << loadedFull << std::endl;

    BinaryFilesWrite[BinaryFilesWrite.size() - 1].close();

    //std::cout << "FILES LOADED: " << counter << std::endl;

    COUNT_BINARY_FILES = BinaryFilesWrite.size();
    std::ofstream in(FirstBinaryFileName, std::ifstream::in | std::ifstream::binary);
    in.seekp(-sizeof(char) - sizeof(int), std::ios::end);
    in.write((char*)&COUNT_BINARY_FILES, sizeof(COUNT_BINARY_FILES));
    in.close();

    std::cout << "[LOADED in BINARY]: m_CenterLatLong = " << m_CenterLatLong << std::endl;
    std::cout << "[LOADED in BINARY]: m_SideMeters = " << m_SideMeters << std::endl;
}

void TrackLoader::SetCenterLatLongAndSideMeters() {
    std::string lastFileName = FileNameShort + "(" + std::to_string(BinaryFilesRead.size() - 1) + ").bin";
    std::cout << "file: " << lastFileName << std::endl;
    std::ifstream in;
    in.open(lastFileName);
    //centerLat
    //centerLong
    //sideMeters
    //loaded
    double sideMeters;
    in.seekg(-sizeof(char) - 3 * sizeof(double), std::ios::end);
    in.read((char*)&m_CenterLatLong.x_d, sizeof(double));
    in.read((char*)&m_CenterLatLong.y_d, sizeof(double));
    in.read((char*)&m_SideMeters, sizeof(double));
    in.close();

    //std::ifstream
}
