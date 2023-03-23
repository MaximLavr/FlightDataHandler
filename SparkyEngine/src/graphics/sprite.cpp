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

        Sprite::Sprite(float x, float y, float width, float height, uint color)
            : Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(width, height), color)
        {
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

        Sprite::Sprite(float x, float y, float z, Texture* texture)
                : Renderable2D(maths::vec3(x, y, z), maths::vec2((float)texture->getWidth(), (float)texture->getHeight()), 0xffffffff)
        {
            m_Texture = texture;
        }

        Sprite::Sprite(float x, float y, float z, float width, float height, uint color)
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