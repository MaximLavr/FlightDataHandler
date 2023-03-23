#if 1
#pragma once
#include <vector>
#include "sound.h"
#include "../../lib/gorilla-audio/include/gorilla/ga.h"
#include "../../lib/gorilla-audio/include/gorilla/gau.h"

namespace sparky { namespace audio {

    class SoundManager {
    private:
        friend class Sound;

        static gau_Manager* m_Manager;
        static ga_Mixer* m_Mixer;

        static std::vector<Sound*> m_Sounds;
    public:
        static void init();
        static void add(Sound *sound);
        static Sound* get(const std::string &name);
        static void update();
        static void clean();

    private:
        SoundManager() { }
    };

} }
#else

#pragma once

#include "/Users/maxim_lavrinenko/Documents/projects/sparky_mac1/src/sparky.h"
#include "/Users/maxim_lavrinenko/Documents/projects/sparky_mac1/src/audio/sound.h"
#include <vector>

#include "/Users/maxim_lavrinenko/Documents/projects/sparky_mac1/lib/gorilla-audio/include/gorilla/ga.h"
#include "/Users/maxim_lavrinenko/Documents/projects/sparky_mac1/lib/gorilla-audio/include/gorilla/gau.h"

namespace sparky { namespace audio {

        class SoundManager
        {
        private:
            friend class Sound;

            static gau_Manager* m_Manager;
            static ga_Mixer* m_Mixer;

            static std::vector<Sound*> m_Sounds;
        public:
            static void Init();
            static Sound* Add(Sound* sound);
            static Sound* Get(const std::string& name);
            static void Update();
            static void Clean();
        private:
            SoundManager() { }
        };

} }
#endif