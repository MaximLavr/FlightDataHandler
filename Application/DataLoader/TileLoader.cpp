#include "TileLoader.h"

using namespace app_loader;

#define MAPBOX_SATELLITE 1

TileLoader::TileLoader(std::string mapType, int z, int x, int y)
{
    m_X = x;
    m_Y = y;
    m_Z = z;
    m_MapType = std::move(mapType);
}

bool file_exists(const char *filename)
{
    FILE *file;
    return file = fopen(filename, "r");
}

size_t TileLoader::Callbackfunction(void* ptr, size_t size, size_t count, void* userdata)
{
    FILE* stream = (FILE*)userdata;
    size_t written = fwrite((FILE*)ptr, size, count, stream);
    return written;
}

bool TileLoader::IsTileLoadedSuccessful(std::string filename)
{
    return std::__fs::filesystem::file_size(filename.c_str()) > 0;
}

std::string TileLoader::DownloadTile(const char* folderName) {
    const char* numLayer_char = std::to_string(m_Z).c_str();
    const char* x_char = std::to_string(int(m_X)).c_str();
    const char* y_char = std::to_string(int(m_Y)).c_str();

    CURL* curl;
    FILE* fp;
    int result;

    m_FolderName = folderName;
    std::string folderNameString = m_FolderName;

    char url[200];
    if (m_MapType == "Satellite") {
        strcpy(url, "https://api.mapbox.com/v4/mapbox.satellite/");
        strcat(url, numLayer_char);
        strcat(url, "/");
        strcat(url, x_char);
        strcat(url, "/");
        strcat(url, y_char);
        strcat(url, "@2x.pngraw?access_token=pk.eyJ1IjoibWF4aW04ODIwMDMiLCJhIjoiY2xqNDFoZWRhMWlqaTNtcDlxMHNiZm1meiJ9.KkQUNWhwiOMpH164P97Sqw");
        //https://api.mapbox.com/v4/mapbox.satellite/14/3826/6127@2x.pngraw?access_token=pk.eyJ1IjoibWF4aW04ODIwMDMiLCJhIjoiY2xqNDFoZWRhMWlqaTNtcDlxMHNiZm1meiJ9.KkQUNWhwiOMpH164P97Sqw
    }
    else if (m_MapType == "Elevation"){
        //https://api.mapbox.com/v/mapbox.terrain-rgb/14/3826/6127.pngraw?access_token=YOUR_MAPBOX_ACCESS_TOKEN
        //https://api.mapbox.com/v/mapbox.terrain-rgb/14/3826/6127.pngraw?access_token=pk.eyJ1IjoibWF4aW04ODIwMDMiLCJhIjoiY2xqNDFoZWRhMWlqaTNtcDlxMHNiZm1meiJ9.KkQUNWhwiOMpH164P97Sqw
        strcpy(url, "https://api.mapbox.com/v4/mapbox.terrain-rgb/");
        strcat(url, numLayer_char);
        strcat(url, "/");
        strcat(url, x_char);
        strcat(url, "/");
        strcat(url, y_char);
        strcat(url, "@2x.pngraw?access_token=pk.eyJ1IjoibWF4aW04ODIwMDMiLCJhIjoiY2xqNDFoZWRhMWlqaTNtcDlxMHNiZm1meiJ9.KkQUNWhwiOMpH164P97Sqw");
    }

    char img_path[200];
    strcpy(img_path, ("../images/" + folderNameString + "/").c_str());
    strcat(img_path, numLayer_char);
    strcat(img_path, "_zoom/");
    strcat(img_path, numLayer_char);
    strcat(img_path, "_");
    strcat(img_path, x_char);
    strcat(img_path, "_");
    strcat(img_path, y_char);
    strcat(img_path, ".png");

    if (!file_exists(img_path))
    {
        fp = fopen(img_path, "wb");

        curl = curl_easy_init();

        curl_easy_setopt(curl, CURLOPT_URL, url);

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 1);
        const char* pathToCertificate = "../SparkyEngine/certificates/cacert.pem";
        curl_easy_setopt(curl, CURLOPT_CAINFO, pathToCertificate);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Callbackfunction);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

        result = curl_easy_perform(curl);

        fclose(fp);

        curl_easy_cleanup(curl);
    }

    if (!IsTileLoadedSuccessful(img_path))
        DownloadTile(m_FolderName);

    return img_path;
}

