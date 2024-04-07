#include "BinaryLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../../SparkyEngine/lib/stb_img/stb_img.h"

using namespace app_loader;

void deleteDirectoryContents(const std::string& dir_path)
{
    for (const auto& entry : std::__fs::filesystem::directory_iterator(dir_path))
        std::__fs::filesystem::remove_all(entry.path());
}

std::string GetAbsolutePath(std::string fileName)
{
    std::__fs::filesystem::path p = fileName;
    return std::__fs::filesystem::absolute(p);
}

int BinaryLoader::GetCountConstants(int z)
{
    return z - MinZoom;
}

bool BinaryLoader::IsFileExist(const char *filename)
{
    FILE *file;
    return file = fopen(filename, "r");
}

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

std::vector<std::string> s_split( const std::string& in, const std::string& delim ) {

    const std::regex re{delim} ;

    return {
            std::sregex_token_iterator( in.begin(), in.end(), re, -1 ),
            std::sregex_token_iterator()
    };
}

std::string getFileExtension(std::string fileName)
{
    size_t dot_index = -1;
    dot_index = fileName.find_last_of('.');
    if (dot_index != -1)
        fileName.erase(0, dot_index + 1);
    return fileName;
}

void removeFileExtension(const std::string& fileName)
{
    size_t dot_index = -1;
    dot_index = fileName.find_last_of('.');
    if (dot_index != -1)
        fileName.substr(0, dot_index);
}

void BinaryLoader::DeleteBinaryFiles()
{
    std::string dir_path = "../images/" + m_FolderName;
    for (const auto& entry : std::__fs::filesystem::directory_iterator(dir_path)) {
        std::string file_path = entry.path();
        if (getFileExtension(file_path) == "bin")
            std::remove(file_path.c_str());
    }
}

std::vector<std::string> BinaryLoader::GetBinaryFilesPaths()
{
    std::vector<std::string> binaryFilesPaths;
    std::string dir_path = "../images/" + m_FolderName;
    for (const auto& entry : std::__fs::filesystem::directory_iterator(dir_path)) {
        std::string file_path = entry.path();
        if (getFileExtension(file_path) == "bin") {
            binaryFilesPaths.push_back(file_path);
        }
    }

    return binaryFilesPaths;
}

void BinaryLoader::DeleteUnnecessaryTiles()
{
    for (int i = MinZoom; i <= MaxZoom; i++) {
#if 1
        std::string dir_path = "../images/" + m_FolderName + "/" + std::to_string(i) + "_zoom/";
        for (const auto& entry : std::__fs::filesystem::directory_iterator(dir_path)) {
            std::string tile_path = entry.path();

            if (getFileExtension(tile_path) != "png")
                continue;

            std::string tile_name = tile_path;

            int index_slash = tile_name.rfind('/');
            tile_name = tile_name.erase(0, index_slash + 1);

            auto z_x_y_string = s_split(tile_name,"_");

            int z = std::stoi(z_x_y_string[0]);
            int x = std::stoi(z_x_y_string[1]);
            int y = std::stoi(z_x_y_string[2]);

            if (std::__fs::filesystem::file_size(tile_path.c_str()) <= 0)
                remove(tile_path.c_str());

            if (x < constants[i - MinZoom].xTileStartDownload || y < constants[i - MinZoom].yTileStartDownload)
            {
                if (IsFileExist(tile_path.c_str())) {
                    remove(tile_path.c_str());
                }
            }

            if (x > constants[i - MinZoom].xTileStartDownload + constants[i - MinZoom].xTileCountDownload - 1 ||
                y > constants[i - MinZoom].yTileStartDownload + constants[i - MinZoom].yTileCountDownload - 1)
            {
                if (IsFileExist(tile_path.c_str())) {
                    remove(tile_path.c_str());
                }
            }
        }
#endif
    }
}

