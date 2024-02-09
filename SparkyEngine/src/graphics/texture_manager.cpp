#include <vector>
#include "texture_manager.h"

namespace sparky { namespace graphics {

        std::vector<Texture*> TextureManager::m_Textures;

        Texture* TextureManager::add(Texture* texture)
        {
            m_Textures.push_back(texture);
            return texture;
        }

        Texture* TextureManager::get(const std::string& name)
        {
            for (Texture* texture : m_Textures)
            {
                if (texture->getName() == name)
                    return texture;
            }
            return nullptr;
        }

        void TextureManager::clean()
        {
            for (unsigned int i = 0; i < m_Textures.size(); i++)
                delete m_Textures[i];
        }

    } }