#include "texture.h"

#define MIP_MAP 0

namespace sparky { namespace graphics {

        Texture::Texture(const std::string& name, const std::string& filename)
                : m_Name(name), m_FileName(filename)
        {
            m_TID = load();
        }

        Texture::Texture(char* array)
        {
            m_CharArray = array;
            m_TID = loadFromByteArray();
        }

        Texture::~Texture()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &m_TID);
        }

        GLuint Texture::loadFromByteArray()
        {
            GLuint result;
            glGenTextures(1, &result);
            glBindTexture(GL_TEXTURE_2D, result);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0,32 == 32 ? GL_RGBA : GL_RGB, 512, 512,
                         0, 32 == 32 ? GL_BGRA: GL_BGR, GL_UNSIGNED_BYTE, m_CharArray);
            //glTexSubImage2D(GL_TEXTURE_2D, 0, 0,0, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, m_CharArray);
            glBindTexture(GL_TEXTURE_2D, 0);

            return result;
        }

        GLuint Texture::load()
        {
            char* pixels = load_image(m_FileName.c_str(), &m_Width, &m_Height, &m_Bits);

            GLuint result;
            glGenTextures(1, &result);
            glBindTexture(GL_TEXTURE_2D, result);
#if MIP_MAP
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_NEAREST);
#else
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
#endif
            glTexImage2D(GL_TEXTURE_2D, 0,m_Bits == 32 ? GL_RGBA : GL_RGB, m_Width, m_Height,
                         0, m_Bits == 32 ? GL_BGRA: GL_BGR, GL_UNSIGNED_BYTE, pixels);
            //glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA8, m_Width, m_Height,
              //           0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
#if MIP_MAP
            glGenerateMipmap(GL_TEXTURE_2D);
#endif
            glBindTexture(GL_TEXTURE_2D, 0);

            delete[] pixels;

            return result;
        }

        void Texture::bind() const
        {
            glBindTexture(GL_TEXTURE_2D, m_TID);
        }

        void Texture::unbind() const
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        void Texture::SetPixelColor(vec2_i pixelCoords, vec4 color)
        {

        }

        void Texture::deleteTexture()
        {
            glBindTexture(GL_TEXTURE_2D, 0);
            glDeleteTextures(1, &m_TID);
        }
    } }