bool BinaryLoader::IsDirExists(const char *dirname)
{
    struct stat info{};

    if(stat(dirname, &info ) != 0)
        return false;
    else if(info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}

vec2_i BinaryLoader::CalculateCenterMapTileCoords(int z)
{
    return app_utils::CoordsConverter::LatLongToTile(CenterPoint.x_d, CenterPoint.y_d, z);
}

char* BinaryLoader::ImageToArrayBytes(std::string image_path, int &size)
{
    FreeImage_Initialise();
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    FIBITMAP *dib = nullptr;
    fif = FreeImage_GetFileType(image_path.c_str(), 0);
    if(fif == FIF_UNKNOWN)
        fif = FreeImage_GetFIFFromFilename(image_path.c_str());
    if(FreeImage_FIFSupportsReading(fif))
        dib = FreeImage_Load(fif, image_path.c_str());
    BYTE* pixels = FreeImage_GetBits(dib);
    GLsizei width, height, bits;
    width = FreeImage_GetWidth(dib);
    height = FreeImage_GetHeight(dib);
    bits = FreeImage_GetBPP(dib);

    size = width * height * (bits / 8);

    char result[size];
    memcpy(result, pixels, size);
    FreeImage_DeInitialise();
    FreeImage_Unload(dib);

    return result;
}

void BinaryLoader::CountTilesAroundCenterToDisplay(vec2_d centerTilePos) const
{
    auto temp = (double)(centerTilePos.x_d - m_MapPosition.x) / m_TileSide;
    if (temp < 0)
        temp = 0;
    else if (temp > int(temp))
        temp += 1;
    CountTilesToDisplay.Left = int(temp);

    temp = (double)((m_MapPosition.x + m_MapSideX) - (centerTilePos.x_d + m_TileSide)) / m_TileSide;
    if (temp < 0)
        temp = 0;
    else if (temp > int(temp))
        temp += 1;
    CountTilesToDisplay.Right = int(temp);

    temp = (double)(centerTilePos.y_d - m_MapPosition.y) / m_TileSide;
    if (temp < 0)
        temp = 0;
    else if (temp > int(temp))
        temp += 1;
    CountTilesToDisplay.Down = int(temp);

    temp = (double)((m_MapPosition.y + m_MapSideY) - (centerTilePos.y_d + m_TileSide)) / m_TileSide;
    if (temp < 0)
        temp = 0;
    else if (temp > int(temp))
        temp += 1;
    CountTilesToDisplay.Up = int(temp);
}

void BinaryLoader::CountTilesAroundCenterAreaToDownload(int zoom)
{
    //CENTER_POINT = vec2_d: (56.833332, 60.583332)
    //40 * 40 км

    //double areaMetersWidth = WidthMeters * 1000;
    //double areaMetersHeight = HeightKilometers * 1000;
    double halfSideWidth = WidthMeters / 2.0;
    double halfSideHeight = HeightMeters / 2.0;

    auto pointUp = app_utils::CoordsConverter::GetLatLongFromPointDistanceAngle(CenterPoint, halfSideHeight, 0);
    auto pointDown = app_utils::CoordsConverter::GetLatLongFromPointDistanceAngle(CenterPoint, -halfSideHeight, 0);
    auto pointLeft = app_utils::CoordsConverter::GetLatLongFromPointDistanceAngle(CenterPoint, halfSideWidth, 360-90);
    auto pointRight = app_utils::CoordsConverter::GetLatLongFromPointDistanceAngle(CenterPoint, -halfSideWidth, 360-90);

    auto centerTileCoords = app_utils::CoordsConverter::LatLongToTile(CenterPoint.x_d, CenterPoint.y_d, zoom);

    auto tileCoordsUp = app_utils::CoordsConverter::LatLongToTile(pointUp.x_d, pointUp.y_d, zoom);
    auto tileCoordsDown = app_utils::CoordsConverter::LatLongToTile(pointDown.x_d, pointDown.y_d, zoom);
    auto tileCoordsLeft = app_utils::CoordsConverter::LatLongToTile(pointLeft.x_d, pointLeft.y_d, zoom);
    auto tileCoordsRight = app_utils::CoordsConverter::LatLongToTile(pointRight.x_d, pointRight.y_d, zoom);

    /*std::cout << "zoom = " << zoom << std::endl;
    std::cout << "centerTileCoords = " << centerTileCoords << std::endl;
    std::cout << "tileCoordsUp = " << tileCoordsUp << std::endl;
    std::cout << "tileCoordsDown = " << tileCoordsDown << std::endl;
    std::cout << "tileCoordsLeft = " << tileCoordsLeft << std::endl;
    std::cout << "tileCoordsRight = " << tileCoordsRight << std::endl;*/

    if (tileCoordsUp.y_i > centerTileCoords.y_i - CountTilesToDisplay.Up)
        tileCoordsUp.y_i = centerTileCoords.y_i - CountTilesToDisplay.Up;
    if (tileCoordsDown.y_i < centerTileCoords.y_i + CountTilesToDisplay.Down)
        tileCoordsDown.y_i = centerTileCoords.y_i + CountTilesToDisplay.Down;
    if (tileCoordsLeft.x_i > centerTileCoords.x_i - CountTilesToDisplay.Left)
        tileCoordsLeft.x_i = centerTileCoords.x_i - CountTilesToDisplay.Left;
    if (tileCoordsRight.x_i < centerTileCoords.x_i + CountTilesToDisplay.Right)
        tileCoordsRight.x_i = centerTileCoords.x_i + CountTilesToDisplay.Right;

    CountTilesToDownload.Left = centerTileCoords.x_i - tileCoordsLeft.x_i;
    CountTilesToDownload.Right = tileCoordsRight.x_i - centerTileCoords.x_i ;
    CountTilesToDownload.Down = tileCoordsDown.y_i - centerTileCoords.y_i;
    CountTilesToDownload.Up = centerTileCoords.y_i - tileCoordsUp.y_i;

    /*std::cout << "center tile = " << centerTileCoords << std::endl;
    std::cout << "count LEFT = " << CountTilesToDownload.Left << std::endl;
    std::cout << "count RIGHT = " << CountTilesToDownload.Right << std::endl;
    std::cout << "count UP = " << CountTilesToDownload.Up << std::endl;
    std::cout << "count DOWN = " << CountTilesToDownload.Down << std::endl;*/
}

vec2_d BinaryLoader::CalculateCenterTileCoordsOnScreenForSecondZoom(vec2 mapPosition, double mapSide, double tileSide)
{
    auto centerTileCoords = app_utils::CoordsConverter::LatLongToTile(CenterPoint.x_d, CenterPoint.y_d, MIN_ZOOM);
    auto tile_side = app_utils::TILE_SIDE;
    auto centerPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(CenterPoint.x_d, CenterPoint.y_d, MIN_ZOOM, app_utils::TILE_SIDE);
    centerPixelCoords.x_i = int(round(centerPixelCoords.x_i)) % tile_side;
    centerPixelCoords.y_i = tile_side - 1 - int(round(centerPixelCoords.y_i)) % tile_side;

    const double pixelToDouble = 0.00488281;
    const double halfPixelToDouble = 0.00488281 / 2.0;

    auto center_pixel_floats = new vec2_d(centerPixelCoords.x_i * pixelToDouble, centerPixelCoords.y_i * pixelToDouble);

    vec2_d centerTilePos;
    auto startPos = vec2_d(mapPosition.x,mapPosition.y + mapSide - tileSide);
    centerTilePos = vec2_d(startPos.x_d + centerTileCoords.x_i * tileSide,
                           startPos.y_d - centerTileCoords.y_i * tileSide);
    m_Diff.x_d = mapSide / 2.0 - (tileSide / 2.5f) * (center_pixel_floats->x_d - halfPixelToDouble) - centerTilePos.x_d;
    m_Diff.y_d = mapSide / 2.0 - (tileSide / 2.5f) * (center_pixel_floats->y_d - halfPixelToDouble) - centerTilePos.y_d;

    return centerTilePos;
}

vec2_d BinaryLoader::CalculateCenterTileCoordsOnScreen(int z, vec2 mapPosition, double mapSideX, double mapSideY, double tileSide)
{
    auto centerTileCoords = app_utils::CoordsConverter::LatLongToTile(CenterPoint.x_d, CenterPoint.y_d,
                                                                      z);
    auto tile_side = app_utils::TILE_SIDE;
    auto centerPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(CenterPoint.x_d, CenterPoint.y_d, z, app_utils::TILE_SIDE);
    centerPixelCoords.x_i = int(round(centerPixelCoords.x_i)) % tile_side;
    centerPixelCoords.y_i = tile_side - 1 - int(round(centerPixelCoords.y_i)) % tile_side;

    const double pixelToDouble = 0.00488281;
    const double halfPixelToDouble = 0.00488281 / 2.0;

    auto center_pixel_floats = new vec2_d(centerPixelCoords.x_i * pixelToDouble, centerPixelCoords.y_i * pixelToDouble);

    vec2_d centerTilePos;
    if (z == 2) {
        auto startPos = vec2_d(mapPosition.x,mapPosition.y + mapSideY - tileSide);
        centerTilePos = vec2_d(startPos.x_d + centerTileCoords.x_i * tileSide,
                               startPos.y_d - centerTileCoords.y_i * tileSide);
        m_Diff.x_d = mapSideX / 2.0 - (tileSide / 2.5f) * (center_pixel_floats->x_d - halfPixelToDouble) - centerTilePos.x_d;
        m_Diff.y_d = mapSideY / 2.0 - (tileSide / 2.5f) * (center_pixel_floats->y_d - halfPixelToDouble) - centerTilePos.y_d;
    }
    else
    {
        centerTilePos.x_d = mapSideX / 2.0 - (tileSide / 2.5f) * (center_pixel_floats->x_d - halfPixelToDouble) - m_Diff.x_d;
        centerTilePos.y_d = mapSideY / 2.0 - (tileSide / 2.5f) * (center_pixel_floats->y_d - halfPixelToDouble) - m_Diff.y_d;
    }
    /*if (z == MinZoom)
    {
        CalculateCenterTileCoordsOnScreenForSecondZoom(mapPosition, mapSide, tileSide);
    }
    else if (z != MIN_ZOOM)
    {
        centerTilePos.x_d = mapSide / 2.0 - (tileSide / 2.5f) * (center_pixel_floats->x_d - halfPixelToDouble) - m_Diff.x_d;
        centerTilePos.y_d = mapSide / 2.0 - (tileSide / 2.5f) * (center_pixel_floats->y_d - halfPixelToDouble) - m_Diff.y_d;
    }*/

    return centerTilePos;
}

bool BinaryLoader::IsFileFull(const char *filename) { //!!!!!!!!

    std::ifstream in(filename, std::ifstream::in | std::ifstream::binary);
    in.seekg(-sizeof(int), std::ios::end);
    //in.seekg(-sizeof(int) - sizeof(int), std::ios::end);
    //in.seekg(-sizeof(int) - sizeof(int) - sizeof(int), std::ios::end);

    int character;
    in.read((char*)&character, sizeof(character));
    in.close();

    return character == 112233;
}

bool BinaryLoader::IsFileExistsAndFullLoaded(const std::string& fileName)
{
    return IsFileExist(fileName.c_str()) && IsFileFull(fileName.c_str());
}

bool BinaryLoader::IsAllDataFilesAlright()
{
    BinaryFilesPaths = GetBinaryFilesPaths();
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


    if (IsFileExistsAndFullLoaded(FileName))
    {
        std::ifstream firstBinaryFile;
        firstBinaryFile.open(FileName);
        int readCountFiles;
        firstBinaryFile.seekg(-sizeof(int) - sizeof(int) - sizeof(int), std::ios::end);
        firstBinaryFile.read((char*)&readCountFiles, sizeof(readCountFiles));
        firstBinaryFile.close();

        bool fullLoaded;
        fullLoaded = IsFileExistsAndFullLoaded(FileName);

        if (readCountFiles == BinaryFilesPaths.size())
        {
            for (int i = 0; i < BinaryFilesPaths.size(); i++)
            {
                std::ifstream in(BinaryFilesPaths[i], std::ios::binary);
                in.seekg(-sizeof(int), std::ios::end);
                int num;
                in.read((char*)&num, sizeof(num));
                if (num != 112233) {
                    return false;
                }
                else {
                    in.seekg(-sizeof(int) - sizeof(int), std::ios::end);
                    int tilesCount;
                    in.read((char*)&tilesCount, sizeof(tilesCount));
                    if (tilesCount > maxCountTilesInBinary) {
                        maxCountTilesInBinary = tilesCount;
                    }
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

BinaryLoader::BinaryLoader(std::string mapType, int minZoom, int maxZoom, vec2_d centerPoint, double widthMeters, double heightMeters,
                           vec2 mapPosition, double mapSideX, double mapSideY, double tileSide, const char* folderName)
{
    MinZoom = minZoom;
    MaxZoom = maxZoom;
    WidthMeters = widthMeters;
    HeightMeters = heightMeters;
    CenterPoint = centerPoint;
    m_MapPosition = mapPosition;
    m_MapSideX = mapSideX;
    m_MapSideY = mapSideY;
    m_TileSide = tileSide;
    m_FolderName = folderName;

    m_MapType = std::move(mapType);

    std::string relativePathToMapFolder = "../images/" + m_FolderName;

    if (!IsDirExists("../images"))
        std::__fs::filesystem::create_directory("../images");
    if (!IsDirExists(relativePathToMapFolder.c_str()))
        std::__fs::filesystem::create_directory(relativePathToMapFolder.c_str());


    for (int i = MinZoom; i <= MaxZoom; i++)
    {
        if (!IsDirExists((relativePathToMapFolder + "/" + std::to_string(i) + "_zoom").c_str()))
            std::__fs::filesystem::create_directory(relativePathToMapFolder + "/" + std::to_string(i) + "_zoom");
    }

    for (int i = MaxZoom + 1; i < 24; i++)
    {
        if (IsDirExists((relativePathToMapFolder + "/" + std::to_string(i) + "_zoom").c_str())) {
            std::__fs::filesystem::remove_all((relativePathToMapFolder + "/" + std::to_string(i) + "_zoom").c_str());
            //std::__fs::filesystem::remove((relativePathToMapFolder + "/" + std::to_string(i) + "_zoom").c_str());
        }
    }

    for (int i = MIN_ZOOM; i < MinZoom; i++)
    {
        if (IsDirExists((relativePathToMapFolder + "/" + std::to_string(i) + "_zoom").c_str())) {
            std::__fs::filesystem::remove_all((relativePathToMapFolder + "/" + std::to_string(i) + "_zoom").c_str());
            //std::__fs::filesystem::remove((relativePathToMapFolder + "/" + std::to_string(i) + "_zoom").c_str());
        }
    }

    FileName = relativePathToMapFolder + "/" +
            std::to_string(MinZoom) + "-" + std::to_string(MaxZoom) +
               "_" + std::to_string(centerPoint.x_d) + "_" + std::to_string(centerPoint.y_d) +
               "_" + std::to_string(widthMeters) +
               "_" + std::to_string(heightMeters) +
               "(0).bin";
    FileNameShort = relativePathToMapFolder + "/" +
                    std::to_string(MinZoom) + "-" + std::to_string(MaxZoom) +
                    "_" + std::to_string(centerPoint.x_d) + "_" + std::to_string(centerPoint.y_d) +
                    "_" + std::to_string(widthMeters) +
                    "_" + std::to_string(heightMeters);

    //f.open("../images/MainMap/mapBinaryLook.bin");

#if 0
    if (IsFileExistsAndFullLoaded(FileName))
    {
        IsLoadedFirstBinaryFile = true;
    }
    else
        DeleteBinaryFiles();  ///!!!@*/
#endif

    FirstZoomTileCoords = app_utils::CoordsConverter::LatLongToTile(centerPoint.x_d, centerPoint.y_d, MinZoom);
    CalculateCenterTileCoordsOnScreen(2, m_MapPosition, m_MapSideX, m_MapSideY, m_TileSide);

    int counter = 0;
    for (int i = MinZoom; i <= MaxZoom; i++) {
        constants.push_back(ConstantStructName);

        constants[counter].centerTileCoordsOnScreenX = CalculateCenterTileCoordsOnScreen(i, m_MapPosition, m_MapSideX, m_MapSideY, m_TileSide).x_d;
        constants[counter].centerTileCoordsOnScreenY = CalculateCenterTileCoordsOnScreen(i, m_MapPosition, m_MapSideX, m_MapSideY, m_TileSide).y_d;
        constants[counter].centerTileCoordX = CalculateCenterMapTileCoords(i).x_i;
        constants[counter].centerTileCoordY = CalculateCenterMapTileCoords(i).y_i;
        constants[counter].areaTileKm = app_utils::EARTH_FULL_AREA_KM2 / pow(2, 2*i);

        CountTilesAroundCenterToDisplay(vec2_d(constants[counter].centerTileCoordsOnScreenX,
                                               constants[counter].centerTileCoordsOnScreenY));

        constants[counter].xTileStartDisplay =  constants[counter].centerTileCoordX - CountTilesToDisplay.Left;
        constants[counter].yTileStartDisplay = constants[counter].centerTileCoordY - CountTilesToDisplay.Up;
        constants[counter].xTileCountDisplay = CountTilesToDisplay.Left + CountTilesToDisplay.Right + 1;
        constants[counter].yTileCountDisplay = CountTilesToDisplay.Up + CountTilesToDisplay.Down + 1;
        constants[counter].countOfTilesDisplay = constants[counter].xTileCountDisplay * constants[counter].yTileCountDisplay;

        CountTilesAroundCenterAreaToDownload(i);

        constants[counter].xTileStartDownload = constants[counter].centerTileCoordX - CountTilesToDownload.Left;
        constants[counter].yTileStartDownload = constants[counter].centerTileCoordY - CountTilesToDownload.Up;
        constants[counter].xTileCountDownload = CountTilesToDownload.Left + CountTilesToDownload.Right + 1;
        constants[counter].yTileCountDownload = CountTilesToDownload.Up + CountTilesToDownload.Down + 1;
        constants[counter].countOfTilesDownload = constants[counter].xTileCountDownload * constants[counter].yTileCountDownload;

        if (counter != 0) {
            m_TotalTiles += constants[counter - 1].countOfTilesDownload;
        }

        constants[counter].countTilesBefore  = m_TotalTiles;

        tilesCountLastZoom = constants[counter].countOfTilesDownload;
        counter++;
    }
    m_TotalTiles += tilesCountLastZoom;

#if 0
    if (!IsLoadedFirstBinaryFile)
    {
        DeleteUnnecessaryTiles();

        file_zooms_info = new std::ofstream(FileNameShort + "(" + std::to_string(BinaryFiles.size()) + ").bin", std::ios::out | std::ios::binary);
        BinaryFiles.push_back(file_zooms_info);
        SaveZoomToBinary();
    }
    else
    {
#if 1
        std::ifstream* firstFile = new std::ifstream(FileNameShort + "(" + std::to_string(0) + ").bin", std::ios::in | std::ios::binary);
        int readCountFiles;
        firstFile->seekg(-sizeof(int), std::ios::end);
        firstFile->read((char*)&readCountFiles, sizeof(readCountFiles));
        std::cout << "readCountFiles = " << readCountFiles << std::endl;
        firstFile->close();
        delete firstFile;

        BinaryFilesPaths = GetBinaryFilesPaths();
        if (readCountFiles == BinaryFilesPaths.size())
        {
            for (int i = 0; i < BinaryFilesPaths.size(); i++)
            {
                std::ifstream in(BinaryFilesPaths[i], std::ios::binary);
                in.seekg(-sizeof(int), std::ios::end);
                int num;
                in.read((char*)&num, sizeof(num));
                if (num != 112233) {
                    needReload = true;
                    break;
                }
                else {
                    in.seekg(-sizeof(int) - sizeof(int), std::ios::end);
                    int tilesCount;
                    in.read((char*)&tilesCount, sizeof(tilesCount));
                    if (tilesCount > maxCountTilesInBinary) {
                        maxCountTilesInBinary = tilesCount;
                    }
                }
                in.close();
            }
        }
        else
        {
            needReload = true;
        }
        /*for (int i = 0; i < BinaryFilesPaths.size(); i++)
        {
            std::ifstream in(BinaryFilesPaths[i], std::ios::binary);
            in.seekg(0, std::ios::end);
            int file_size = in.tellg();
            if (file_size < minBinaryFileSize)
            {
                minBinaryFileSize = file_size;
                minBinaryFileName = BinaryFilesPaths[i];
            }
        }*/
        if (/*!IsFileFull(minBinaryFileName.c_str())*/needReload)
        {
            IsLoadedFirstBinaryFile = false;
            DeleteUnnecessaryTiles();
            DeleteBinaryFiles();
            BinaryFiles.clear();

            file_zooms_info = new std::ofstream(FileNameShort + "(" + std::to_string(BinaryFiles.size()) + ").bin", std::ios::out | std::ios::binary);
            BinaryFiles.push_back(file_zooms_info);
            SaveZoomToBinary();
        }
#endif
    }
#endif
    if (!IsAllDataFilesAlright())
    {
        DeleteUnnecessaryTiles();
        DeleteBinaryFiles();
        BinaryFilesWrite.clear();
        BinaryFilesWrite.push_back(std::ofstream(FileName, std::ios::out | std::ios::binary));
        DownloadDataTiles();
        SaveZoomToBinary();
    }
    else {
        //DeleteBinaryFiles();
        IsLoaded = true;
        //DeleteUnnecessaryTiles();
    }
}

vec2_d BinaryLoader::GetTilePositionRelativeCenter(int z, int x, int y)
{
    //GetCenterTilePositionOnScreen(z);
    //GetFinalCenterMapTileCoords
    auto centerTileCoordX = constants[z - MinZoom].centerTileCoordX;
    auto centerTileCoordY = constants[z - MinZoom].centerTileCoordY;
    auto centerTilePosX = constants[z - MinZoom].centerTileCoordsOnScreenX;
    auto centerTilePosY = constants[z - MinZoom].centerTileCoordsOnScreenY;

    //m_TileSide
    int diffX = centerTileCoordX - x;
    int diffY = centerTileCoordY - y;

    auto ansX = centerTilePosX - diffX * m_TileSide;
    auto ansY = centerTilePosY + diffY * m_TileSide;

    return vec2_d(ansX, ansY);
}

void BinaryLoader::DownloadDataTiles()
{
    int constantsCounter = 0;
    float k = 0;
    for (int i = MinZoom; i <= MaxZoom; i++)
    {
        for (int x = constants[i - MinZoom].xTileStartDownload; x < constants[i - MinZoom].xTileStartDownload + constants[constantsCounter].xTileCountDownload; x++)
        {
            for (int y = constants[i - MinZoom].yTileStartDownload; y < constants[i - MinZoom].yTileStartDownload + constants[constantsCounter].yTileCountDownload; y++)
            {
                std::string image_path = "../images/" + m_FolderName + "/" + std::to_string(i) + "_zoom/" + std::to_string(i) + "_" +
                                         std::to_string(x) + "_" + std::to_string(y) + ".png";
                TileData tile;

                tile.zoom = i;
                tile.x = x;
                tile.y = y;

                TileLoader* tileLoader = new TileLoader(m_MapType, tile.zoom, tile.x, tile.y);

                if (!(IsFileExist(image_path.c_str()) && filesize(image_path.c_str()) > 0))
                    tileLoader->DownloadTile(m_FolderName.c_str());
                delete tileLoader;

                k++;
                std::cout << "DOWNLOADING MAP TILES... (" << int((double)k / m_TotalTiles * 100) << "%)" << std::endl;
            }
        }
        constantsCounter++;
    }
}

void BinaryLoader::SaveColors(const char* texturePath) {

}

void BinaryLoader::SaveZoomToBinary()
{

    int constantsCounter = 0;
#if 0
    for (int i = MinZoom; i <= MaxZoom; i++) {


        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].centerTileCoordsOnScreenX,
                               sizeof(constants[constantsCounter].centerTileCoordsOnScreenX));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].centerTileCoordsOnScreenY,
                               sizeof(constants[constantsCounter].centerTileCoordsOnScreenY));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].centerTileCoordX,
                               sizeof(constants[constantsCounter].centerTileCoordX));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].centerTileCoordY,
                               sizeof(constants[constantsCounter].centerTileCoordY));

        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].countOfTilesDisplay,
                               sizeof(constants[constantsCounter].countOfTilesDisplay));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].xTileCountDisplay,
                               sizeof(constants[constantsCounter].xTileCountDisplay));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].yTileCountDisplay,
                               sizeof(constants[constantsCounter].yTileCountDisplay));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].xTileStartDisplay,
                               sizeof(constants[constantsCounter].xTileStartDisplay));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].yTileStartDisplay,
                               sizeof(constants[constantsCounter].yTileStartDisplay));

        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].countOfTilesDownload,
                               sizeof(constants[constantsCounter].countOfTilesDownload));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].xTileCountDownload,
                               sizeof(constants[constantsCounter].xTileCountDownload));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].yTileCountDownload,
                               sizeof(constants[constantsCounter].yTileCountDownload));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].xTileStartDownload,
                               sizeof(constants[constantsCounter].xTileStartDownload));
        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].yTileStartDownload,
                               sizeof(constants[constantsCounter].yTileStartDownload));

        BinaryFiles[BinaryFiles.size() - 1]->write((char *) &constants[constantsCounter].countTilesBefore,
                               sizeof(constants[constantsCounter].countTilesBefore));


        //m_TotalTiles += constants[i - MinZoom].countOfTilesDownload;
        constantsCounter++;
    }

    constantsCounter = 0;
