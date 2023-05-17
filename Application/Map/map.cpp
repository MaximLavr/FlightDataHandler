#include "map.h"

namespace app_graphics
{
    Map::Map(float x, float y, float width, float height, maths::vec2 quality, Shader *shader, maths::mat4 projectionMatrix)
    {
        m_Position = vec2(x, y);
        m_Size = vec2(width, height);
        m_Quality = quality;
        m_Shader = shader;
        m_ProjectionMatrix = projectionMatrix;
        m_MapParts.push_back(new Map_part(m_Position, m_Shader, m_ProjectionMatrix));
        m_MapMovePosition = m_Position;
        m_MapMoveSize = m_Size;
    }

    Map::Map(float x, float y, float width, float height, maths::vec3 centerLongLatCoords, maths::vec2 quality,
             Shader *shader, maths::mat4 projectionMatrix)
    {
        m_Position = vec2(x, y);
        m_Size = vec2(width, height);
        m_CenterLongLatCoords = centerLongLatCoords;
        m_Quality = quality;
        m_Shader = shader;
        m_ProjectionMatrix = projectionMatrix;
        m_MapParts.push_back(new Map_part(m_Position, m_Shader, m_ProjectionMatrix));
        m_MapMovePosition = m_Position;
        m_MapMoveSize = m_Size;

        auto tile_size_x = m_Size.x / m_Quality.x;
        auto tile_size_y = m_Size.y / m_Quality.y;
        auto center_tile_coords = app_utils::CoordsConverter::LongLatToTile(m_CenterLongLatCoords.x, m_CenterLongLatCoords.y, (int)m_CenterLongLatCoords.z);

        if (m_CenterLongLatCoords.z == 2)
        {
            tile_size_x *= 1.25f;
            tile_size_y *= 1.25f;
        }

        for (int i = 0; i < m_Quality.x - 1; i++)
        {
            m_TilesCountHeight = 0;
            for (int j = 0; j < m_Quality.y - 1; j++)
            {
                this->add(vec2(float(-6 + tile_size_x * i),float(-4 + tile_size_y * j)),
                          vec2(tile_size_x, tile_size_y),
                          center_tile_coords.x_d - m_Quality.x / 2 + i + 1,
                          center_tile_coords.y_d - j + 2, m_CenterLongLatCoords.z);
                m_TilesCountHeight++;
            }
            m_TilesCountWidth++;
        }

        //left
        //add_side(vec2(-8.5f, 0), 4);

        //right
        //add_side(vec2(-6 + (float)m_TilesCountWidth * 2.5f, 0), 0);

        //add_side(vec2(4, 0), 0);
        //add_side(vec2(6.5f, 0), 2);

        /*this->add(vec2(-6, -4), vec2(2, 2), 87608, 40276, 17);
        this->add(vec2(-6, 0), vec2(2, 2), 87608, 40274, 17);
        this->add(vec2(-6, -2), vec2(2, 2), 87608, 40275, 17);
        this->add(vec2(-6, 2), vec2(2, 2), 87608, 40273, 17);
        this->add(vec2(-6, 4), vec2(2, 2), 87608, 40272, 17);

        this->add(vec2(-4, -4), vec2(2, 2), 87609, 40276, 17);
        this->add(vec2(-4, -2), vec2(2, 2), 87609, 40275, 17);
        this->add(vec2(-4, 0), vec2(2, 2), 87609, 40274, 17);
        this->add(vec2(-4, 2), vec2(2, 2), 87609, 40273, 17);
        this->add(vec2(-4, 4), vec2(2, 2), 87609, 40272, 17);

        this->add(vec2(-2, -4), vec2(2, 2), 87610, 40276, 17);
        this->add(vec2(-2, -2), vec2(2, 2), 87610, 40275, 17);
        //this->add(vec2(-2, 0), vec2(2, 2), 87610, 40274, 17);
        this->add(vec2(-2, 2), vec2(2, 2), 87610, 40273, 17);
        this->add(vec2(-2, 4), vec2(2, 2), 87610, 40272, 17);

        this->add(vec2(0, -4), vec2(2, 2), 87611, 40276, 17);
        this->add(vec2(0, -2), vec2(2, 2), 87611, 40275, 17);
        this->add(vec2(0, 0), vec2(2, 2), 87611, 40274, 17);
        this->add(vec2(0, 2), vec2(2, 2), 87611, 40273, 17);
        this->add(vec2(0, 4), vec2(2, 2), 87611, 40272, 17);

        this->add(vec2(2, -4), vec2(2, 2), 87612, 40276, 17);
        this->add(vec2(2, -2), vec2(2, 2), 87612, 40275, 17);
        this->add(vec2(2, 0), vec2(2, 2), 87612, 40274, 17);
        this->add(vec2(2, 2), vec2(2, 2), 87612, 40273, 17);
        this->add(vec2(2, 4), vec2(2, 2), 87612, 40272, 17);*/
    }

