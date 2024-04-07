#pragma once
//#include "Map_part.h"
//#include "Map_loader.h"
#include <vector>
//#include "../Utils/CoordsConverter.h"
#include "../DataLoader/BinaryLoader.h"
#include "../Track/Track.h"
#include "MapLayer.h"

using namespace sparky;
using namespace graphics;
using namespace app_loader;

namespace app_graphics
{
    enum class Side
    {
        Right,
        Left,
        Up,
        Down
    };

    class Map
    {
    private:
        Shader* m_Shader;
        vec2 m_Size;
        mat4 m_ProjectionMatrix;

        vec2_d m_ZoomLatLongCoords;

        vec2 m_PointedTilePos;
        vec2 m_PixelPointedPos;
        vec2 m_BeginPos;
        vec2 m_BoundingBoxSize;

    public:
        enum class MapType
        {
            Elevation,
            Satellite
        };
        ~Map()
        {
            delete m_Shader;
            delete pointLayer;
            delete mapLayer;
            //delete m_PointLatLong;
            delete m_Track;
            textures.clear();
            trackPoints.clear();
        };
        const int MIN_ZOOM = 2;
        float speed = 3.0f; //default: 0.4f
        double m_TileSide;
        BinaryLoader* binaryLoader;
        Window* m_Window;
        Track* m_Track = nullptr;
        bool isTrackAdded;
        MapLayer* pointLayer = nullptr;
        int PixelsInFloat;
        vec2 m_Position;
        vec2 m_BoundingBoxPosition;
        bool isPointAdded;
        MapType m_MapType;

        Texture* tx1 = nullptr;
        Texture* tx2 = nullptr;
        Texture* tx3 = nullptr;
        std::vector<Texture*> textures;

        Map(float x, float y, float side, MapType mapType, vec2_d zoomLatLongCoords, double widthMeters, double heightMeters,
            Shader* shader, mat4 ortho, Window* window, const char* folderPath = "");

        void LoadData(int minZoom, int maxZoom);
        void AddTiles();
        void render();
        void AddPointLayer(MapLayer* layer);
        int MinZoom;
        int MaxZoom;

        void AddTrack(Track* track);

        void add(Tile* tile);

        void AddTilesAround(int zoom);
        void ZoomIn();
        void ZoomOut();
        //void delete_map();
        void delete_track();
        void delete_points();

        vec2 getSize() { return m_Size; }
        vec2 getPosition() { return m_Position; }
        vec2_i getPositionPixels() { return app_utils::CoordsConverter::FloatToPixels(getPosition(), m_Window,
                                                                                      m_Window->getHeight() / 60.0f, m_Window->getWidth() / 60.0f); }
        vec2 getBoundingBoxPosition() { return m_BoundingBoxPosition; }
        vec2_i getBoundingBoxPositionPixels() { return app_utils::CoordsConverter::FloatToPixels(getBoundingBoxPosition(), m_Window,
                                                                                                 m_Window->getHeight() / 60.0f, m_Window->getWidth() / 60.0f); }
        vec2 getBoundingBoxSize() { return m_BoundingBoxSize; }
        double getTileSide() { return m_TileSide; }

        void MoveRight();
        void MoveLeft();
        void MoveDown();
        void MoveUp();

        vec2_d m_CenterTilePosition;
        vec2_i m_CenterTileCoords;
        vec2_d CenterLatLongCoords;
        int CurrentZoom;
        MapLayer* mapLayer;
        double WidthMeters;
        double HeightMeters;

        static struct CountTilesToDisplay getCountTilesToDisplay() { return CountTilesToDisplay; }


        std::string FileName;

        bool isLoaded() { return binaryLoader->IsLoaded; }

        bool flag;
        vec2_i const_left_up_tile;

        int countSide;
        int counter = 1;

        bool beforeMoveRight = false;
        bool beforeMoveLeft = false;

        int counterDeleted = 0;
        int counterDeleted2 = 0;

        bool wasDeleteLeft = false;
        bool wasDeleteRight = false;

        bool flag2 = false;

        vec2_i TileLeftUp;
        vec2_i TileRightDown;

        bool firstAdd = true;


        bool CanMoveLeft();
        bool CanMoveRight();
        bool CanMoveUp();
        bool CanMoveDown();

        bool right;
        bool left;

        int TILES_ON_SCREEN = 0;

        int tilesCountWidth;
        int tilesCountHeight;

        bool deletedRight;
        bool deletedLeft;
        bool deletedUp;
        bool deletedDown;

        int deletedRightCount;
        int deletedLeftCount;
        int deletedUpCount;
        int deletedDownCount;

        int countWidth;
        int countHeight;

        void TransformTiles();
        void DeleteTilesOutOfBounds(Renderable2D* r);
        void AddNewTile();
        bool IsInBounds(Renderable2D* renderable);

        Renderable2D* GetTile(int z, int x, int y);

        //MapLayer* trackLayer;
        float m_CameraVertical;
        float m_CameraHorizontal;

        std::vector<double> trackPoints;
        void DrawTrack(vec2 latLong, unsigned int color, MapLayer* layer, Window* window,
                       float cameraVertical, float cameraHorizontal, vec2_i leftUpTileCoords);

        void DrawTrackFinal(unsigned int color, vec2_i leftUpTileCoords);

        vec2_i GetAbsoluteMapPixelCoords(vec2_i screenPixelCoords, int zoom);
        vec4 GetMapPixelColor(vec2_i screenPixelCoords);

        vec2_i GetAbsolute(vec2 coords);

        Renderable2D* pointedTrackItem;

        void AddPin(vec2 latLon);
        //vec2* m_PointLatLong = nullptr;

        bool isHovered();

        bool m_IsTrackFirstLoad = true;
        std::ifstream f;
        std::string m_FolderName;

        inline const char* EnumToString(MapType mapType)
        {
            switch (mapType)
            {
                case MapType::Elevation:   return "Elevation";
                case MapType::Satellite:   return "Satellite";
                default:      return "Satellite";
            }
        }
    };
}

