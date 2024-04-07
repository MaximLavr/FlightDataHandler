#pragma once

#include "texture.h"
#include <vector>

namespace sparky { namespace graphics {

        class TextureManager
        {
        private:
            static std::vector<Texture*> m_Textures;
        public:
            static Texture* add(Texture* texture);
            static Texture* get(const std::string& name);
            static void clean();
        private:
            TextureManager() { }
        };

    } }
