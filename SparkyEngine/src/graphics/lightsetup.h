#pragma once

#include "light.h"

namespace sparky { namespace graphics {

        class LightSetup
                {
                        private:
                        std::vector<Light*> m_Lights;
                        public:
                        LightSetup();
                        ~LightSetup();

                        Light* Add(Light* light);
                        void Remove(Light* light);

                        inline const std::vector<Light*>& GetLights() const { return m_Lights; }
                };

    } }