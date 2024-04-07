#define DEBUG false
#include "map.h"

namespace app_graphics
{
    Map::Map(float x, float y, float side, MapType mapType, vec2_d zoomLatLongCoords, double widthMeters, double heightMeters,
             Shader* shader, mat4 ortho, Window* window, const char* folderName)
             : m_FolderName(folderName)
    {
        m_BoundingBoxPosition = vec2(x, y);
        m_BoundingBoxSize = vec2(side, side);

        PixelsInFloat = int(round((float)window->getWidth() / (2 * (window->getWidth() / 60.0f))));

        m_Position = m_BoundingBoxPosition;
        m_Size = vec2(side, side);

        CenterLatLongCoords = zoomLatLongCoords;
        WidthMeters = widthMeters;
        HeightMeters = heightMeters;

        m_Shader = shader;
        m_ProjectionMatrix = ortho;

        m_Window = window;

        mapLayer = new MapLayer(new BatchRenderer2D(m_Window), m_Shader, m_ProjectionMatrix, false);

        m_TileSide = m_Size.y/ 4.0;

        m_ZoomLatLongCoords = zoomLatLongCoords;

        shader->setUniform1f("left_position", m_BoundingBoxPosition.x);
        shader->setUniform1f("right_position", m_BoundingBoxPosition.x + m_BoundingBoxSize.x);
        shader->setUniform1f("up_position", m_BoundingBoxPosition.y + m_BoundingBoxSize.y);
        shader->setUniform1f("down_position",  m_BoundingBoxPosition.y);

        m_Shader = shader;
        m_ProjectionMatrix = ortho;
        m_MapType = mapType;

        if (m_FolderName.empty())
            m_FolderName = EnumToString(m_MapType);
    }

    void Map::LoadData(int minZoom, int maxZoom)
    {
        MinZoom = minZoom;
        MaxZoom = maxZoom;
        CurrentZoom = MinZoom;

        if (m_MapType == MapType::Elevation)
        {
            binaryLoader = new BinaryLoader("Elevation", MinZoom, MaxZoom, m_ZoomLatLongCoords, WidthMeters, HeightMeters,
                                            m_BoundingBoxPosition, m_Size.x, m_Size.y, m_TileSide, m_FolderName.c_str());
        }
        else if (m_MapType == MapType::Satellite)
        {
            binaryLoader = new BinaryLoader("Satellite", MinZoom, MaxZoom, m_ZoomLatLongCoords, WidthMeters, HeightMeters,
                                            m_BoundingBoxPosition, m_Size.x, m_Size.y, m_TileSide, m_FolderName.c_str());
        }
    }

    void Map::AddTiles()
    {
        if (binaryLoader->IsLoaded)
        {
            m_CenterTileCoords = binaryLoader->GetFinalCenterMapTileCoords(CurrentZoom);
            m_CenterTilePosition = binaryLoader->GetCenterTilePositionOnScreen(CurrentZoom);

            AddTilesAround(CurrentZoom);
        }
        else
        {
            std::cout << "Please load the map!" << std::endl;
        }
    }

