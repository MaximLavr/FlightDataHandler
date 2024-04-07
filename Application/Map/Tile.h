#pragma once
#include <fstream>
#include <iostream>
#include "../../SparkyEngine/src/engine.h"
#include "../../SparkyEngine/src/graphics/window.h"
#include "../../SparkyEngine/src/graphics/layers/layer.h"
#include "../Utils/CoordsConverter.h"

using namespace sparky;
using namespace maths;
using namespace graphics;

namespace app_graphics
{
    class Tile : public sparky::graphics::Renderable2D {
    private:
    public:
        Tile()
            : m_Position(0.0, 0.0), size(0), startPoint(0) { }
        int size;
        unsigned long startPoint;
        bool isHereTrack;
        bool isColorsSaved;

        Texture* m_Texture = new Texture("", "");
        sparky::maths::vec3 m_Position;
        sparky::maths::vec2 m_Size;

        Tile(float xPos, float yPos, float side, int zoom, int x, int y);

        void DrawPixelOn(vec2 latLong, int zoom, unsigned int color, Layer* layer, Window* window,
                     float cameraVertical, float cameraHorizontal, vec2 mapPos, vec2_i leftUpTileCoords);

        /*virtual void submit(sparky::graphics::Renderer2D* renderer) const
        {
            renderer->submit(this);
        }*/

        const sparky::maths::vec3& getPosition() const { return m_Position; }

        ~Tile() override {
            delete m_Texture;
        }
    };

    void AddNewEthalons(double *impulses, const char *path);
}
