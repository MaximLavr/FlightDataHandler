#pragma once

#include <FreeImage.h>
#include <string>
#include <GL/glew.h>
#include "../utils/imageLoad.h"

namespace sparky { namespace graphics {

        class Texture
        {
        private:
            std::string m_Name, m_FileName;
            GLsizei m_Width, m_Height;
            GLuint m_TID;
            unsigned int m_Bits;
        public:
            Texture(const std::string& name, const std::string& filename);
            ~Texture();
            void bind() const;
            void unbind() const;

            inline const std::string& getName() const { return m_Name; }
            inline const unsigned int getWidth() const { return m_Width; }
            inline const unsigned int getHeight() const { return m_Height; }
            inline const unsigned int getID() const { return m_TID; }

        private:
            GLuint load();
        };

    } }
