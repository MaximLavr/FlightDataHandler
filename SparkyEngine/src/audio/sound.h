#if 1
#pragma once

#include <iostream>
#include "../../lib/gorilla-audio/include/gorilla/ga.h"
#include "../../lib/gorilla-audio/include/gorilla/gau.h"

namespace sparky { namespace audio {

    class Sound {
    private:
        std::string m_Name;
        std::string m_Filename;
        uint m_Count;

        ga_Sound *m_Sound;
        ga_Handle *m_Handle;
        int m_Position;


        bool m_Playing;
        float m_Gain;
    public:
        Sound(const std::string &name, const std::string &filename);

        ~Sound();

        void play();

        void loop();

        void pause();

        void resume();

        void stop();

        void setGain(float gain);

        inline const bool isPlaying() const { return m_Playing; }

        inline const float getGain() const { return m_Gain; }

        inline const std::string &getName() const { return m_Name; }

        inline const std::string &getFileName() const { return m_Filename; }

        friend void destroy_on_finish(ga_Handle *in_handle, void *in_context);

        friend void loop_on_finish(ga_Handle *in_handle, void *in_context);
    };
} }
#else

#pragma once

#include "/Users/maxim_lavrinenko/Documents/projects/sparky_mac1/src/sparky.h"
#include <iostream>

#ifndef SP_PLATFORM_WEB
struct ga_Sound;
struct ga_Handle;
#endif

namespace sparky { namespace audio {

        class Sound
        {
        private:
            std::std::string m_Name;
            std::std::string m_Filename;
            uint m_Count;

            ga_Sound* m_Sound;
            ga_Handle* m_Handle;
            int m_Position;


            bool m_Playing;
            float m_Gain;
        public:
            Sound(const std::std::string& name, const std::std::string& filename);
            ~Sound();


            void Play();
            void Loop();
            void Pause();
            void Resume();
            void Stop();

            void SetGain(float gain);

            inline const bool IsPlaying() const { return m_Playing; }
            inline const float GetGain() const { return m_Gain; }
            inline const std::std::string& GetName() const { return m_Name; }
            inline const std::std::string& GetFileName() const { return m_Filename; }

            friend void destroy_on_finish(ga_Handle* in_handle, void* in_context);
            friend void loop_on_finish(ga_Handle* in_handle, void* in_context);

        };

} }
#endif