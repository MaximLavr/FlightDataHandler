#include "sprite.h"

#include <utility>

namespace sparky { namespace graphics {

        Sprite::Sprite()
                : Renderable2D()
        {
        }

        Sprite::Sprite(Texture* texture)
            : Renderable2D(maths::vec3(0.0f, 0.0f, 0.0f), maths::vec2((float)texture->getWidth(), (float)texture->getHeight()), 0xffffffff)
        {
            m_Texture = texture;
        }

        Sprite::Sprite(float x, float y, Texture* texture)
            : Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2((float)texture->getWidth(), (float)texture->getHeight()), 0xffffffff)
        {
            m_Texture = texture;
        }

        Sprite::Sprite(float x, float y, float width, float height, unsigned int color)
            : Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), color)
        {
        }
        Sprite::Sprite(float x, float y, float width, float height, unsigned int color, Window* window)
                : Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), color, window)
        {
        }

        /*!*/
        Sprite::Sprite(int pixelX, int pixelY, float width, float height, unsigned int color,
                       Window* window, float cameraVertical, float cameraHorizontal)
            : Renderable2D(maths::vec3(2.0f * pixelX * abs(cameraHorizontal) / window->getWidth() - abs(cameraHorizontal),
                                       abs(cameraVertical) - 2.0f * pixelY * abs(cameraVertical) / window->getHeight(), 0.0f),
                                    maths::vec2(width, height), color)
        {
            PixelCoords = vec2_i(pixelX, pixelY);
        }


        Sprite::Sprite(int pixelX, int pixelY, float width, float height, unsigned int color, std::string filePath, size_t pointerFile,
                       Window* window, float cameraVertical, float cameraHorizontal)
                : Renderable2D(maths::vec3(2.0f * pixelX * abs(cameraHorizontal) / window->getWidth() - abs(cameraHorizontal),
                                           abs(cameraVertical) - 2.0f * pixelY * abs(cameraVertical) / window->getHeight(), 0.0f),
                               maths::vec2(width, height), color)
        {
            PixelCoords = vec2_i(pixelX, pixelY);
            PointerFile = pointerFile;
            FilePath = filePath;
        }
        Sprite::Sprite(int pixelX, int pixelY, float width, float height, unsigned int color, size_t pointerFile,
                       Window* window, float cameraVertical, float cameraHorizontal)
                : Renderable2D(maths::vec3(2.0f * pixelX * abs(cameraHorizontal) / window->getWidth() - abs(cameraHorizontal),
                                           abs(cameraVertical) - 2.0f * pixelY * abs(cameraVertical) / window->getHeight(), 0.0f),
                               maths::vec2(width, height), color)
        {
            PixelCoords = vec2_i(pixelX, pixelY);
            PointerFile = pointerFile;
        }
        /*!*/

        Sprite::Sprite(double pixelX, double pixelY, float width, float height, Texture* texture,
                       Window* window, float cameraVertical, float cameraHorizontal)
                : Renderable2D(maths::vec3(2.0f*(pixelX * abs(cameraHorizontal) / window->getWidth() * 2 - abs(cameraHorizontal) / 2.0f),
                                           abs(cameraVertical) -pixelY * 2 * abs(cameraVertical) / window->getHeight() * 2.0f, 0.0f),
                               maths::vec2(width, height), 0xffffffff)
        {
            m_Window = window;
            m_Texture = texture;
            PixelCoords = vec2_i(pixelX, pixelY);
        }

        Sprite::Sprite(float x, float y, float width, float height, const maths::vec4& color)
            : Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), 0xffffffff)
        {
            setColor(color);
        }

        Sprite::Sprite(float x, float y, float width, float height, Texture* texture)
            : Renderable2D(maths::vec3(x, y, -0.0f), maths::vec2(width, height), 0xffffffff)
        {
            m_Texture = texture;
        }

        Sprite::Sprite(double x, double y, double width, double height, Texture *texture)
            : Renderable2D(maths::vec3(x, y, -0.0f), maths::vec2(width, height), 0xffffffff)
        {
            m_Texture = texture;
        }

        Sprite::Sprite(double x, double y, double width, double height, const maths::vec4& color)
                : Renderable2D(maths::vec3(x, y, -0.0f), maths::vec2(width, height), 0xffffffff)
        {
            setColor(color);
        }

        Sprite::Sprite(float x, float y, float z, Texture* texture)
                : Renderable2D(maths::vec3(x, y, z), maths::vec2((float)texture->getWidth(), (float)texture->getHeight()), 0xffffffff)
        {
            m_Texture = texture;
        }

        Sprite::Sprite(float x, float y, float z, float width, float height, unsigned int color)
                : Renderable2D(maths::vec3(x, y, z), maths::vec2(width, height), color)
        {
        }

        Sprite::Sprite(float x, float y, float width, float z, float height, const maths::vec4& color)
                : Renderable2D(maths::vec3(x, y, z), maths::vec2(width, height), 0xffffffff)
        {
            setColor(color);
        }

        Sprite::Sprite(float x, float y, float z, float width, float height, Texture* texture)
                : Renderable2D(maths::vec3(x, y, z), maths::vec2(width, height), 0xffffffff)
        {
            m_Texture = texture;
        }

        void Sprite::SetUV(const std::vector<maths::vec2>& uv)
        {
            m_UV = uv;
        }

    } }