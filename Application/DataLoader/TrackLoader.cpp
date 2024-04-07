#include "TrackLoader.h"
#include <iostream>
#include <filesystem>
#include <dirent.h>
#include <cmath>
#include <utility>

namespace fs = std::filesystem;

//DeleteBinaryFiles
//GetBinaryFilesPaths
//CurrentFilePath =

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
        if (getFileExtension1(file_path) == "txt")
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
        if (getFileExtension1(file_path) == "txt") {
            binaryFilesPaths.push_back(file_path);
        }
    }

    return binaryFilesPaths;
}

bool TrackLoader::IsAllDataFilesAlright() {
    //auto binaryFilesPaths = GetBinaryFilesPaths();
    //BinaryFilesWrite.push_back(std::ofstream(BinaryFilesPaths[0], std::ios::out | std::ios::binary));
    //std::cout << "BEGIN ANALYSIS" << std::endl;
    //std::cout << "BinaryFilesPaths.size = " << BinaryFilesPaths.size() << std::endl;
    //std::cout << "FileName = " << FileName << std::endl;
    for (int i = 0; i < BinaryFilesPaths.size(); i++) {

        BinaryFilesRead.push_back(std::ifstream(BinaryFilesPaths[i], std::ios::out | std::ios::binary));
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
        firstBinaryFile.seekg(-sizeof(char) - 2 * sizeof(int), std::ios::end);
        firstBinaryFile.read((char*)&readCountFiles, sizeof(readCountFiles));
        firstBinaryFile.close();

        //bool fullLoaded;
        //fullLoaded = IsFileExistsAndFullLoaded(FirstBinaryFileName);

        if (readCountFiles == BinaryFilesPaths.size())
        {
            for (int i = 0; i < BinaryFilesPaths.size(); i++)
            {
                std::ifstream in(BinaryFilesPaths[i], std::ios::binary);
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

std::vector<double> TrackLoader::GetDataVectorDoubles(std::string fileName, std::string dataFieldName) {
    mat_t* matfp;
    matvar_t* matvar;
    std::vector<double> finalData;

    matfp = Mat_Open(fileName.c_str(), MAT_ACC_RDONLY);
    matvar = Mat_VarRead(matfp, dataFieldName.c_str());

    if ( nullptr != matvar ) {
        auto matData = (double*)(matvar[0].data);

        auto size = Mat_VarGetSize(matvar) / sizeof(double);
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


    m_PathToZ_AR_Z_ARU = m_PathToMatFilesFolder + "/Z_AR;Z_ARU/ARU_AR.mat";

    m_Z_AR = GetDataVectorDoubles(m_PathToZ_AR_Z_ARU, "Z_AR");
    m_Z_ARU = GetDataVectorDoubles(m_PathToZ_AR_Z_ARU, "Z_ARU");

    auto m_CountDataPoint = 17 * sizeof(double) +
            6 * 63 * 341 * sizeof(double) + 80 * sizeof(double) + 4 * sizeof(double);

    CurrentFilePath = "../Data/Track/TrackMat_" +
            std::to_string(m_CountCoords) +
            "(0).txt"; //bin

    FirstBinaryFileName = CurrentFilePath;


    FileNameShort = "../Data/Track/TrackMat_" +
                    std::to_string(m_CountCoords);

    BinaryFilesPaths = GetBinaryFilesPaths();

    //if (!(IsFileAlreadyExist(m_BinaryFilePath.c_str()) && IsFileFull()))
    if (!IsAllDataFilesAlright())
    {
        DeleteBinaryFiles();
        BinaryFilesWrite.clear();
        BinaryFilesPaths.clear();
        BinaryFilesPaths.push_back(CurrentFilePath);
        BinaryFilesWrite.push_back(std::ofstream(CurrentFilePath, std::ios::out | std::ios::binary));
        LoadData();
    }
    else {
        //DeleteBinaryFiles();
        IsLoaded = true;
        SetCenterLatLongAndSideMeters();
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

    int countPoints = 0;
    int totalCountPoints = 0;
    unsigned long currentDataSize = 0;
    for (auto& path : matFilesPaths) {
        //std::string path = "/Users/maxim_lavrinenko/Documents/projects/FlightDataHandlerDIST/Data/Data_19.05.07/Alapaevsk_18_09_21/data/bsi00002.mat";
        std::ifstream matFile;
        matFile.open(path);

        std::string pointNumberStr = path;
        pointNumberStr = pointNumberStr.substr(pointNumberStr.length() - 9, 5);

        FlightData fd;

        if (currentDataSize > sizeOneFile)
        {
            char loadedFull = 'L';
            if (BinaryFilesWrite.size() - 1 == 0) {
                double centerX = 0;
                double centerY = 0;
                double sideMeters = 0;
                BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char *) &centerX, sizeof(centerX));
                BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char *) &centerY, sizeof(centerY));
                BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char *) &sideMeters, sizeof(sideMeters));
                int countFiles = 1;
                BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char *) &countFiles, sizeof(countFiles));
            }
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&countPoints, sizeof(countPoints));
            //std::cout << "cp = " << countPoints << std::endl;
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&loadedFull, sizeof(char));
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].close();

            CurrentFilePath = FileNameShort + "(" + std::to_string(BinaryFilesWrite.size()) + ").txt";
            BinaryFilesWrite.push_back(std::ofstream(CurrentFilePath, std::ios::out | std::ios::binary));
            BinaryFilesPaths.push_back(CurrentFilePath);
            currentDataSize = 0;
            countPoints = 0;
            //binaryFile << loadedFull;
        }

        fd.pointNumber = stoi(pointNumberStr);
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

        fd.Sig_C1 = GetDataVectorDoubles(path, "Sig_C1");
        fd.Sig_C2 = GetDataVectorDoubles(path, "Sig_C2");
        fd.Sig_C3 = GetDataVectorDoubles(path, "Sig_C3");

        fd.Sig_S1 = GetDataVectorDoubles(path, "Sig_S1");
        fd.Sig_S2 = GetDataVectorDoubles(path, "Sig_S2");
        fd.Sig_S3 = GetDataVectorDoubles(path, "Sig_S3");

        fd.Swr = GetDataVectorDoubles(path, "Swr");
        fd.Trv = GetDataOneDouble(path, "Trv");

        fd.dH = GetDataOneDouble(path, "dH");
        fd.tau = GetDataOneDouble(path, "tau");
        fd.tau_H = GetDataOneDouble(path, "tau_H");

        fd.sizeSig_C1 = fd.Sig_C1.size();
        fd.sizeSig_C2 = fd.Sig_C2.size();
        fd.sizeSig_C3 = fd.Sig_C3.size();
        fd.sizeSig_S1 = fd.Sig_S1.size();
        fd.sizeSig_S2 = fd.Sig_S2.size();
        fd.sizeSig_S3 = fd.Sig_S3.size();
        fd.sizeSwr = fd.Swr.size();
        matFile.close();

        auto sizePointBytes = 21 * sizeof(double) +
                fd.Sig_C1.size() * sizeof(double) +
                fd.Sig_C2.size() * sizeof(double) +
                fd.Sig_C3.size() * sizeof(double) +
                fd.Sig_S1.size() * sizeof(double) +
                fd.Sig_S2.size() * sizeof(double) +
                fd.Sig_S3.size() * sizeof(double) +
                fd.Swr.size() * sizeof(double) +
                8 * sizeof(int);

                //binaryFile << counter << ")" << std::endl;
        //binaryFile << path << std::endl;

        //vec2_i fullMapPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(fd.G_Lat, fd.G_Long, counter + m_MinZoom, 512);

        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << fullMapPixelCoords.x_i;
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << fullMapPixelCoords.y_i;
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd, sizeof(fd));

        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.pointNumber, sizeof(fd.pointNumber));

        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.sizeSig_C1, sizeof(fd.sizeSig_C1));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.sizeSig_C2, sizeof(fd.sizeSig_C2));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.sizeSig_C3, sizeof(fd.sizeSig_C3));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.sizeSig_S1, sizeof(fd.sizeSig_S1));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.sizeSig_S2, sizeof(fd.sizeSig_S2));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.sizeSig_S3, sizeof(fd.sizeSig_S3));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&fd.sizeSwr, sizeof(fd.sizeSwr));

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

        //auto Sig_C1_Length = fd.Sig_C1.size();
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&Sig_C1_Length, sizeof(Sig_C1_Length));
        for (double j: fd.Sig_C1)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        //auto Sig_C2_Length = fd.Sig_C2.size();
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&Sig_C2_Length, sizeof(Sig_C2_Length));
        for (double j: fd.Sig_C2)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        //auto Sig_C3_Length = fd.Sig_C3.size();
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&Sig_C3_Length, sizeof(Sig_C3_Length));
        for (double j: fd.Sig_C3)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        //auto Sig_S1_Length = fd.Sig_S1.size();
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&Sig_S1_Length, sizeof(Sig_S1_Length));
        for (double j: fd.Sig_S1)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        //auto Sig_S2_Length = fd.Sig_S2.size();
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&Sig_S2_Length, sizeof(Sig_S2_Length));
        for (double j: fd.Sig_S2)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        //auto Sig_S3_Length = fd.Sig_S3.size();
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&Sig_S3_Length, sizeof(Sig_S3_Length));
        for (double j: fd.Sig_S3)
            BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&j, sizeof(j));
            //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << j << " ";
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1] << std::endl;

        //auto Swr_Length = fd.Swr.size();
        //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&Swr_Length, sizeof(Swr_Length));
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

        currentDataSize += sizePointBytes;/*21 * sizeof(double) + (Sig_C1_Length+1) * sizeof(double) +
                (Sig_C2_Length+1) * sizeof(double) + (Sig_C3_Length+1) * sizeof(double) +
                (Sig_S1_Length+1) * sizeof(double) + (Sig_S2_Length+1) * sizeof(double) +
                (Sig_S3_Length+1) * sizeof(double) + (Swr_Length+1) * sizeof(double);*/

        countPoints++;
        totalCountPoints++;
        std::cout << "LOADING TRACK... (" << (double)totalCountPoints / (double)m_CountCoords * 100 << "%)" << std::endl;
    }

    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&countPoints, sizeof(countPoints));
    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_MinLatitude, sizeof(m_MinLatitude));
    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_MaxLatitude, sizeof(m_MaxLatitude));
    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_MinLongitude, sizeof(m_MinLongitude));
    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_MaxLongitude, sizeof(m_MaxLongitude));

    m_CenterLatLong = vec2_d(m_MinLatitude + (m_MaxLatitude - m_MinLatitude) / 2.0,
                             m_MinLongitude + (m_MaxLongitude - m_MinLongitude) / 2.0);

    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_CenterLatLong.x_d, sizeof(m_CenterLatLong.x_d));
    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_CenterLatLong.y_d, sizeof(m_CenterLatLong.y_d));

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


    //BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&m_SideMeters, sizeof(m_SideMeters));


    COUNT_BINARY_FILES = BinaryFilesWrite.size();
    if (COUNT_BINARY_FILES != 1)
    {
        char loadedFull = 'L';
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&countPoints, sizeof(countPoints));
        //std::cout << "cp = " << countPoints << std::endl;
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&loadedFull, sizeof(loadedFull));
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].close();
    }

    std::ofstream in(FirstBinaryFileName, std::ifstream::in | std::ifstream::binary);
    in.seekp(-sizeof(char) - 2 * sizeof(int) - 3 * sizeof(double), std::ios::end);
    in.write((char*)&m_CenterLatLong.x_d, sizeof(m_CenterLatLong.x_d));
    in.write((char*)&m_CenterLatLong.y_d, sizeof(m_CenterLatLong.y_d));
    in.write((char*)&m_SideMeters, sizeof(m_SideMeters));
    in.write((char*)&COUNT_BINARY_FILES, sizeof(COUNT_BINARY_FILES));
    if (COUNT_BINARY_FILES == 1)
    {
        char loadedFull = 'L';
        in.write((char*)&countPoints, sizeof(countPoints));
        in.write((char*)&loadedFull, sizeof(loadedFull));
    }
    in.close();

    /*if (COUNT_BINARY_FILES != 1)
    {
        char loadedFull = 'L';
        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&loadedFull, sizeof(loadedFull));
    }
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].close();*/

}

void TrackLoader::SetCenterLatLongAndSideMeters() {
    std::ifstream in;
    in.open(FirstBinaryFileName);
    in.seekg(-sizeof(char) - 2 * sizeof(int) - 3 * sizeof(double), std::ios::end);
    in.read((char*)&m_CenterLatLong.x_d, sizeof(double));
    in.read((char*)&m_CenterLatLong.y_d, sizeof(double));
    in.read((char*)&m_SideMeters, sizeof(double));
    in.close();
}
