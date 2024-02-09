#pragma once
#include <cmath>
#include <chrono>

namespace sparky {
    class Timer
    {
    private:
        typedef std::chrono::high_resolution_clock HighResolutionClock;
        typedef std::chrono::duration<float, std::milli> milliseconds_type;
        std::chrono::time_point<HighResolutionClock> m_Start;
    public:
        Timer()
        {
            reset();
        }

        void reset()
        {
            m_Start = HighResolutionClock::now();
        }

        float elapsed()
        {
            return std::chrono::duration_cast<milliseconds_type>(HighResolutionClock::now() - m_Start).count() / 1000.0f;
        }

    };
    /*class Timer
    {
    private:
        clock_t m_CurrentTicks, m_DeltaTicks, frequency;
    public:
        Timer()
        {
            m_CurrentTicks = 0;
            m_DeltaTicks = 0;
            frequency = mach_absolute_time();
        }

        void reset()
        {
            m_CurrentTicks = clock();
        }

        float elapsed()
        {
            m_DeltaTicks = clock() - m_CurrentTicks;
            
            //return CLOCKS_PER_SEC / m_DeltaTicks;
            return m_DeltaTicks * frequency / pow(10, 18);
        }
    };*/
}

//#pragma once
//#include <assert.h>
//#include <CoreServices/CoreServices.h>
//#include <mach/mach.h>
//#include <mach/mach_time.h>
//#include <unistd.h>
//
//namespace sparky {
//    class Timer
//    {
//    private:
//        long long m_Start;
//        double m_Frequency;
//        uint64_t start, end, elpased;
//    public:
//        Timer()
//        {
//            long long frequency;
//            start = mach_absolute_time();
//            m_Frequency = 1.0 / frequency;
//            end = mach_absolute_time();
//        }
//
//        void reset()
//        {
//            end = mach_absolute_time();
//        }
//
//        float elapsed()
//        {
//            long long current;
//            end = mach_absolute_time();
//            unsigned int cycles = end - m_Start;
//            m_Start = end;
//            return (float)(cycles * m_Frequency);
//        }
//    };
//}