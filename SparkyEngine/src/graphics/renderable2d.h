#pragma once

#include <utility>

#include "buffers/buffer.h"
#include "buffers/indexbuffer.h"
#include "buffers/vertexarray.h"

#include "renderer2d.h"

#include "../maths/maths.h"
#include "shader.h"
#include "texture.h"
#include "window.h"

namespace sparky { namespace graphics {

    struct VertexData
    {
        maths::vec3 vertex;
        maths::vec2 uv;
        float tid;
        unsigned int color;
    };

    class Renderable2D
    {
    protected:
        unsigned int m_Color;
        maths::vec4 m_ColorVector;
        std::vector<maths::vec2> m_UV;
        maths::Rectangle m_Bounds;
        std::vector<float> m_Vertices;
    protected:
        Renderable2D() { setUVDefaults(); }
    public:
        int x, y, z; // for tiles
        Window* m_Window;
        vec2_i PixelCoords;
        Texture* m_Texture;
        bool isRemoved = false;
        size_t PointerFile;
        std::string FilePath;

        std::string Text;

        struct TSide
        {
            bool Left;
            bool Right;
            bool Up;
            bool Bottom;
        } TSide;

        //Renderable2D* neighborLeft;
        //Renderable2D* neighborRight;
        //Renderable2D* neighborUp;
        //Renderable2D* neighborBottom;

        bool isLast;

        int index;
        maths::vec3 m_Position;
        maths::vec2 m_Size;
        maths::vec2_i m_SizePixels;

        /*Line*/
        /*Line*/

        Renderable2D(maths::vec3 position, maths::vec2 size, unsigned int color, Window* window)
            :m_Bounds(position, size), m_Position(position), m_Size(size),  m_Color(color), m_Texture(nullptr), m_Window(window)
        { setUVDefaults(); }

        Renderable2D(maths::vec3 position, maths::vec2 size, unsigned int color)
            : m_Bounds(position, size), m_Position(position), m_Size(size),  m_Color(color), m_Texture(nullptr)
        { setUVDefaults(); }

        Renderable2D(maths::vec3 position, maths::vec2 size, Texture* texture)
                : m_Bounds(position, size), m_Position(position), m_Size(size),  m_Texture(texture)
        { setUVDefaults(); }

        Renderable2D(maths::vec2 position, maths::vec2 size, Texture* texture)
                : m_Bounds(position, size), m_Position(position.x, position.y, 0), m_Size(size),  m_Texture(texture)
        { setUVDefaults(); }

        Renderable2D(std::vector<float> vertices, unsigned int color)
                : m_Vertices(std::move(vertices)),  m_Color(color)
        { setUVDefaults(); }

        Renderable2D(std::vector<float> vertices, Texture* texture)
                : m_Vertices(std::move(vertices)),  m_Texture(texture)
        { setUVDefaults(); }

        Renderable2D(maths::vec3 position, maths::vec2 size, maths::vec4 color)
                : m_Bounds(position, size), m_Position(position), m_Size(size), m_ColorVector(color), m_Texture(nullptr)
        { setUVDefaults(); }

        virtual ~Renderable2D() {
            //delete m_Texture;
        }

        inline const maths::Rectangle& GetBounds() const { return m_Bounds; }
        inline maths::Rectangle& GetBounds() { return m_Bounds; }

        inline maths::AABB GetBoundingBox() const { return m_Bounds; }

        virtual void submit(Renderer2D* renderer) const
        {
            renderer->submit(this);
        }

        inline const maths::vec3& getPosition() const { return m_Position; }
        inline const std::vector<float>& getVertices() const { return m_Vertices; }
        inline const maths::vec2& getSize() const { return m_Size; }
        inline const unsigned int getColor() const { return m_Color; }
        inline const std::vector<maths::vec2>& getUV() const { return m_UV; }

        //inline const GLunsigned int getTID() const { return m_Texture ? m_Texture->getID() : 0; }
        inline const GLuint getTID() const { return m_Texture == nullptr ? 0: m_Texture->getID(); }
        void setColor(unsigned int color) { m_Color = color; }
        void setColor(const maths::vec4& color)
        {
            int r = color.x * 255.0f;
            int g = color.y * 255.0f;
            int b = color.z * 255.0f;
            int a = color.w * 255.0f;

            m_Color = a << 24 | b << 16 | g << 8 | r;
        }

        bool isHovered() const {
#ifdef __APPLE__
            float x = m_Window->getMousePosition().x / (m_Window->getWidth() / m_Window->cameraRight / 2.0f) - m_Window->cameraRight;
            float y = -(m_Window->getMousePosition().y / (m_Window->getHeight() / m_Window->cameraUp / 2.0f) - m_Window->cameraUp);
#else
            float x = (m_Window->getMousePosition().x - m_Window->getWidth() / 2.0f) / 30.0f;
    float y = -(m_Window->getMousePosition().y - m_Window->getHeight() / 2.0f) / 30.0f;
#endif
            if ((x >= this->getPosition().x) && (x <= this->getPosition().x + this->getSize().x) &&
                (y >= this->getPosition().y) && (y <= this->getPosition().y + this->getSize().y)) {
                return true;
            }
            return false;
        }

        inline void SetPosition(const maths::vec2& position) { m_Bounds.position = position; }
        inline void SetSize(const maths::vec2& size) { m_Bounds.size = size; }
        inline void SetTexture(Texture* texture) { m_Texture = texture; }
        //inline void DeleteTexture() { delete m_Texture; }
        //inline void DeleteTexture() { m_Texture->deleteTexture(); }

        static std::vector<maths::vec2> getDefaultUVs()
        {
            static std::vector<maths::vec2> results;
            if (!results.size())
            {
                results.push_back(maths::vec2(0, 1));
                results.push_back(maths::vec2(1, 1));
                results.push_back(maths::vec2(1, 0));
                results.push_back(maths::vec2(0, 0));
            }
            return results;
        }


    private:
        void setUVDefaults()
        {
            m_UV.push_back(maths::vec2(0, 0));
            m_UV.push_back(maths::vec2(0, 1.0));
            m_UV.push_back(maths::vec2(1.0, 1.0));
            m_UV.push_back(maths::vec2(1.0, 0));
        }
    };

} }