    void Map::AddTilesAround(int zoom)
    {
        srand(time(0));
        TILES_ON_SCREEN = 0;
        firstAdd = true;
        binaryLoader->CountTilesAroundCenterToDisplay(m_CenterTilePosition);

        auto tileStartX = binaryLoader->constants[zoom - binaryLoader->MinZoom].xTileStartDisplay;
        auto tileStartY = binaryLoader->constants[zoom - binaryLoader->MinZoom].yTileStartDisplay;
        auto countDisplayX = binaryLoader->constants[zoom - binaryLoader->MinZoom].xTileCountDisplay;
        auto countDisplayY = binaryLoader->constants[zoom - binaryLoader->MinZoom].yTileCountDisplay;

        tilesCountWidth = countDisplayX;
        tilesCountHeight = countDisplayY;

        auto LeftUpPos = binaryLoader->GetTilePositionRelativeCenter(zoom, tileStartX, tileStartY);

        auto startPos = LeftUpPos;
        auto tileStartX_count = tileStartX;
        auto tileStartY_count = tileStartY;

        m_Size.x = countDisplayX * m_TileSide;
        m_Size.y = countDisplayY * m_TileSide;
        m_Position = vec2(LeftUpPos.x_d, LeftUpPos.y_d - (countDisplayY - 1) * m_TileSide);

        firstAdd = true;
        for (int i = tileStartX; i < tileStartX + countDisplayX; i++)
        {
            for (int j = tileStartY; j < tileStartY + countDisplayY; j++)
            {
                Tile* tile = new Tile(startPos.x_d, startPos.y_d, m_TileSide, CurrentZoom, i, j);
                this->add(tile);
                //Sprite* sp = new Sprite(startPos.x_d, startPos.y_d, 0, m_TileSide, m_TileSide, testTexture);
                //240 mb
                //this->mapLayer->add(sp);

                if (firstAdd) {
                    TileLeftUp = vec2_i(tileStartX_count, tileStartY_count);
                    firstAdd = false;
                }
                startPos.y_d -= m_TileSide;
                if (i == tileStartX + countDisplayX - 1)
                    TileRightDown = vec2_i(tileStartX_count, tileStartY_count);
                tileStartY_count++;
            }
            startPos.y_d = LeftUpPos.y_d;
            startPos.x_d += m_TileSide;
            tileStartY_count = tileStartY;
            tileStartX_count++;
        }


        countWidth = countDisplayX;
        countHeight = countDisplayY;

        if (m_Track != nullptr)
        {
            m_Track->AddPoints(getPosition(),  getBoundingBoxSize().x, CurrentZoom, m_Track->m_Color, vec2_i(tileStartX, tileStartY));

            /*if (m_IsTrackFirstLoad)
            {
                f.open(m_Track->FileNameBinary, std::ios::in | std::ios::binary);
            }
            for (int k = 0; k < m_Track->CountCoords; k++)
            {
                if (k != m_Track->CountCoords - 1) {
                    double lat, lon;
                    if (m_IsTrackFirstLoad)
                    {
                        f.seekg(3 * sizeof(int) + k * (2 * sizeof(double) + m_Track->CountZooms * ((2 * sizeof(int) + m_Track->CountDataString * sizeof(double)))));
                        f.read((char*)&lat, sizeof(lat));
                        f.read((char*)&lon, sizeof(lon));
                        m_Track->points.push_back(lat);
                        m_Track->points.push_back(lon);
                    }
                    m_Track->AddPoint(vec2(m_Track->points[k], m_Track->points[k + 1]), getPosition(), getBoundingBoxSize().x,
                                      CurrentZoom, m_Track->m_Color,
                                      3 * sizeof(int) + k * (2 * sizeof(double) + m_Track->CountZooms * ((2 * sizeof(int) + m_Track->CountDataString * sizeof(double)))),
                                      vec2_i(tileStartX, tileStartY));
                    k++;
                }
            }
            m_IsTrackFirstLoad = false;*/
        }
        else
            std::cout << "track is NULL" << std::endl;
    }

    void Map::add(Tile* tile)
    {
        tile->SetTexture(binaryLoader->GetTileTexture(tile->z, tile->x, tile->y));
        this->mapLayer->add(tile);
    }

    void Map::render()
    {
        mapLayer->render();
        if (m_Track != nullptr) {
            m_Track->render();
        }
        if (pointLayer != nullptr)
            pointLayer->render();
    }

    /*void Map::delete_map()
    {
        for (auto& i : mapLayer->m_Renderables->getArr())
        {
            mapLayer->m_Renderables->remove(i);
            i->DeleteTexture();
            delete i;
        }
        mapLayer->remove_all();
    }
     */

    void Map::delete_points() {
        if (pointLayer != nullptr)
        {
            /*for (auto& i : pointLayer->m_Renderables->getArr())
            {
                delete i;
            }
            pointLayer->m_Renderables->getArr().clear();*/
            for (auto& i : pointLayer->m_Renderables->getArr())
            {
                pointLayer->m_Renderables->remove(i);
                if (i->m_Texture != nullptr)
                    i->m_Texture->deleteTexture();
                delete i;
            }
            pointLayer->m_Renderables->getArr().clear();
            pointLayer->indexCounter = 0;
        }
    }

    void Map::delete_track()
    {
        if (m_Track != nullptr)
        {
            for (auto& i : m_Track->m_Renderables)
            {
                delete i;
            }
            m_Track->m_Renderables.clear();
        }
    }

    void Map::ZoomIn()
    {
        if (CurrentZoom < binaryLoader->MaxZoom)
        {
            //delete_map();
            mapLayer->remove_all();
            delete_track();
            delete_points();
            CurrentZoom++;
            AddTiles();
            if (m_Track != nullptr && m_Track->pointLatLong != nullptr)
                //AddPin(vec2(m_PointLatLong->x, m_PointLatLong->y));
                AddPin(vec2(m_Track->pointLatLong->x, m_Track->pointLatLong->y));
        }
    }