#endif
    auto start_point_for_tile = sizeof(constants.at(0))*constants.size() +
                               sizeof(Tile) * (m_TotalTiles);
    float k = 0;
    int prev_perc = -1;
    int cur_perc;
    int loaded_count = 0;
    int counterTilesLoaded = 0;
    for (int i = MinZoom; i <= MaxZoom; i++)
    {
        for (int x = constants[i - MinZoom].xTileStartDownload; x < constants[i - MinZoom].xTileStartDownload + constants[constantsCounter].xTileCountDownload; x++)
        {
            for (int y = constants[i - MinZoom].yTileStartDownload; y < constants[i - MinZoom].yTileStartDownload + constants[constantsCounter].yTileCountDownload; y++)
            {
                std::string image_path = "../images/" + m_FolderName + "/" + std::to_string(i) + "_zoom/" + std::to_string(i) + "_" +
                                         std::to_string(x) + "_" + std::to_string(y) + ".png";
                int s;
                auto result = ImageToArrayBytes(image_path, s);

                currentFileSize += sizeof(char) * s;

                if (currentFileSize > sizeOneFile)
                {
                    if (counterTiles > maxCountTilesInBinary)
                        maxCountTilesInBinary = counterTiles;

                    int loadedFull = 112233;
                    if (BinaryFilesWrite.size() - 1 == 0) {
                        int countFiles = 1;
                        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char *) &countFiles, sizeof(countFiles));
                    }
                    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&counterTiles, sizeof(counterTiles));
                    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&loadedFull, sizeof(loadedFull));
                    BinaryFilesWrite[BinaryFilesWrite.size() - 1].close();

                    //file_zooms_info->close();
                    currentFilePath = FileNameShort + "(" + std::to_string(BinaryFilesWrite.size()) + ").bin";
                    //delete file_zooms_info;

                    //file_zooms_info = ;
                    BinaryFilesWrite.push_back(std::ofstream(FileNameShort + "(" + std::to_string(BinaryFilesWrite.size()) + ").bin", std::ios::out | std::ios::binary));
                    //BinaryFiles.push_back(std::ofstream(FileNameShort + "(" + std::to_string(BinaryFiles.size()) + ").bin", std::ios::out | std::ios::binary));
                    currentFileSize = sizeof(char) * s;
                    countTilesInBinaryFile.push_back(counterTiles);

                    counterTiles = 0;
                }

                /*TileData tile;

                tile.zoom = i;
                tile.x = x;
                tile.y = y;*/

                /*auto tilePosRelativeCenter = GetTilePositionRelativeCenter(tile.zoom, tile.x, tile.y);
                tile.XPosOnMap = tilePosRelativeCenter.x_d;
                tile.YPosOnMap = tilePosRelativeCenter.y_d;

                TileLoader* tileLoader = new TileLoader(tile.zoom, tile.x, tile.y);

                if (!(IsFileExist(image_path.c_str()) && filesize(image_path.c_str()) > 0))
                    tileLoader->DownloadTile(m_FolderName.c_str());
                delete tileLoader;*/

                k++;

                std::cout << "LOADING MAP... (" << int((double)k / m_TotalTiles * 100) << "%)" << std::endl;


                /*tile.size = s;
                tile.startPoint = start_point_for_tile;*/

                //BinaryFiles[BinaryFiles.size() - 1]->write((char*)&tile, sizeof(TileData));
                //file_zooms_info->seekp(tile.startPoint);
                //f << &tile;
                //f.seekp(tile.startPoint);

                /*start_point_for_tile += tile.size;*/

                for (int h = 0; h < s; h++)
                {
                    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&result[h], sizeof(result[h]));
                }
                counterTilesLoaded += 1;

                /*SAVE ELEVATION COLORS*/
                int x1,y1,n1;
                int counter1 = 0;
                unsigned char *data = stbi_load(image_path.c_str(), &x1, &y1, &n1, 0);
                for (int i1 = 0; i1 < x1; i1++)
                {
                    for (int j1 = 0; j1 < y1; j1++)
                    {
                        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&data[counter1], sizeof(data[counter1]));
                        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&data[counter1 + 1], sizeof(data[counter1 + 1]));
                        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&data[counter1 + 2], sizeof(data[counter1 + 2]));
                        BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&data[counter1 + 3], sizeof(data[counter1 + 3]));
                        counter1+=4;
                    }
                }
                stbi_image_free(data);
                /*SAVE ELEVATION COLORS*/

                loaded_count++;
                counterTiles++;
                //file_zooms_info->seekp(sizeof(constants.at(0))*constants.size() + sizeof(Tile) * (loaded_count));
                //f.seekp(sizeof(constants.at(0))*constants.size() + sizeof(Tile) * (loaded_count));
            }
        }
        constantsCounter++;
    }
    countTilesInBinaryFile.push_back(counterTiles);
    int loadedFull = 112233;
    //BinaryFiles.size() - 1
    //2
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&counterTiles, sizeof(counterTiles));
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].write((char*)&loadedFull, sizeof(loadedFull));
    BinaryFilesWrite[BinaryFilesWrite.size() - 1].close();



    COUNT_BINARY_FILES = BinaryFilesWrite.size();
    std::ofstream in(FileName, std::ifstream::in | std::ifstream::binary);
    in.seekp(-sizeof(int) - sizeof(int) - sizeof(int), std::ios::end);
    in.write((char*)&COUNT_BINARY_FILES, sizeof(COUNT_BINARY_FILES));
    in.close();
    //BinaryFiles[0]->seekp(std::ios::end);
    //std::ofstream firstFile(FileName, std::ios::app);
    //firstFile << COUNT_BINARY_FILES;
    //firstFile.close();

    if (counterTiles > maxCountTilesInBinary)
        maxCountTilesInBinary = counterTiles;
    counterTiles = 0;
    //BinaryFiles[BinaryFiles.size() - 1]->close();
    //f << loadedFull;
    //f.close();

    //for (auto& file : BinaryFiles)
      //  file->close();

    //file_zooms_info->close();
    //delete file_zooms_info;

    IsLoaded = true;
    //COUNT_BINARY_FILES = BinaryFiles.size();
    //std::ofstream firstFile;
    //firstFile.open(FileName);
    //firstFile.write((char*)&COUNT_BINARY_FILES, sizeof(COUNT_BINARY_FILES));
    //std::cout << "COUNT_BINARY_FILES = " << COUNT_BINARY_FILES << std::endl;
    //firstFile.close();

    
    //file_zooms_info = new std::ofstream(FileNameShort + "(" + std::to_string(0) + ").bin", std::ios::out | std::ios::binary);
    //file_zooms_info->seekp(std::ios::end);
    //file_zooms_info->write((char*)&COUNT_BINARY_FILES, sizeof(COUNT_BINARY_FILES));
    //file_zooms_info->close();
    //delete file_zooms_info;
}

