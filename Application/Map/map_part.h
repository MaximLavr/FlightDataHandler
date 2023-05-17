#pragma once
#include <curl/curl.h>
#include <iostream>
#include "../../SparkyEngine/src/sparky.h"

using namespace sparky;
using namespace graphics;

namespace app_graphics {
    class Map_part : public Layer {
    private:
        Shader* m_Shader;
        maths::mat4 m_ProjectionMatrix;
        std::string m_ImagePath;
        vec2 m_MapPosition;
    public:
        friend class Map;
        Map_part(maths::vec2 mapPosition, Shader* shader, maths::mat4 projectionMatrix);
        void getTile(float x, float y, int numLayer);
        void AddTile(vec2 position, vec2 size, float x, float y, float zoom);
        std::vector<Renderable2D*> renderables;
        int tile_counter = 0;
        ~Map_part();
    };
}