    void Map::render()
    {
        if (!m_MapParts.empty())
        {
            for (auto* map_part : m_MapParts)
            {
                map_part->render();
            }
        }
    }

    void Map::add(vec2 position, vec2 size, float x, float y, float zoom)
    {
        m_TilesCounter++;
        //std::cout << "MapPartsCount = " << m_MapParts.size() << std::endl;
        if (m_TilesCounter > 15 && m_TilesCounter % 16 == 0)
        {
            //added -= 15;
            //m_TilesCounter = 0;
            m_MapParts.push_back(new Map_part(m_Position, m_Shader, m_ProjectionMatrix));
        }
        m_MapParts.back()->AddTile(position, size, x, y, zoom);
    }

    int c = 0;
    int k1 = 0;
    int d = 0;
    void Map::RemoveElements(int beginIndex, int endIndex) // 32 70
    {
        //int k = beginIndex / 15; // mod = 2
        //int k2 = endIndex / 15; // mod = 4
        //int d = beginIndex % 15;
        //std::cout << "k = " << k << std::endl;
        for (int i = beginIndex; i <= endIndex; i++)
        {
            m_MapParts[k1]->remove(m_MapParts[k1]->renderables[d]);
            if (d % 16 == 0 && d > 0)
            {
                k1++;
                d = -1;
                std::cout << "new K = " << k1 << std::endl;
            }
            d++;
            m_TilesCounter--;
        }
    }

    int delete_counter = 0;
    int map_part_counter = 0;
    int a1= 0;
    int b1 = 0;
    void Map::delete_side(Sides side)
    {
        if (side == Right)
        {
            RemoveElements(m_TilesCounter - m_TilesCountHeight, m_TilesCounter - 1);
            m_TilesCountWidth -= 1;
            //m_MapMoveSize.x -= 2.5f;
        }
        if (side == Left)
        {
            RemoveElements(a1, m_TilesCountHeight - 1 + b1);
            //RemoveElements(0 + a1, 1+b1);
            m_TilesCountWidth -= 1;
            //m_MapMoveSize.x -= 2.5f;
            m_MapMovePosition.x += 2.5f;
            m_MapMoveSize.x -= 2.5f;
            a1+=4;
            b1+=4;
        }
    }

    int m_Delta = 0;
    float k = 2.5f;

    void Map::add_side(Sides side)
    {
        //Sides side
        //vec2 position, float delta
        if (m_Delta < 4) {
            m_Delta++;
        }
        else
            m_Delta = 1;

        //m_MapMovePosition.x -= 2.5f;
        vec2 position = vec2(m_MapMovePosition.x + m_MapMoveSize.x - 1.0f, 0.0f);
        m_MapMoveSize.x += 2.5f;

        auto tile_size_x = m_Size.x / m_Quality.x;
        auto tile_size_y = m_Size.y / m_Quality.y;
        auto center_tile_coords = app_utils::CoordsConverter::LongLatToTile(m_CenterLongLatCoords.x, m_CenterLongLatCoords.y, (int)m_CenterLongLatCoords.z);
        int height = m_Quality.x * position.x != 0 ? (int)m_Quality.x - 1 : 1;
        int width = m_Quality.y * position.y != 0 ? (int)m_Quality.y - 1 : 1;

        if (m_CenterLongLatCoords.z == 2)
        {
            tile_size_x *= 1.25f;
            tile_size_y *= 1.25f;
        }

        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {
                position.y = -4 + tile_size_y * j;
                this->add(position, vec2(tile_size_x, tile_size_y),
                          center_tile_coords.x_d - m_Quality.x / 2 + i + m_Delta, //m_Delata =
                          center_tile_coords.y_d - j + 2,
                          m_CenterLongLatCoords.z);
            }
            std::cout << "total tiles: " << m_TilesCounter << std::endl;
        }