int BinaryLoader::GetCount(int z, int x, int y)
{
    int difX = x - constants[z - MinZoom].xTileStartDownload;
    int difY = y - constants[z - MinZoom].yTileStartDownload;

    return constants[z - MinZoom].countTilesBefore + difX * constants[z - MinZoom].yTileCountDownload + difY;
}

Texture* BinaryLoader::GetTileTexture(int z, int x, int y) {
    //5_20_11

    const int image_size = 1048576;
    int countTilesBefore = GetCount(z, x, y);
    int fileNumber = countTilesBefore / maxCountTilesInBinary;
    int countTilesBeforeInLastBinary = countTilesBefore - fileNumber * maxCountTilesInBinary;

    std::ifstream binary(FileNameShort + "(" + std::to_string(fileNumber) + ").bin", std::ios::in | std::ios::binary);

    binary.seekg(countTilesBeforeInLastBinary * image_size +
        countTilesBeforeInLastBinary * app_utils::TILE_SIDE * app_utils::TILE_SIDE * 4 * sizeof(unsigned char));

    char chars[image_size];
    int c = 0;
    for (int i = 0; i < image_size; i++)
    {
        binary.read((char*)&chars[c], sizeof(char));
        c++;
    }
    binary.close();

    Texture* texture_final = new Texture(chars);

    return texture_final;
}

