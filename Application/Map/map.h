#pragma once
#include "map_part.h"
#include "../../SparkyEngine/src/engine.h"
#include "../Utils/CoordsConverter.h"
#include <vector>

//map = new Map(float x, float y, float width, float height, vec2 coords_center, int z)

using namespace sparky;
using namespace graphics;

namespace app_graphics
{
    class Map
    {
    private:
        std::vector<Map_part*> m_Cache;
        Shader* m_Shader;
        maths::vec2 m_Position;
        maths::vec2 m_Size;
        maths::vec2 m_Quality;
        maths::vec3 m_CenterLongLatCoords;
        maths::mat4 m_ProjectionMatrix;
        int m_TilesCountWidth = 0;
        int m_TilesCountHeight = 0;
        maths::vec2 m_MapMovePosition;
        maths::vec2 m_MapMoveSize;

        std::vector<Map_part*> m_LeftTiles;
        std::vector<Map_part*> m_RightTiles;
    public:
        enum Sides { Left, Right, Bottom, Up };
        Map(float x, float y, float width, float height, maths::vec2 quality, Shader* shader, maths::mat4 projectionMatrix);
        Map(float x, float y, float width, float height, maths::vec3 centerTileCoords, maths::vec2 quality,  Shader* shader, maths::mat4 projectionMatrix);
        void render();
        void add(vec2 position, vec2 size, float x, float y, float zoom);
        void delete_side(Sides side);
        std::vector<Map_part*> m_MapParts;
        void move_right();
        void move_left();
        void move_up();
        void move_down();

        void add_side(Sides side);
        void move();

        vec2 getPosition() { return m_Position; }
        vec2 getMovePosition() { return m_MapMovePosition; }
        vec2 getSize() { return m_Size; }

        int total_tiles_count = 0;
        int m_TilesCounter = 0;

        void RemoveMapPart(Map_part* mapPart);
        void RemoveElements(int begin, int end);

        int getCountTileWidth() { return m_TilesCountWidth; }
        int getCountTileHeight() { return m_TilesCountHeight; }
    };
}

