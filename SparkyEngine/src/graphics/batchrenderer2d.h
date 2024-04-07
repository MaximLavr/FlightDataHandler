#pragma once

#include "renderer2d.h"
#include "renderable2d.h"
#include "buffers/indexbuffer.h"
#include <cstddef>
#include "../../../Application/Map/Tile.h"


namespace sparky { namespace graphics {

#define RENDERER_MAX_SPRITES    60000
#define RENDERER_VERTEX_SIZE    sizeof(VertexData)
#define RENDERER_SPRITE_SIZE    RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE    RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE   RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES   64

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX 1
#define SHADER_TID_INDEX 2
#define SHADER_COLOR_INDEX 3

    class BatchRenderer2D : public Renderer2D
    {
    private:
        GLuint m_VAO;
        GLuint m_VBO;
        IndexBuffer* m_IBO;
        GLsizei m_IndexCount;
        VertexData* m_Buffer;

        std::vector<GLuint> m_TextureSlots;
        Window* m_Window;
    public:
        BatchRenderer2D(Window* window);
        ~BatchRenderer2D();
        void begin() override;
        void submit(const Renderable2D* renderable) override;
        Window* getWindow() { return m_Window; }

        void DrawRect(float x, float y, float width, float height, unsigned int color = 0xffffffff) override;
        void DrawRect(const maths::vec2& position, const maths::vec2& size, unsigned int color = 0xffffffff) override;
        void DrawRect(const maths::Rectangle& rectangle, unsigned int color = 0xffffffff) override;
        void DrawLine(float x0, float y0, float x1, float y1, unsigned int color = 0xffffffff, float thickness = 0.02f);
        void DrawLine(const maths::vec2& start, const maths::vec2& end, unsigned int color = 0xffffffff, float thickness = 0.02f);
        void drawFigure(std::vector<float> vertices, unsigned int color);
        void drawShadow(Renderable2D* renderable) override;

        void drawString(const std::string& text, const maths::vec3& position,
                        const Font& font, unsigned int color) override;
        void end() override;
        void flush() override;
    private:
        void init();
    };
} }