std::vector<int> BinaryLoader::GetTileColors(int z, int x, int y) {
    //5_20_11
    const int image_size = 1048576;
    int countTilesBefore = GetCount(z, x, y);
    int fileNumber = countTilesBefore / maxCountTilesInBinary;
    int countTilesBeforeInLastBinary = countTilesBefore - fileNumber * maxCountTilesInBinary;

    std::ifstream binary(FileNameShort + "(" + std::to_string(fileNumber) + ").bin", std::ios::in | std::ios::binary);

    binary.seekg(countTilesBeforeInLastBinary * image_size +
                 countTilesBeforeInLastBinary * app_utils::TILE_SIDE * app_utils::TILE_SIDE * 4 * sizeof(unsigned char) +
                 image_size);

    std::vector<int> colors;
    int c = 0;
    for (int i = 0; i < 512 * 512 * 4; i++)
    {
        unsigned char color;
        binary.read((char*)&color, sizeof(color));
        colors.push_back(color & 0xff);
        c++;
    }
    binary.close();

    return colors;
}

vec2_d BinaryLoader::GetCenterTilePositionOnScreen(int z)
{
    std::ifstream binary(FileName, std::ios::in | std::ios::binary);

    size_t size;
    int diff = z - MinZoom;
    size = (diff) * (sizeof(constants[0])) - diff * sizeof(int);

    binary.seekg(size);

    double centerTileCoordsOnScreenX, centerTileCoordsOnScreenY;
    binary.read((char *)&centerTileCoordsOnScreenX, sizeof(centerTileCoordsOnScreenX));
    binary.read((char *)&centerTileCoordsOnScreenY, sizeof(centerTileCoordsOnScreenY));


    binary.close();

    return vec2_d(centerTileCoordsOnScreenX, centerTileCoordsOnScreenY);
}

