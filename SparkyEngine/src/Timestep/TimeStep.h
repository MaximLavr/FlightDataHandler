#pragma once

class TimeStep
{
public:
    TimeStep(float time = 0.0f)
    {
        m_Time = time;
    }
    float GetSeconds() const { return m_Time; }
    float GetMilliseconds() const { return m_Time * 1000; }
private:
    float m_Time;
};