    void Map::ZoomOut()
    {
        if (CurrentZoom > binaryLoader->MinZoom)
        {
            //delete_map();
            mapLayer->remove_all();
            delete_track();
            delete_points();
            CurrentZoom--;
            AddTiles();
            if (m_Track != nullptr && m_Track->pointLatLong != nullptr)
                //AddPin(vec2(m_PointLatLong->x, m_PointLatLong->y));
                AddPin(vec2(m_Track->pointLatLong->x, m_Track->pointLatLong->y));
                //m_Track->AddPoint(m_PointLatLong->x, m_PointLatLong->y, getPosition(), getSize().y, CurrentZoom,
                  //                )
        }
    }

    void Map::MoveRight()
    {
        for (auto& tile : mapLayer->m_Renderables->getArr())
        {
            tile->m_Position.x += speed;
        }
        if (m_Track != nullptr)
        {
            for (auto& point : m_Track->m_Renderables)
            {
                point->m_Position.x += speed;
            }
        }
        if (pointLayer != nullptr)
        {
            for (auto& r : pointLayer->m_Renderables->getArr())
            {
                r->m_Position.x += speed;
            }
        }

        m_Position.x += speed;
    }

    void Map::MoveLeft()
    {
        for (auto& tile : mapLayer->m_Renderables->getArr())
        {
            tile->m_Position.x -= speed;
        }
        if (m_Track != nullptr)
        {
            for (auto& point : m_Track->m_Renderables)
            {
                point->m_Position.x -= speed;
            }
        }
        if (pointLayer != nullptr)
        {
            for (auto& r : pointLayer->m_Renderables->getArr())
            {
                r->m_Position.x -= speed;
            }
        }
        m_Position.x -= speed;
    }

    void Map::MoveUp()
    {
        for (auto& tile : mapLayer->m_Renderables->getArr())
        {
            tile->m_Position.y += speed;
        }
        if (m_Track != nullptr)
        {
            for (auto& point : m_Track->m_Renderables)
            {
                point->m_Position.y += speed;
            }
        }
        if (pointLayer != nullptr)
        {
            for (auto& r : pointLayer->m_Renderables->getArr())
            {
                r->m_Position.y += speed;
            }
        }
        m_Position.y += speed;
    }

    void Map::MoveDown()
    {
        for (auto& tile : mapLayer->m_Renderables->getArr())
        {
            tile->m_Position.y -= speed;
        }
        if (m_Track != nullptr)
        {
            for (auto& point : m_Track->m_Renderables)
            {
                point->m_Position.y -= speed;
            }
        }
        if (pointLayer != nullptr)
        {
            for (auto& r : pointLayer->m_Renderables->getArr())
            {
                r->m_Position.y -= speed;
            }
        }
        m_Position.y -= speed;
    }