/*vec2_d BinaryLoader::GetTilePositionRelativeCenterFromBinary(int z, int x, int y)
{
    std::ifstream binary(FileName, std::ios::in | std::ios::binary);
    int diff = z - MinZoom;
    int countBefore = constants[diff].countTilesBefore + (x - constants[diff].xTileStartDisplay) * constants[diff].yTileCountDisplay+ (y - constants[diff].yTileStartDisplay);

    double xPos = 0.0;
    double yPos = 0.0;
    if (countBefore != 0)
    {
        //auto size1 = sizeof(constants[0]) * constants.size() + countBefore * sizeof(TileData);
        auto size2 = sizeof(constants[0]) * constants.size() + countBefore * sizeof(TileData) + 2 * sizeof(double);
        binary.seekg(size2);
        binary.read((char*)&xPos, sizeof(double));
        binary.read((char*)&yPos, sizeof(double));
    }
    else
    {
        //auto size1 = sizeof(constants[0]) * constants.size() + countBefore * sizeof(TileData) - 2 * sizeof(double);
        auto size2 = sizeof(constants[0]) * constants.size() + countBefore * sizeof(TileData);
        binary.seekg(size2);
        binary.read((char*)&xPos, sizeof(double));
        binary.read((char*)&yPos, sizeof(double));
    }

    binary.close();

    return vec2_d(xPos, yPos);
}*/

