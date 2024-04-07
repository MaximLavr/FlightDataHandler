#pragma once

#include <FreeImage.h>
#include <string>
#include <GL/glew.h>
#include "../utils/imageLoad.h"
#include "../maths/maths.h"
#include <fstream>

using namespace sparky::maths;

namespace sparky { namespace graphics {

        class Texture
        {
        private:
            std::string m_Name, m_FileName;
            GLsizei m_Width, m_Height;
            GLuint m_TID;
            unsigned int m_Bits;
        public:
            Texture(char* array);
            Texture(const std::string& name, const std::string& filename);
            ~Texture();
            void bind() const;
            void unbind() const;
            void deleteTexture();

            inline const std::string& getName() const { return m_Name; }
            inline const unsigned int getWidth() const { return m_Width; }
            inline const unsigned int getHeight() const { return m_Height; }
            inline const unsigned int getID() const { return m_TID; }

            void SetPixelColor(vec2_i pixelCoords, vec4 color);

        private:
            GLuint load();
            GLuint loadFromByteArray();
            char* m_CharArray = nullptr;
        };

    } }