        //m_MapMovePosition.x -= tile_size_x;
        //m_MapMovePosition.x -= 5.0f;
    }

    float counter = 0.0f;

    void Map::move()
    {
        //if (m_MapMovePosition.x > 0.0f)
            //add_side(Left);
            /*if (m_MapMovePosition.x == -5.5f) {
                delete_side(Left);
            }*/
                //std::cout << "On left line!" << std::endl;
        if (m_MapMovePosition.x/* + 2.5f*/ < -5.0f) {
            /*if (m_TilesCountWidth > 0)
            {*/
                delete_side(Left);
                //std::cout << "DELETE!" << std::endl;
            //}
        }
        if (m_MapMovePosition.x + m_MapMoveSize.x < 5.0f) {
            add_side(Left);
            //std::cout << "ADD!" << std::endl;
        }
        /*if (m_MapMovePosition.x > a) {
            add_side(Left);
            //delete_side("Right");
            //m_MapMovePosition.x -= 7.5f;
            //std::cout << "Slide added!" << std::endl;
        }

        if (m_MapMovePosition.x + 10.0f - 2.5f > 5.0f)
        {

        }*/

        //std::cout << "total tiles = " << m_TilesCounter << std::endl;
        //std::cout << "sprites width = " << m_TilesCountWidth << std::endl;
        //std::cout << "sprites height = " << m_TilesCountHeight << std::endl;

        /*if (m_MapMovePosition.x + 10 - 2.5f > a + 10) {
            delete_side(Right);
            std::cout << "Slide deleted!" << std::endl;
        }*/
        //std::cout << "pos.x = " << m_MapMovePosition.x << std::endl;
    }

    void Map::move_right()
    {
        for (auto& part : m_MapParts)
        {
            for (auto& sprite : part->renderables)
            {
                sprite->m_Position.x += 0.25f;
            }
        }

        m_MapMovePosition.x += 0.25f;

        /*if (m_MapMovePosition.x > -5.0f)
        {
            add_side(vec2(-8.3f - k, 0), 4);
        }*/

        //std::cout << "m_MapMovePosition.x = " << m_MapMovePosition.x << std::endl;
        //std::cout << "m_Position.x = " << m_Position.x << std::endl;
        //add_side(vec2(-8.3f - k, 0), 4);
    }

    void Map::move_left()
    {
        for (auto& part : m_MapParts)
        {
            for (auto& sprite : part->renderables)
            {
                sprite->m_Position.x-=0.25f;
            }
        }
        //std::cout << "m_MapMovePosition.x = " << m_MapMovePosition.x << std::endl;
        //std::cout << "m_Position.x = " << m_Position.x << std::endl;

        m_MapMovePosition.x -= 0.25f;
    }

    void Map::move_up()
    {
        if (m_CenterLongLatCoords.z != 2)
        {
            for (auto& part : m_MapParts)
            {
                for (auto& sprite : part->renderables)
                {
                    sprite->m_Position.y+=0.25f;
                }
            }
        }
    }

    void Map::move_down()
    {
        if (m_CenterLongLatCoords.z != 2)
        {
            for (auto& part : m_MapParts)
            {
                for (auto& sprite : part->renderables)
                {
                    sprite->m_Position.y+=0.25f;
                }
            }
        }
    }
}