vec2_i BinaryLoader::GetFinalCenterMapTileCoords(int z)
{
    std::ifstream binary(FileName, std::ios::in | std::ios::binary);
    size_t size;

    int diff = z - MinZoom;
    size = (diff) * (sizeof(constants[0])) + 2 * sizeof(double) - diff * sizeof(int);

    binary.seekg(size);

    int x, y;
    binary.read((char *)&x, sizeof(x));
    binary.read((char *)&y, sizeof(y));
    binary.close();

    return vec2_i(x, y);
}

void BinaryLoader::CheckSavedData()
{
    std::ifstream ifstr(FileName);
    for (int i = MinZoom; i <= MaxZoom; i++) {
        double centerTileCoordsOnScreenX;
        double centerTileCoordsOnScreenY;
        int countOfTiles;
        int xTileCount;
        int yTileCount;
        int xTileStart;
        int yTileStart;
        ifstr.read((char *)&centerTileCoordsOnScreenX, sizeof(centerTileCoordsOnScreenX));
        ifstr.read((char *)&centerTileCoordsOnScreenY, sizeof(centerTileCoordsOnScreenY));
        ifstr.read((char *)&countOfTiles, sizeof(countOfTiles));
        ifstr.read((char *)&xTileCount, sizeof(xTileCount));
        ifstr.read((char *)&yTileCount, sizeof(yTileCount));
        ifstr.read((char *)&xTileStart, sizeof(xTileStart));
        ifstr.read((char *)&yTileStart, sizeof(yTileStart));
        //std::cout << "countOfTiles = " << countOfTiles << std::endl;
        //std::cout << "xTileCount = " << xTileCount << std::endl;
        //std::cout << "yTileCount = " << yTileCount << std::endl;
        //std::cout << "xTileStart = " << xTileStart << std::endl;
        //std::cout << "yTileStart = " << yTileStart << std::endl;
    }
}

