#include "map_part.h"
#include "map.h"
#include <sys/stat.h>

namespace app_graphics
{
    Map_part::Map_part(vec2 mapPosition, Shader* shader, maths::mat4 projectionMatrix)
        : Layer(new BatchRenderer2D, shader, projectionMatrix, false)
    {
        m_Shader = shader;
        m_ProjectionMatrix = projectionMatrix;
        m_MapPosition = mapPosition;
    }

    bool file_exists (const char *filename)
    {
        FILE *file;
        return file = fopen(filename, "r");
    }

    long getFileSize(std::string filename)
    {
        struct stat stat_buf;
        int rc = stat(filename.c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : -1;
    }

    void Map_part::getTile(float x, float y, int numLayer)
    {
        const char* numLayer_char = std::to_string(numLayer).c_str();
        const char* x_char = std::to_string(int(x)).c_str();
        const char* y_char = std::to_string(int(y)).c_str();

        CURL* curl;
        FILE* fp;
        int result;

        char url[200];
        strcpy(url, "https://api.mapbox.com/v4/mapbox.satellite/");
        strcat(url, numLayer_char);
        strcat(url, "/");
        strcat(url, x_char);
        strcat(url, "/");
        strcat(url, y_char);
        strcat(url, "@2x.pngraw?access_token=pk.eyJ1Ijoia2FwYXN0b3IiLCJhIjoiY2p3eTg3eWJoMG1jZjQ4bzZmcGg5c3F3cSJ9.vhyCyD9xDDGP9EQnhB9xtA");

        char img_path[200];
        strcpy(img_path, "../images/map/satellite_images/img_map");
        strcat(img_path, x_char);
        strcat(img_path, "_");
        strcat(img_path, y_char);
        strcat(img_path, "_");
        strcat(img_path, numLayer_char);
        strcat(img_path, ".png");

        if (!file_exists(img_path))
        {
            fp = fopen(img_path, "wb");

            curl = curl_easy_init();

            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

            result = curl_easy_perform(curl);
            std::cout << "RESULT = " << result << std::endl;

            fclose(fp);

            curl_easy_cleanup(curl);
        }

        m_ImagePath = img_path;
    }

    void Map_part::AddTile(vec2 position, vec2 size, float x, float y, float zoom)
    {
        getTile(x, y, zoom);
        if (getFileSize(m_ImagePath) != -1)
        {
            auto* sprite = new Sprite(m_MapPosition.x + position.x, m_MapPosition.y + position.y, size.x, size.y, new Texture("final_map", m_ImagePath));
            this->add(sprite);
            renderables.push_back(sprite);
            tile_counter++;
        }
        //y reverse
        //x normal
        //maxidom: 87608, 40274, 17
        //[56.82091206787613,60.62468449999998]
        //1000 tiles ~ 150 fps
    }

    Map_part::~Map_part()
    {
        for (auto & r : m_Renderables)
        {
            delete r;
        }
    }
}