    bool Map::CanMoveLeft() {
        return TileRightDown.x_i < binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].xTileStartDownload +
                                   binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].xTileCountDownload - 1;
    }

    bool Map::CanMoveRight() {
        return TileLeftUp.x_i > binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].xTileStartDownload;
    }

    bool Map::CanMoveDown() {
        return TileLeftUp.y_i > binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].yTileStartDownload;
    }

    bool Map::CanMoveUp() {
        return TileRightDown.y_i < binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].yTileStartDownload +
                                   binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].yTileCountDownload - 1;
    }


    Renderable2D *Map::GetTile(int z, int x, int y) {
        for (auto& r : mapLayer->m_Renderables->getArr())
        {
            if (r->z == z && r->x == x && r->y == y)
                return r;
        }
        return nullptr;
    }

    int counterLeft = 0;
    int counterRight = 0;
    int counterDown = 0;
    int counterUp = 0;
    void Map::AddNewTile()
    {
        auto tilesInSideX = binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].xTileCountDisplay;
        auto tilesInSideY = binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].yTileCountDisplay;
        //ADD LEFT
        if (m_Position.x > m_BoundingBoxPosition.x) {
            Tile* tile = new Tile(m_Position.x - m_TileSide, m_Position.y + counterLeft * m_TileSide,
                                  m_TileSide, CurrentZoom, TileLeftUp.x_i - 1, TileLeftUp.y_i + (tilesInSideY - 1 - counterLeft));
            this->add(tile);
            /*Sprite* sp = new Sprite(m_Position.x - m_TileSide, m_Position.y + counterLeft * m_TileSide, 0, m_TileSide, m_TileSide, testTexture);
            this->mapLayer->add(sp);*/

            counterLeft++;
            if (counterLeft == tilesInSideY) {
                TileLeftUp.x_i -= 1;
                m_Position.x -= m_TileSide;
                m_Size.x += m_TileSide;
                counterLeft = 0;
            }
        }
        //ADD RIGHT
        else if (m_Position.x + m_Size.x < m_BoundingBoxPosition.x + m_BoundingBoxSize.x)
        {
            Tile* tile = new Tile(m_Position.x + m_Size.x, m_Position.y + counterRight * m_TileSide, m_TileSide,
                                  CurrentZoom, TileRightDown.x_i + 1, TileRightDown.y_i - counterRight);
            this->add(tile);
            /*Sprite* sp = new Sprite(m_Position.x + m_Size.x, m_Position.y + counterRight * m_TileSide, 0, m_TileSide, m_TileSide, testTexture);
            this->mapLayer->add(sp);*/

            counterRight++;
            if (counterRight == tilesInSideY) {
                TileRightDown.x_i += 1;
                m_Size.x += m_TileSide;
                counterRight = 0;
            }
        }
        //ADD DOWN
        else if (m_Position.y > m_BoundingBoxPosition.y)
        {
            Tile* tile = new Tile(m_Position.x + counterDown * m_TileSide, m_Position.y - m_TileSide,
                                  m_TileSide, CurrentZoom, TileRightDown.x_i - (tilesInSideX - 1 - counterDown), TileRightDown.y_i + 1);
            this->add(tile);
            /*Sprite* sp = new Sprite(m_Position.x + counterDown * m_TileSide, m_Position.y - m_TileSide, 0, m_TileSide, m_TileSide, testTexture);
            this->mapLayer->add(sp);*/

            counterDown++;
            if (counterDown == tilesInSideX) {
                TileRightDown.y_i += 1;
                m_Position.y -= m_TileSide;
                m_Size.y += m_TileSide;
                counterDown = 0;
            }
        }
        //ADD UP
        else if (m_Position.y + m_Size.y < m_BoundingBoxPosition.y + m_BoundingBoxSize.y)
        {
            Tile* tile = new Tile(m_Position.x + counterUp * m_TileSide, m_Position.y + m_Size.y,
                                  m_TileSide, CurrentZoom, TileLeftUp.x_i + counterUp, TileLeftUp.y_i - 1);
            this->add(tile);
            /*Sprite* sp = new Sprite(m_Position.x + counterUp * m_TileSide, m_Position.y + m_Size.y, 0, m_TileSide, m_TileSide, testTexture);
            this->mapLayer->add(sp);*/

            counterUp++;
            if (counterUp == tilesInSideX) {
                TileLeftUp.y_i -= 1;
                m_Size.y += m_TileSide;
                counterUp = 0;
            }
        }
    }

    int kLeft = 0;
    int kRight = 0;
    int kDown = 0;
    int kUp = 0;
    bool Map::IsInBounds(Renderable2D* renderable)
    {
        auto tilesInSideX = binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].xTileCountDisplay;
        auto tilesInSideY = binaryLoader->constants[CurrentZoom - binaryLoader->MinZoom].yTileCountDisplay;

        if (renderable->m_Position.x < m_BoundingBoxPosition.x - m_TileSide)
        {
            kLeft++;
            if (kLeft == tilesInSideY) {
                TileLeftUp.x_i += 1;
                m_Position.x += m_TileSide;
                m_Size.x -= m_TileSide;
                kLeft = 0;
            }
        }
        if (renderable->m_Position.x > m_BoundingBoxPosition.x + m_BoundingBoxSize.x)
        {
            kRight++;
            if (kRight == tilesInSideY) {
                TileRightDown.x_i -= 1;
                m_Size.x -= m_TileSide;
                kRight = 0;
            }
        }
        if (renderable->m_Position.y < m_BoundingBoxPosition.y - m_TileSide)
        {
            kDown++;
            if (kDown == tilesInSideX) {
                TileRightDown.y_i -= 1;
                m_Position.y += m_TileSide;
                m_Size.y -= m_TileSide;
                kDown = 0;
            }
        }
        if (renderable->m_Position.y > m_BoundingBoxPosition.y + m_BoundingBoxSize.y)
        {
            kUp++;
            if (kUp == tilesInSideX) {
                TileLeftUp.y_i += 1;
                m_Size.y -= m_TileSide;
                kUp = 0;
            }
        }

        return renderable->m_Position.x >= m_BoundingBoxPosition.x - m_TileSide &&
               renderable->m_Position.x <= m_BoundingBoxPosition.x + m_BoundingBoxSize.x &&
               renderable->m_Position.y >= m_BoundingBoxPosition.y - m_TileSide &&
               renderable->m_Position.y <= m_BoundingBoxPosition.y + m_BoundingBoxSize.y;
    }

    void Map::DeleteTilesOutOfBounds(Renderable2D* r) {
        if (!IsInBounds(r)) {
            mapLayer->remove(r);
        }
    }

    void Map::TransformTiles() {
        for (auto &r: mapLayer->m_Renderables->getArr()) {
            if (r != nullptr) {
                DeleteTilesOutOfBounds(r);
                AddNewTile();
            }
        }
        //std::cout << "total tiles count on screen: " << mapLayer->m_Renderables->getSize() << std::endl;
    }

    void Map::AddTrack(Track *track)
    {
        m_Track = track;
    }

    void Map::AddPointLayer(MapLayer *layer) {
        pointLayer = layer;
    }

    vec4 Map::GetMapPixelColor(vec2_i screenPixelCoords)
    {
#if 0
        GLubyte color[3];

        std::cout << "screenX = " << screenPixelCoords.x_i << std::endl;
        std::cout << "screenY = " << screenPixelCoords.y_i << std::endl;

        glReadPixels(screenPixelCoords.x_i, m_Window->getHeight() - 1 - screenPixelCoords.y_i, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);

        auto r = (color[0] & 0xFF0000) >> 16;
        auto g = (color[1] & 0x00FF00) >> 8;
        auto b = color[2] & 0x0000FF;

        std::cout << std::endl;
        std::cout << "r = " << r << std::endl;
        std::cout << "g = " << g << std::endl;
        std::cout << "b = " << b << std::endl;

#endif
        unsigned char data[3] = {0};
        int x = 0;
        int y = 0;
        glReadPixels(1, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
        std::cout << "r: " << data[0] / 1.0 << ", g: " << data[1] / 1.0 << ", b: " << data[2] / 1.0
                  << std::endl;
        return vec4(-1, -1, -1, 1);
    }

    vec2_i Map::GetAbsoluteMapPixelCoords(vec2_i screenPixelCoords, int zoom) {
        vec2_i absolutePixelCoords;
        vec2_i mapPosPixels = getPositionPixels();

        int pixelsInFloatX = int(round((float)m_Window->getWidth() / (m_Window->getWidth() / 60.0f)));
        int pixelsInFloatY = int(round((float)m_Window->getHeight() / (m_Window->getHeight() / 60.0f)));

        int mapSidePixelsX = int(round(getSize().x * (float)pixelsInFloatX));
        int mapSidePixelsY = int(round(getSize().y * (float)pixelsInFloatY));

        int myTileSideX = int(round(mapSidePixelsX / binaryLoader->constants[zoom - MinZoom].xTileCountDisplay));
        int myTileSideY = int(round(mapSidePixelsY / binaryLoader->constants[zoom - MinZoom].yTileCountDisplay));

        auto kX = 1024.0f / (float)myTileSideX;
        auto kY = 1024.0f / (float)myTileSideY;

        int tilePixelsSide = 512;

        int mapSizePixels = tilePixelsSide * binaryLoader->constants[zoom - MinZoom].xTileCountDisplay;

        auto a = (float)(screenPixelCoords.x_i - mapPosPixels.x_i);
        auto b = (float)(screenPixelCoords.y_i - mapPosPixels.y_i);
        absolutePixelCoords.x_i = int(round(a * kX)) + TileLeftUp.x_i * tilePixelsSide;
        absolutePixelCoords.y_i = mapSizePixels + int(round(b * kY)) + TileLeftUp.y_i * tilePixelsSide;

        return absolutePixelCoords;
    }

    vec2_i Map::GetAbsolute(vec2 coords) {
        auto inputPixelCoords = app_utils::CoordsConverter::FloatToPixels(coords, m_Window, m_Window->getHeight() / 60.0f, m_Window->getWidth() / 60.0f);
        float floatsInPixel = (m_Window->getWidth() / 60.0f) / (float)m_Window->getWidth();
        auto pixelsInFloat = (float)m_Window->getWidth() / (2 * (m_Window->getWidth() / 60.0f));

        vec2_i ans = vec2_i(-1, -1);
        for (auto& i : mapLayer->m_Renderables->getArr())
        {
            bool con1 = coords.x > i->m_Position.x && coords.x < i->m_Position.x + i->m_Size.x;
            bool con2 = coords.y >i->m_Position.y && coords.y < i->m_Position.y + i->m_Size.y;
            if (con1 && con2) {
                auto pixelTileCoords = app_utils::CoordsConverter::FloatToPixels(i->m_Position, m_Window, m_Window->getHeight() / 60.0f, m_Window->getWidth() / 60.0f);
                int xDif = inputPixelCoords.x_i - pixelTileCoords.x_i;
                int yDif = pixelTileCoords.y_i - inputPixelCoords.y_i;

                int xTileDiff = i->x - TileLeftUp.x_i;
                int yTileDiff = i->y - TileLeftUp.y_i;

                int xAns = int(round(xTileDiff * m_TileSide * pixelsInFloat));
                int yAns = int(round(yTileDiff * m_TileSide * pixelsInFloat));

                xAns += xDif;
                yAns -= yDif;
                ans.x_i = xAns;
                ans.y_i = yAns;
                break;
            }
        }

        return ans;
    }

    void Map::AddPin(vec2 latLon) {
#if 1
        //m_PointLatLong = new vec2(latLon.x, latLon.y);

        int tileSideInPixels = int(round(((float)m_Window->getWidth() * (getSize().x / 4.0f)) / ((float)m_Window->getWidth() / 60.0f)));
        tileSideInPixels = int(round((float)tileSideInPixels / (float)2));
        vec2_i fullMapPixelCoords = app_utils::CoordsConverter::LatLongToTilePixelXY(latLon.x, latLon.y, CurrentZoom, tileSideInPixels);
        float floatsInPixel = 2.0f * ((float)m_Window->getWidth() / 60.0f) / (float)m_Window->getWidth();
        int pixelsInFloat = int(round((float)m_Window->getWidth() / (2.0f * ((float)m_Window->getWidth() / 60.0f))));
        int mapSizePixels = int(round(getSize().x * (float)pixelsInFloat));

        int tileSidePixels = int(round((float)mapSizePixels / (float)(binaryLoader->constants[CurrentZoom - MinZoom].yTileCountDisplay)));

        auto tileCoordsOfPoint = app_utils::CoordsConverter::LatLongToTile(m_Track->pointLatLong->x, m_Track->pointLatLong->y, CurrentZoom);
        float tileSide = getSize().x / (float)(binaryLoader->constants[CurrentZoom - MinZoom].yTileCountDisplay);
        float posX = getPosition().x;
        float posY = getPosition().y;
        int yDif = TileLeftUp.y_i + binaryLoader->constants[CurrentZoom - MinZoom].yTileCountDisplay - 1 - tileCoordsOfPoint.y_i;

        vec2_d chosenTileCoords = vec2_d(posX + double(tileCoordsOfPoint.x_i - TileLeftUp.x_i) * tileSide,
                                     posY + (double)yDif * tileSide + tileSide);
        vec2_i pixelCoordsOnTile = app_utils::CoordsConverter::LatLongToTilePixelXYOnTile(latLon.x, latLon.y, CurrentZoom, tileSidePixels);

        float pointSize = 1.0f;
        pointLayer->add(new Sprite((float)chosenTileCoords.x_d + (float)pixelCoordsOnTile.x_i * floatsInPixel - pointSize / 2.0f,
                                   (float)chosenTileCoords.y_d - (float)pixelCoordsOnTile.y_i * floatsInPixel - pointSize / 2.0f, 1.0f, 1.0f, new Texture("", "../images/circle.bmp")));

        /*for (auto& i : mapLayer->m_Renderables->getArr())
        {
            if (i->x == tileCoordsOfPoint.x_i && i->y == tileCoordsOfPoint.y_i)
            {
                pointLayer->remove_all();
                vec2 pos = vec2(i->m_Position.x + pixelOnTile.x_i * floatsInPixel,
                                i->m_Position.y + i->m_Size.y - pixelOnTile.y_i * floatsInPixel);
                pointLayer->add(new Sprite(pos.x + floatsInPixel * m_Track->m_ScalePointSize - 0.5f, pos.y + floatsInPixel * m_Track->m_ScalePointSize - 0.5f, 1.0f, 1.0f, new Texture("", "../images/circle.bmp")));
                std::cout << "POINT ADDED!" << std::endl;
            }
        }*/
#endif
        //m_Track->AddPoint(latLong, vec2 mapPos, float mapSize, int zoom, unsigned int color, Window *window, size_t pointerSize, vec2_i leftUpTileCoords);
    }
}