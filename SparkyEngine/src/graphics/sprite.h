#pragma once

#include "renderable2d.h"

namespace sparky { namespace graphics {
    
    class Sprite : public Renderable2D
    {
    protected:
        Sprite();
    public:
        Sprite(Texture* texture);
        Sprite(float x, float y, Texture* texture);
        Sprite(float x, float y, float width, float height, uint color);
        Sprite(float x, float y, float width, float height, const maths::vec4& color);
        Sprite(float x, float y, float width, float height, Texture* texture);

        Sprite(float x, float y, float z, Texture* texture);
        Sprite(float x, float y, float z, float width, float height, uint color);
        Sprite(float x, float y, float z, float width, float height, const maths::vec4& color);
        Sprite(float x, float y, float z, float width, float height, Texture* texture);
        Sprite(std::vector<float> vertices, uint color);

        void SetUV(const std::vector<maths::vec2>& uv);

        inline void SetTexture(Texture* texture) { m_Texture = texture; }
    };

} }