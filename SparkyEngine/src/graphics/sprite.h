#pragma once

#include "renderable2d.h"
#include "window.h"

namespace sparky { namespace graphics {
    
    class Sprite : public Renderable2D
    {
    protected:
        Sprite();
    public:
        Sprite(Texture* texture);
        Sprite(float x, float y, Texture* texture);
        Sprite(float x, float y, float width, float height, unsigned int color);
        Sprite(float x, float y, float width, float height, unsigned int color, Window* window);
        Sprite(float x, float y, float width, float height, const maths::vec4& color);
        Sprite(float x, float y, float width, float height, Texture* texture);
        Sprite(double x, double y, double width, double height, Texture* texture);

        Sprite(float x, float y, float z, Texture* texture);
        Sprite(float x, float y, float z, float width, float height, unsigned int color);
        Sprite(float x, float y, float z, float width, float height, const maths::vec4& color);
        Sprite(float x, float y, float z, float width, float height, Texture* texture);
        Sprite(std::vector<float> vertices, unsigned int color);
        Sprite(double x, double y, double width, double height, const maths::vec4& color);

        /*By pixel coord*/
        Sprite(int pixelX, int pixelY, float width, float height, unsigned int color,
               Window* window,  float cameraVertical, float cameraHorizontal);
        Sprite(int pixelX, int pixelY, float width, float height, unsigned int color, std::string fileNumber, size_t pointerFile,
               Window* window,  float cameraVertical, float cameraHorizontal);
        Sprite(int pixelX, int pixelY, float width, float height, unsigned int color, size_t pointerFile,
               Window* window,  float cameraVertical, float cameraHorizontal);
        Sprite(double pixelX, double pixelY, float width, float height, Texture* texture,
               Window* window, float cameraVertical, float cameraHorizontal);
        Sprite(double pixelX, double pixelY, float width, float height, const maths::vec4& color);

        void SetUV(const std::vector<maths::vec2>& uv);

        inline void SetTexture(Texture* texture) { m_Texture = texture; }
    };

} }