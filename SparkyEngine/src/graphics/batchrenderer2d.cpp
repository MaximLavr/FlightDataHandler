#include <cmath>
#include "batchrenderer2d.h"
#include "../utils/colorTransformer.h"

namespace sparky { namespace graphics {

    BatchRenderer2D::BatchRenderer2D(Window* window)
        : m_IndexCount(0)
    {
        m_Window = window;
        init();
    }

    BatchRenderer2D::~BatchRenderer2D()
    {
        delete m_IBO;
        glDeleteBuffers(1, &m_VBO);
    }

    void BatchRenderer2D::init()
    {
        //glViewport(0, 0, m_Window->getWidth(), m_Window->getHeight());
        //glViewport(0, 0, m_Window->m_StartWidth,  m_Window->m_StartHeight);
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
        glEnableVertexAttribArray(SHADER_UV_INDEX);
        glEnableVertexAttribArray(SHADER_TID_INDEX);
        glEnableVertexAttribArray(SHADER_COLOR_INDEX);

        glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
        glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, uv)));
        glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, tid)));
        glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, color)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        GLuint* indices = new GLuint[RENDERER_INDICES_SIZE];

        int offset = 0;
        for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
        {
            indices[  i  ] = offset + 0;
            indices[i + 1] = offset + 1;
            indices[i + 2] = offset + 2;

            indices[i + 3] = offset + 2;
            indices[i + 4] = offset + 3;
            indices[i + 5] = offset + 0;

            offset += 4;
        }

        m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

        glBindVertexArray(0);
    }

    void BatchRenderer2D::begin()
    {
        //glViewport(0, 0, m_Window->getWidth(), m_Window->getHeight());
        //glViewport(0, 0, m_Window->m_StartWidth,  m_Window->m_StartHeight);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    }

    void BatchRenderer2D::drawShadow(Renderable2D* renderable)
    {
        const auto& vertices = renderable->getVertices();
        const unsigned int color = renderable->getColor();
        const std::vector<maths::vec2>& uv = renderable->getUV();

        m_Buffer->vertex = *m_TransformationBack * maths::vec2(vertices[0], vertices[1]);
        m_Buffer->uv = uv[0];
        m_Buffer->color = color;
        m_Buffer++;

        m_Buffer->vertex = *m_TransformationBack * maths::vec2(vertices[2], vertices[3]);
        m_Buffer->uv = uv[1];
        m_Buffer->color = color;
        m_Buffer++;

        m_Buffer->vertex = *m_TransformationBack * maths::vec2(vertices[4], vertices[5]);
        m_Buffer->uv = uv[2];
        m_Buffer->color = color;
        m_Buffer++;

        m_Buffer->vertex = *m_TransformationBack * maths::vec2(vertices[6], vertices[7]);
        m_Buffer->uv = uv[3];
        m_Buffer->color = color;
        m_Buffer++;

        m_IndexCount += 6;
    }

        void BatchRenderer2D::submit(const Renderable2D* renderable)
        {
            //glViewport(0, 0, m_Window->getWidth(), m_Window->getHeight());
            //glViewport(0, 0, m_Window->m_StartWidth,  m_Window->m_StartHeight);

            const maths::vec3& position = renderable->getPosition();
            const maths::vec2& size = renderable->getSize();
            const unsigned int color = renderable->getColor();
            const std::vector<maths::vec2>& uv = renderable->getUV();
            const GLuint tid = renderable->getTID();

                float ts = 0.0f;
                if (tid > 0)
                {
                    bool found = false;
                    for (int i = 0; i < m_TextureSlots.size(); i++)
                    {
                        if (m_TextureSlots[i] == tid)
                        {
                            ts = (float)(i + 1);
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        if (m_TextureSlots.size() >= 16)
                        {
                            end();
                            flush();
                            begin();
                        }
                        m_TextureSlots.push_back(tid);
                        ts = (float)(m_TextureSlots.size());
                    }
                }

                m_Buffer->vertex = *m_TransformationBack * position;
                m_Buffer->uv = uv[0];
                m_Buffer->tid = ts;
                m_Buffer->color = color;
                m_Buffer++;

                m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x, position.y + size .y, position.z);
                m_Buffer->uv = uv[1];
                m_Buffer->tid = ts;
                m_Buffer->color = color;
                m_Buffer++;

                m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y + size .y, position.z);
                m_Buffer->uv = uv[2];
                m_Buffer->tid = ts;
                m_Buffer->color = color;
                m_Buffer++;

                m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y, position.z);
                m_Buffer->uv = uv[3];
                m_Buffer->tid = ts;
                m_Buffer->color = color;
                m_Buffer++;

                m_IndexCount += 6;
        }

    void BatchRenderer2D::DrawLine(float x0, float y0, float x1, float y1, unsigned int color, float thickness)
    {
        const std::vector<maths::vec2>& uv = Renderable2D::getDefaultUVs();
        float ts = 0.0f;
        maths::mat4 maskTransform = maths::mat4::identity();
        //float mid = m_Mask ? SubmitTexture(m_Mask->texture) : 0.0f;

        float ms = 0.0f;

        maths::vec2 normal = maths::vec2(y1 - y0, -(x1 - x0)).Normalise() * thickness;

        maths::vec3 vertex = *m_TransformationBack * maths::vec3(x0 + normal.x, y0 + normal.y, 0.0f);
        m_Buffer->vertex = vertex;
        m_Buffer->uv = uv[0];
            //m_Buffer->mask_uv = maskTransform * vertex;
            m_Buffer->tid = ts;
            //m_Buffer->mid = ms;
            m_Buffer->color = color;
            m_Buffer++;

            vertex = *m_TransformationBack * maths::vec3(x1 + normal.x, y1 + normal.y, 0.0f);
            m_Buffer->vertex = vertex;
            m_Buffer->uv = uv[1];
            //m_Buffer->mask_uv = maskTransform * vertex;
            m_Buffer->tid = ts;
            //m_Buffer->mid = ms;
            m_Buffer->color = color;
            m_Buffer++;

            vertex = *m_TransformationBack * maths::vec3(x1 - normal.x, y1 - normal.y, 0.0f);
            m_Buffer->vertex = vertex;
            m_Buffer->uv = uv[2];
            //m_Buffer->mask_uv = maskTransform * vertex;
            m_Buffer->tid = ts;
            //m_Buffer->mid = ms;
            m_Buffer->color = color;
            m_Buffer++;

            vertex = *m_TransformationBack * maths::vec3(x0 - normal.x, y0 - normal.y, 0.0f);
            m_Buffer->vertex = vertex;
            m_Buffer->uv = uv[3];
            //m_Buffer->mask_uv = maskTransform * vertex;
            m_Buffer->tid = ts;
            //m_Buffer->mid = ms;
            m_Buffer->color = color;
            m_Buffer++;

            m_IndexCount += 6;
        }



    void BatchRenderer2D::DrawRect(float x, float y, float width, float height, unsigned int color)
    {
        DrawLine(x, y, x + width, y, color);
        DrawLine(x + width, y, x + width, y + height, color);
        DrawLine(x + width, y + height, x, y + height, color);
        DrawLine(x, y + height, x, y, color);
    }

    void BatchRenderer2D::DrawLine(const maths::vec2& start, const maths::vec2& end, unsigned int color, float thickness)
    {
        DrawLine(start.x, start.y, end.x, end.y, color, thickness);
    }

    void BatchRenderer2D::DrawRect(const maths::vec2& position, const maths::vec2& size, unsigned int color)
    {
        DrawRect(position.x, position.y, size.x, size.y, color);
    }

    void BatchRenderer2D::DrawRect(const maths::Rectangle& rectangle, unsigned int color)
    {
        DrawRect(rectangle.GetMinimumBound(), rectangle.size * 2.0f, color);
    }

    void BatchRenderer2D::drawFigure(std::vector<float> vertices, unsigned int color)
    {
        int k = 0;
        for (int i = 0; i < vertices.size() / 2 - 1; i+=1)
        {
            DrawLine(vertices[k], vertices[k + 1], vertices[k + 2], vertices[k + 3], color);
            k += 2;
        }
        DrawLine(vertices[k], vertices[k + 1], vertices[0], vertices[1], color);
    }

    void BatchRenderer2D::drawString(const std::string& text, const maths::vec3& position,
                                     const Font& font, unsigned int color)
    {
        using namespace ftgl;

        float ts = 0.0f;
        bool found = false;
        for (int i = 0; i < m_TextureSlots.size(); i++)
        {
            if (m_TextureSlots[i] == font.getID())
            {
                ts = (float)(i + 1);
                found = true;
                break;
            }
        }

        if (!found)
        {
            if (m_TextureSlots.size() >= RENDERER_MAX_TEXTURES)
            {
                end();
                flush();
                begin();
            }
            m_TextureSlots.push_back(font.getID());
            ts = (float)(m_TextureSlots.size());
        }

        const maths::vec2& scale = font.getScale();

        float x = position.x;

        texture_font_t* ftFont = font.getFTFont();


        for (int i = 0; i < text.length(); i++)
        {
            char c = text[i];
            texture_glyph_t* glyph = texture_font_get_glyph(ftFont, c);
            if (glyph != NULL)
            {
                if (i > 0)
                {
                    float kerning = texture_glyph_get_kerning(glyph, text[i - 1]);
                    x += kerning / scale.x;
                }

                float x0 = x + glyph->offset_x / scale.x;
                float y0 = position.y + glyph->offset_y / scale.y;
                float x1 = x0 + glyph->width / scale.y;
                float y1 = y0 - glyph->height / scale.y;

                float u0 = glyph->s0;
                float v0 = glyph->t0;
                float u1 = glyph->s1;
                float v1 = glyph->t1;

                m_Buffer->vertex = *m_TransformationBack * maths::vec3(x0, y0, 0);
                m_Buffer->uv = maths::vec2(u0, v0);
                m_Buffer->tid = ts;
                m_Buffer->color = color;
                m_Buffer++;

                m_Buffer->vertex = *m_TransformationBack * maths::vec3(x0, y1, 0);
                m_Buffer->uv = maths::vec2(u0, v1);
                m_Buffer->tid = ts;
                m_Buffer->color = color;
                m_Buffer++;


                m_Buffer->vertex = *m_TransformationBack * maths::vec3(x1, y1, 0);
                m_Buffer->uv = maths::vec2(u1, v1);
                m_Buffer->tid = ts;
                m_Buffer->color = color;
                m_Buffer++;

                m_Buffer->vertex = *m_TransformationBack * maths::vec3(x1, y0, 0);
                m_Buffer->uv = maths::vec2(u1, v0);
                m_Buffer->tid = ts;
                m_Buffer->color = color;
                m_Buffer++;

                m_IndexCount += 6;

                x += glyph->advance_x / scale.x;
            }
        }
    }

    void BatchRenderer2D::end()
    {
        //glViewport(0, 0, m_Window->getWidth(), m_Window->getHeight());
        //glViewport(0, 0, m_Window->m_StartWidth,  m_Window->m_StartHeight);
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    void BatchRenderer2D::flush()
    {
        //glViewport(0, 0, m_Window->getWidth(), m_Window->getHeight());
        //glViewport(0, 0, m_Window->m_StartWidth,  m_Window->m_StartHeight);
        for (int i = 0; i < m_TextureSlots.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
        }

        glBindVertexArray(m_VAO);
        m_IBO->bind();

        glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, NULL);

        m_IBO->unbind();
        glBindVertexArray(0);

        m_IndexCount = 0;
        m_TextureSlots.clear();
    } 

} }