/*double BinaryLoader::GetAreaInOneTile(int tileZ, int tileX, int tileY)
{
    auto latLong = app_utils::CoordsConverter::TileToLatLong(tileX, tileY, tileZ);
    return app_utils::EQUATOR_LENGTH * cos(latLong.x_d) / pow(2, tileZ);
}*/

int BinaryLoader::GetCountOfTilesToDownload(int zoom)
{
    //CenterTileCoords
    //MapAreaKilometers
    //(!lat! long) CenterPoint
    //auto areaInOneTile =


    //auto tileCoordsLeftUp =
    //auto tileCoordsLeftDown =


    return 0;
}

double AngleFromCoordinate(double lat1, double long1, double lat2, double long2) {

    double dLon = (long2 - long1);

    double y = sin(dLon) * cos(lat2);
    double x = cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(dLon);

    auto all_func = atan2(sin(long2 - long1) * cos(lat2), cos(lat1) * sin(lat2) - sin(lat1) * cos(lat2) * cos(long2 - long1));

    double brng = atan2(y, x); //Radians

    return brng;
}


/*vec2_i BinaryLoader::GetLeftUpTileCoordsToLoad(int zoom)
{
    //auto tileCoordsLeftUp =
    //auto tileCoordsLeftDown =
    //CenterPint

    auto sourceLatitude = CenterPoint.x_d;
    auto sourceLongitude = CenterPoint.y_d;

    auto areaHalfSideKm = sqrt(MapAreaKilometers) / 2.0;




    return vec2_i(0, 0);
}*/
