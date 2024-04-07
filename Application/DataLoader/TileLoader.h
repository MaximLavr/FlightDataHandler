#pragma once

#include <curl/curl.h>
#include <vector>
#include "../../SparkyEngine/src/engine.h"
#include "../Map/Tile.h"

using namespace sparky;
using namespace maths;

namespace app_loader
{
    class TileLoader {
    private:
        int m_X, m_Y, m_Z;
        const char* m_FolderName;
        std::string m_MapType;
        static size_t Callbackfunction(void* ptr, size_t size, size_t count, void* userdata);
    public:
        TileLoader(std::string mapType, int x, int y, int z);
        std::string DownloadTile(const char* folderName);
        bool IsTileLoadedSuccessful(std::string filename);
        ~TileLoader() { }
    };
}
